#include <locatedeps_findallsymbolsaction.h>

#include <clang/Lex/PPCallbacks.h>
#include <clang/Lex/Preprocessor.h>

#include <llvm/ADT/STLExtras.h>

namespace clang {
namespace find_all_symbols {

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

}  // namespace find_all_symbols
}  // namespace clang
