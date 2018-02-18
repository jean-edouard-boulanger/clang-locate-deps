#ifndef LOCATE_DEPS_JSON_REPORTER
#define LOCATE_DEPS_JSON_REPORTER

#include <locatedeps_dependency.h>

#include <string>
#include <set>


namespace clang {
namespace locate_deps {

class JsonReporter {
public:
    void report(const std::string& filename,
                const std::set<Dependency>& dependencies);
};

}
}

#endif