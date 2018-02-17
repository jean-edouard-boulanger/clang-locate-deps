#include <locatedeps_contextfactory.h>
#include <locatedeps_context.h>

#include <clang/AST/Decl.h>
#include <clang/AST/DeclCXX.h>


namespace clang {
namespace find_all_symbols {

Scope
build_scope(const NamedDecl& namedDecl)
{
    Scope scope;
    for (const auto* declContext = namedDecl.getDeclContext();
         declContext;
         declContext = declContext->getParent())
    {
        if (llvm::isa<TranslationUnitDecl>(declContext) ||
            llvm::isa<LinkageSpecDecl>(declContext))
        {
            break;
        }

        assert(llvm::isa<NamedDecl>(declContext) &&
               "Expect declContext to be a NamedDecl");

        if (const auto* namespaceDecl = dyn_cast<NamespaceDecl>(declContext))
        {
            if(namespaceDecl->isInlineNamespace()) continue;

            scope.push_back(
                Context(Context::Kind::Namespace,
                        namespaceDecl->getName().str()));
        }
        else if (const auto* enumDecl = dyn_cast<EnumDecl>(declContext))
        {
            scope.push_back(
                Context(Context::Kind::EnumDecl,
                        namespaceDecl->getName().str()));
        }
        else if (const auto* recordDecl = dyn_cast<RecordDecl>(declContext))
        {
            scope.push_back(
                Context(Context::Kind::Record,
                        namespaceDecl->getName().str()));
        }
        else
        {
            assert(false &&
                "recordDecl must be NamespaceDecl or EnumDecl or RecordDecl");
        }
    }
    return scope;
}

}
}