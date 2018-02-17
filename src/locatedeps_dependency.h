#ifndef LOCATE_DEPS_DEPENDENCY
#define LOCATE_DEPS_DEPENDENCY

#include <locatedeps_symbol.h>
#include <locatedeps_location.h>

#include <string>

namespace clang {
namespace find_all_symbols {

class Dependency {
public:
    Dependency(const Location& from, const Symbol& to);

    bool operator==(const Dependency&) const;
    bool operator<(const Dependency&) const;

private:
    Location _from;
    Symbol _to;
};

}
}

#endif