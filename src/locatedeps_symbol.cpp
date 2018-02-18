#include <locatedeps_symbol.h>

#include <tuple>

namespace clang {
namespace locate_deps {

Symbol::Symbol(const Location& location,
               const Scope& scope,
               Kind kind,
               std::string name):
                   _location(location),
                   _scope(scope),
                   _kind(kind),
                   _name(name)
{}

const Location&
Symbol::location() const
{
    return _location;
}

const Scope&
Symbol::scope() const
{
    return _scope;
}

Symbol::Kind
Symbol::kind() const
{
    return _kind;
}

const std::string&
Symbol::name() const
{
    return _name;
}

bool
Symbol::operator==(const Symbol& other) const
{
    return std::tie(_location, _scope, _kind, _name) ==
        std::tie(other._location, other._scope, other._kind, other._name);
}

bool
Symbol::operator<(const Symbol& other) const
{
    return std::tie(_location, _scope, _kind, _name) <
        std::tie(other._location, other._scope, other._kind, other._name);
}

}
}