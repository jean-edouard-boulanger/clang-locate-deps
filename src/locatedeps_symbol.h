#ifndef LOCATE_DEPS_SYMBOL
#define LOCATE_DEPS_SYMBOL

#include <locatedeps_context.h>
#include <locatedeps_location.h>

#include <vector>


namespace clang {
namespace find_all_symbols {

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