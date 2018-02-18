#ifndef LOCATE_DEPS_JSON_REPORTER
#define LOCATE_DEPS_JSON_REPORTER

#include <locatedeps_dependency.h>
#include <locatedeps_dependenciesreporter.h>

#include <llvm/ADT/StringRef.h>

#include <string>
#include <set>


namespace clang {
namespace locate_deps {

class JsonReporter : public DependenciesReporter {
public:
    void report(llvm::StringRef filename,
                const std::set<Dependency>& dependencies) override;
};

}
}

#endif