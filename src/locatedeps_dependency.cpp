#include <locatedeps_dependency.h>

#include <tuple>

namespace clang {
namespace find_all_symbols {

Dependency::Dependency(const Location& from, const Symbol& to):
    _from(from),
    _to(to)
{}

bool
Dependency::operator==(const Dependency& other) const
{
    return std::tie(_from, _to) == std::tie(other._from, other._to);
}

bool
Dependency::operator<(const Dependency& other) const
{
    return std::tie(_from, _to) < std::tie(other._from, other._to);
}

}
}