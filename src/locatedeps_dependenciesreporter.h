#ifndef LOCATE_DEPS_SYMBOL_REPORTER_H
#define LOCATE_DEPS_SYMBOL_REPORTER_H

#include <locatedeps_dependency.h>

#include <llvm/ADT/StringRef.h>

#include <set>


namespace clang {
namespace locate_deps {

/// \brief An interface for classes that collect symbols.
class DependenciesReporter {
public:
    virtual ~DependenciesReporter() = default;

    virtual void report(llvm::StringRef fileName,
                        const std::set<Dependency>&) = 0;
};

}  // namespace locate_deps
}  // namespace clang

#endif  // LOCATE_DEPS_SYMBOL_REPORTER_H
