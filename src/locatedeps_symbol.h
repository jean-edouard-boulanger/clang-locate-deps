#ifndef LOCATE_DEPS_SYMBOL
#define LOCATE_DEPS_SYMBOL

#include <locatedeps_context.h>
#include <locatedeps_location.h>

#include <vector>


namespace clang {
namespace locate_deps {

class Symbol {
public:
    enum class Kind {
        Function,
        Class,
        Variable,
        Typedef,
        Enum,
        EnumConstant,
        Macro,
        Unknown,
    };

    const Location& location() const;
    const Scope& scope() const;
    Kind kind() const;
    const std::string& name() const;

    Symbol(const Location&, const Scope&, Kind kind, std::string name);

    bool operator==(const Symbol&) const;
    bool operator<(const Symbol&) const;

private:
    Location _location;
    Scope _scope;
    Kind _kind;
    std::string _name;
};


}
}

#endif