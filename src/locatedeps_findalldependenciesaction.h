#ifndef LOCATE_DEPS_FIND_ALL_DEPENDENCIES_ACTION
#define LOCATE_DEPS_FIND_ALL_DEPENDENCIES_ACTION

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

class FindAllDependenciesAction : public clang::ASTFrontendAction {
public:
    explicit FindAllDependenciesAction(
        DependenciesReporter& reporter,
        const HeaderMapCollector::RegexHeaderMap* regexHeaderMap = nullptr):
            _reporter(reporter),
            _collector(regexHeaderMap),
            _matcher(reporter, &_collector)
    {
        _matcher.registerMatchers(&_matchFinder);
    }

    std::unique_ptr<clang::ASTConsumer>
    CreateASTConsumer(clang::CompilerInstance& compiler,
                      StringRef inputFile) override;

private:
    DependenciesReporter& _reporter;
    clang::ast_matchers::MatchFinder _matchFinder;
    HeaderMapCollector _collector;
    FindAllDependencies _matcher;
};

class FindAllDependenciesActionFactory : public tooling::FrontendActionFactory {
public:
    FindAllDependenciesActionFactory(
        DependenciesReporter& reporter,
        const HeaderMapCollector::RegexHeaderMap* regexHeaderMap = nullptr):
            _reporter(reporter),
            _regexHeaderMap(regexHeaderMap)
    {}

    clang::FrontendAction* create() override;

private:
    DependenciesReporter& _reporter;
    const HeaderMapCollector::RegexHeaderMap* const _regexHeaderMap;
};

}  // namespace locate_deps
}  // namespace clang

#endif  // LOCATE_DEPS_FIND_ALL_DEPENDENCIES_ACTION
