#include <locatedeps_findalldependenciesaction.h>

#include <llvm/ADT/STLExtras.h>


namespace clang {
namespace locate_deps {

std::unique_ptr<ASTConsumer>
FindAllDependenciesAction::CreateASTConsumer(
    CompilerInstance& Compiler, StringRef InFile)
{
    return _matchFinder.newASTConsumer();
}

clang::FrontendAction*
FindAllDependenciesActionFactory::create()
{
    return new FindAllDependenciesAction(_reporter, _regexHeaderMap);
}

}  // namespace locate_deps
}  // namespace clang
