#ifndef LOCATE_DEPS_FIND_ALL_DEPENDENCIES
#define LOCATE_DEPS_FIND_ALL_DEPENDENCIES

#include <locatedeps_symbolinfo.h>
#include <locatedeps_dependenciesreporter.h>
#include <locatedeps_dependency.h>

#include <clang/ASTMatchers/ASTMatchFinder.h>

#include <llvm/ADT/Optional.h>

#include <string>
#include <set>

namespace clang {
namespace locate_deps {

class HeaderMapCollector;

/// \brief FindAllSymbols collects all classes, free standing functions and
/// global variables with some extra information such as the path of the header
/// file, the namespaces they are contained in, the type of variables and the
/// parameter types of functions.
///
/// NOTE:
///   - Symbols declared in main files are not collected since they can not be
///   included.
///   - Member functions are not collected because accessing them must go
///   through the class. #include fixer only needs the class name to find
///   headers.
///
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
