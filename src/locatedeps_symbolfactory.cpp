#include <locatedeps_symbolfactory.h>
#include <locatedeps_contextfactory.h>
#include <locatedeps_symbol.h>
#include <locatedeps_location.h>
#include <locatedeps_headermapcollector.h>
#include <locatedeps_pathconfig.h>

#include <clang/AST/Decl.h>
#include <clang/AST/DeclCXX.h>
#include <clang/Basic/SourceManager.h>


namespace clang {
namespace locate_deps {
namespace {

std::string
prettyArguments(clang::FunctionDecl::param_const_iterator first,
                clang::FunctionDecl::param_const_iterator last)
{
    std::string output;
    while(first != last)
    {
        output += (*first)->getOriginalType().getAsString();
        if((++first) != last)
        {
            output += ", ";
        }
    }
    return output;
}

std::string
make_symbol_name(const FunctionDecl& f)
{
    return f.getReturnType().getAsString()
           + std::string(" ")
           + f.getQualifiedNameAsString()
           + std::string("(")
           + prettyArguments(f.param_begin(), f.param_end())
           + std::string(")");
}

std::string
make_symbol_name(const VarDecl& varDecl)
{
    return varDecl.getType().getAsString()
           + std::string(" ")
           + varDecl.getNameAsString();
}

std::string
make_symbol_name(const NamedDecl& namedDecl)
{
    if(llvm::isa<FunctionDecl>(namedDecl))
    {
        return make_symbol_name(
            *llvm::dyn_cast<FunctionDecl>(&namedDecl));
    }
    if(llvm::isa<VarDecl>(namedDecl))
    {
        return make_symbol_name(
            *llvm::dyn_cast<VarDecl>(&namedDecl));
    }
    return namedDecl.getNameAsString();
}

llvm::Optional<Symbol::Kind>
get_symbol_kind(const NamedDecl& namedDecl)
{
    if(llvm::isa<VarDecl>(namedDecl))
    {
        return Symbol::Kind::Variable;
    }
    if(llvm::isa<FunctionDecl>(namedDecl))
    {
        return Symbol::Kind::Function;
    }
    if(llvm::isa<TypedefNameDecl>(namedDecl))
    {
        return Symbol::Kind::Typedef;
    }
    if(llvm::isa<EnumConstantDecl>(namedDecl))
    {
        return Symbol::Kind::EnumConstant;
    }
    if(llvm::isa<EnumDecl>(namedDecl))
    {
        // Ignore anonymous enum declarations.
        if (namedDecl.getName().empty()) return llvm::None;
        return Symbol::Kind::Enum;
    }
    if(llvm::isa<RecordDecl>(namedDecl))
    {
        // C-style record decl can have empty name, e.g "struct { ... } var;".
        if (namedDecl.getName().empty()) return llvm::None;
        return Symbol::Kind::Class;
    }
    return llvm::None;
}

}

llvm::Optional<Symbol>
build_symbol(const NamedDecl& namedDecl,
             const SourceManager& sourceManager,
             const HeaderMapCollector* collector)
{
    const llvm::Optional<Symbol::Kind> symbolKind = get_symbol_kind(namedDecl);
    if(!symbolKind)
    {
        return llvm::None;
    }

    const SourceLocation symbolLocation = sourceManager.getExpansionLoc(
        namedDecl.getLocation());

    if(!symbolLocation.isValid())
    {
        return llvm::None;
    }

    const std::string symbolFilePath = getIncludePath(
        sourceManager, symbolLocation, collector);

    if(symbolFilePath.empty())
    {
        return llvm::None;
    }

    const Location fullLocation(
        symbolFilePath,
        sourceManager.getExpansionLineNumber(symbolLocation),
        sourceManager.getExpansionColumnNumber(symbolLocation));

    return Symbol(
        fullLocation,
        build_scope(namedDecl),
        *symbolKind,
        make_symbol_name(namedDecl));
}

}
}