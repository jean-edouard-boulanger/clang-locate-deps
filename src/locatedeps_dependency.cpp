#include <locatedeps_dependency.h>

#include <tuple>

namespace clang {
namespace locate_deps {

Dependency::Dependency(const Location& from, const Symbol& to):
    _from(from),
    _to(to)
{}

const Location&
Dependency::from() const
{
    return _from;
}

const Symbol&
Dependency::to() const
{
    return _to;
}

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