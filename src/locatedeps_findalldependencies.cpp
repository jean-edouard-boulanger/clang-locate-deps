#include <locatedeps_findalldependencies.h>
#include <locatedeps_headermapcollector.h>
#include <locatedeps_pathconfig.h>
#include <locatedeps_symbolinfo.h>
#include <locatedeps_dependency.h>
#include <locatedeps_location.h>
#include <locatedeps_symbolfactory.h>
#include <locatedeps_symbol.h>
#include <locatedeps_assert.h>
#include <locatedeps_jsonreporter.h>

#include <clang/AST/Decl.h>
#include <clang/AST/DeclCXX.h>
#include <clang/AST/Type.h>
#include <clang/ASTMatchers/ASTMatchFinder.h>
#include <clang/ASTMatchers/ASTMatchers.h>
#include <clang/Tooling/Tooling.h>

#include <llvm/ADT/Optional.h>
#include <llvm/Support/FileSystem.h>

using namespace clang::ast_matchers;

namespace clang {
namespace locate_deps {
namespace {

AST_MATCHER(EnumConstantDecl, isInScopedEnum) {
    if (const auto* enumDecl = dyn_cast<EnumDecl>(Node.getDeclContext()))
        return enumDecl->isScoped();
    return false;
}

AST_POLYMORPHIC_MATCHER(isFullySpecialized,
                        AST_POLYMORPHIC_SUPPORTED_TYPES(FunctionDecl, VarDecl,
                                                        CXXRecordDecl))
{
    if (Node.getTemplateSpecializationKind() == TSK_ExplicitSpecialization)
    {
        const bool isPartialSpecialization =
            llvm::isa<VarTemplatePartialSpecializationDecl>(Node) ||
            llvm::isa<ClassTemplatePartialSpecializationDecl>(Node);
        return !isPartialSpecialization;
    }
    return false;
}

}  // namespace

FindAllDependencies::FindAllDependencies(SymbolReporter* reporter,
                                         HeaderMapCollector* collector):
                                             _reporter(reporter),
                                             _collector(collector)
{}

void
FindAllDependencies::registerMatchers(MatchFinder* MatchFinder)
{
    // FIXME: Handle specialization.
    auto IsInSpecialization = hasAncestor(
        decl(anyOf(cxxRecordDecl(isExplicitTemplateSpecialization()),
                   functionDecl(isExplicitTemplateSpecialization()))));

    // Matchers for both C and C++.
    // We only match symbols from header files, i.e. not from main files (see
    // function's comment for detailed explanation).
    auto CommonFilter =
        allOf(unless(isImplicit()), unless(isExpansionInMainFile()));

    auto HasNSOrTUCtxMatcher =
        hasDeclContext(anyOf(namespaceDecl(), translationUnitDecl()));

    // We need seperate rules for C record types and C++ record types since some
    // template related matchers are inapplicable on C record declarations.
    //
    // Matchers specific to C++ code.
    // All declarations should be in namespace or translation unit.
    auto CCMatcher =
        allOf(HasNSOrTUCtxMatcher, unless(IsInSpecialization),
              unless(ast_matchers::isTemplateInstantiation()),
              unless(isInstantiated()), unless(isFullySpecialized()));

    // Matchers specific to code in extern "C" {...}.
    auto ExternCMatcher = hasDeclContext(linkageSpecDecl());

    // Matchers for variable declarations.
    //
    // In most cases, `ParmVarDecl` is filtered out by hasDeclContext(...)
    // matcher since the declaration context is usually `MethodDecl`. However,
    // this assumption does not hold for parameters of a function pointer
    // parameter.
    // For example, consider a function declaration:
    //        void Func(void (*)(float), int);
    // The float parameter of the function pointer has an empty name, and its
    // declaration context is an anonymous namespace; therefore, it won't be
    // filtered out by our matchers above.
    auto Vars = varDecl(CommonFilter, anyOf(ExternCMatcher, CCMatcher),
                        unless(parmVarDecl()));

    // Matchers for C-style record declarations in extern "C" {...}.
    auto CRecords = recordDecl(CommonFilter, ExternCMatcher, isDefinition());
    // Matchers for C++ record declarations.
    auto CXXRecords = cxxRecordDecl(CommonFilter, CCMatcher, isDefinition());

    // Matchers for function declarations.
    // We want to exclude friend declaration, but the `DeclContext` of a friend
    // function declaration is not the class in which it is declared, so we need
    // to explicitly check if the parent is a `friendDecl`.
    auto Functions = functionDecl(CommonFilter, unless(hasParent(friendDecl())),
                                  anyOf(ExternCMatcher, CCMatcher));

    // Matcher for typedef and type alias declarations.
    //
    // typedef and type alias can come from C-style headers and C++ headers.
    // For C-style headers, `DeclContxet` can be either `TranslationUnitDecl`
    // or `LinkageSpecDecl`.
    // For C++ headers, `DeclContext ` can be either `TranslationUnitDecl`
    // or `NamespaceDecl`.
    // With the following context matcher, we can match `typedefNameDecl` from
    // both C-style headers and C++ headers (except for those in classes).
    // "cc_matchers" are not included since template-related matchers are not
    // applicable on `TypedefNameDecl`.
    auto Typedefs = typedefNameDecl(
        CommonFilter,
        anyOf(HasNSOrTUCtxMatcher, hasDeclContext(linkageSpecDecl())));

    // Matchers for enum declarations.
    auto Enums = enumDecl(CommonFilter, isDefinition(),
                          anyOf(HasNSOrTUCtxMatcher, ExternCMatcher));

    // Matchers for enum constant declarations.
    // We only match the enum constants in non-scoped enum declarations which
    // are inside toplevel translation unit or a namespace.
    auto EnumConstants = enumConstantDecl(
        CommonFilter, unless(isInScopedEnum()),
        anyOf(hasDeclContext(enumDecl(HasNSOrTUCtxMatcher)), ExternCMatcher));

    // Most of the time we care about all matchable decls, or all types.
    auto Types = namedDecl(anyOf(CRecords, CXXRecords, Enums));
    auto Decls = namedDecl(anyOf(CRecords, CXXRecords, Enums, Typedefs, Vars,
                                 EnumConstants, Functions));

    // ... and all uses of them bound to "use". These have many cases:
    // Uses of values/functions: these generate a declRefExpr.
    MatchFinder->addMatcher(
        declRefExpr(isExpansionInMainFile(), to(Decls.bind("use")))
            .bind("client"),
        this);
    // Uses of function templates:
    MatchFinder->addMatcher(
        declRefExpr(isExpansionInMainFile(),
                    to(functionDecl(hasParent(
                        functionTemplateDecl(has(Functions.bind("use")))))))
            .bind("client"),
        this);

    // Uses of most types: just look at what the typeLoc refers to.
    MatchFinder->addMatcher(
        typeLoc(isExpansionInMainFile(),
                loc(qualType(hasDeclaration(Types.bind("use")))))
            .bind("client"),
        this);
    // Uses of typedefs: these are often transparent to hasDeclaration, so we
    // need to handle them explicitly.
    MatchFinder->addMatcher(
        typeLoc(isExpansionInMainFile(),
                loc(typedefType(hasDeclaration(Typedefs.bind("use")))))
            .bind("client"),
        this);
    // Uses of class templates:
    // The typeLoc names the templateSpecializationType. Its declaration is the
    // ClassTemplateDecl, which contains the CXXRecordDecl we want.

    MatchFinder->addMatcher(
        typeLoc(isExpansionInMainFile(),
                loc(templateSpecializationType(hasDeclaration(
                    classTemplateDecl(has(CXXRecords.bind("use")))))))
            .bind("client"),
        this);
}

void
FindAllDependencies::run(const MatchFinder::MatchResult& result)
{
    // Ignore Results in failing TUs.
    if (result.Context->getDiagnostics().hasErrorOccurred())
    {
        return;
    }

    const SourceManager& sourceManager = *(result.SourceManager);

    SymbolInfo::Signals Signals;
    const NamedDecl* namedDecl;

    const std::string mainFileName = sourceManager.getFileEntryForID(
        sourceManager.getMainFileID())->getName();

    if ((namedDecl = result.Nodes.getNodeAs<NamedDecl>("use")))
    {
        llvm::Optional<SourceLocation> clientLocation;
        if (const auto* client = result.Nodes.getNodeAs<DeclRefExpr>("client"))
        {
            clientLocation = client->getLocStart();
        }
        else if (const auto* client = result.Nodes.getNodeAs<TypeLoc>("client"))
        {
            clientLocation = client->getLocStart();
        }
        else
        {
            return;
        }

        // TODO: understand why this need to be done
        if(!sourceManager.isInMainFile(*clientLocation))
        {
            return;
        }

        const Location fromLocation(
            mainFileName,
            sourceManager.getExpansionLineNumber(*clientLocation),
            sourceManager.getExpansionColumnNumber(*clientLocation));

        const llvm::Optional<Symbol> toSymbol = build_symbol(
            *namedDecl, sourceManager, _collector);

        if(toSymbol)
        {
            const Dependency dependency(fromLocation, *toSymbol);
            _dependencies.insert(dependency);
            _filename = mainFileName;
        }
    }
    else
    {
        UNSAFE_ASSERT(false && "Must match a NamedDecl!");
    }
}

void
FindAllDependencies::onEndOfTranslationUnit()
{
    if (_filename)
    {
        llvm::errs() << "found " << _dependencies.size()
                     << " dependencies in " << *_filename << "\n";

        JsonReporter reporter;
        reporter.report("", _dependencies);

        _dependencies.clear();
        _filename.reset();
    }
}

}  // namespace locate_deps
}  // namespace clang
