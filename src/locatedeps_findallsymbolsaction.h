#ifndef LOCATE_DEPS_FIND_ALL_SYMBOLS_ACTION_H
#define LOCATE_DEPS_FIND_ALL_SYMBOLS_ACTION_H

#include <locatedeps_findalldependencies.h>
#include <locatedeps_headermapcollector.h>

#include <clang/ASTMatchers/ASTMatchFinder.h>
#include <clang/Frontend/CompilerInstance.h>
#include <clang/Frontend/FrontendAction.h>
#include <clang/Tooling/Tooling.h>

#include <llvm/ADT/StringRef.h>

#include <memory>

namespace clang {
namespace locate_deps {

class FindAllSymbolsAction : public clang::ASTFrontendAction {
public:
    explicit FindAllSymbolsAction(
        SymbolReporter* Reporter,
        const HeaderMapCollector::RegexHeaderMap* RegexHeaderMap = nullptr);

    std::unique_ptr<clang::ASTConsumer> CreateASTConsumer(
        clang::CompilerInstance& Compiler, StringRef InFile) override;

private:
    SymbolReporter* const Reporter;
    clang::ast_matchers::MatchFinder MatchFinder;
    HeaderMapCollector Collector;
    FindAllDependencies Matcher;
};

class FindAllSymbolsActionFactory : public tooling::FrontendActionFactory {
public:
    FindAllSymbolsActionFactory(
        SymbolReporter* Reporter,
        const HeaderMapCollector::RegexHeaderMap* RegexHeaderMap = nullptr)
        : Reporter(Reporter), RegexHeaderMap(RegexHeaderMap) {}

    clang::FrontendAction* create() override {
        return new FindAllSymbolsAction(Reporter, RegexHeaderMap);
    }

private:
    SymbolReporter* const Reporter;
    const HeaderMapCollector::RegexHeaderMap* const RegexHeaderMap;
};

}  // namespace locate_deps
}  // namespace clang

#endif  // LOCATE_DEPS_FIND_ALL_SYMBOLS_ACTION_H
