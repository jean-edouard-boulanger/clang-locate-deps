#include <locatedeps_context.h>

#include <tuple>

namespace clang {
namespace find_all_symbols {

Context::Context(Kind kind, std::string name):
    _kind(kind),
    _name(name)
{}

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