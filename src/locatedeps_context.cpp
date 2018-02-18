#include <locatedeps_context.h>

#include <tuple>

namespace clang {
namespace locate_deps {

Context::Context(Kind kind, std::string name):
    _kind(kind),
    _name(name)
{}

Context::Kind
Context::kind() const
{
    return _kind;
}

const std::string&
Context::name() const
{
    return _name;
}

bool
Context::operator==(const Context& other) const
{
    return std::tie(_kind, _name) < std::tie(other._kind, other._name);
}

bool
Context::operator<(const Context& other) const
{
    return std::tie(_kind, _name) < std::tie(other._kind, other._name);
}

}
}