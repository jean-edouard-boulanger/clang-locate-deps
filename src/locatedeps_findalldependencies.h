#ifndef LOCATE_DEPS_FIND_ALL_DEPENDENCIES
#define LOCATE_DEPS_FIND_ALL_DEPENDENCIES

#include <locatedeps_dependency.h>

#include <clang/ASTMatchers/ASTMatchFinder.h>

#include <llvm/ADT/Optional.h>

#include <string>
#include <set>

namespace clang {
namespace locate_deps {

class HeaderMapCollector;
class DependenciesReporter;

class FindAllDependencies : public ast_matchers::MatchFinder::MatchCallback {
public:
    explicit FindAllDependencies(DependenciesReporter& reporter,
                                 HeaderMapCollector* collector = nullptr);

    void registerMatchers(ast_matchers::MatchFinder* MatchFinder);

    void run(const ast_matchers::MatchFinder::MatchResult& result) override;

protected:
    void onEndOfTranslationUnit() override;

private:
    llvm::Optional<std::string> _filename;
    std::set<Dependency> _dependencies;
    DependenciesReporter& _reporter;
    HeaderMapCollector* const _collector;
};

}  // namespace locate_deps
}  // namespace clang

#endif  // LOCATE_DEPS_FIND_ALL_DEPENDENCIES
