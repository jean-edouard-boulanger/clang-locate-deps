#include <locatedeps_findallsymbolsaction.h>

#include <clang/Lex/PPCallbacks.h>
#include <clang/Lex/Preprocessor.h>

#include <llvm/ADT/STLExtras.h>

namespace clang {
namespace locate_deps {

FindAllSymbolsAction::FindAllSymbolsAction(
    SymbolReporter* Reporter,
    const HeaderMapCollector::RegexHeaderMap* RegexHeaderMap):
        Reporter(Reporter),
        Collector(RegexHeaderMap),
        Matcher(Reporter, &Collector)
{
    Matcher.registerMatchers(&MatchFinder);
}

std::unique_ptr<ASTConsumer> FindAllSymbolsAction::CreateASTConsumer(
    CompilerInstance& Compiler, StringRef InFile)
{
    return MatchFinder.newASTConsumer();
}

}  // namespace locate_deps
}  // namespace clang
