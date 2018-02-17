#ifndef LOCATE_DEPS_CONTEXT
#define LOCATE_DEPS_CONTEXT

#include <string>
#include <vector>

namespace clang {
namespace find_all_symbols {

class Context {
public:
    enum class Kind {
        Namespace,  // Symbols declared in a namespace.
        Record,     // Symbols declared in a class.
        EnumDecl,   // Enum constants declared in a enum declaration.
    };
    Context(Kind kind, std::string name);

    bool operator==(const Context&) const;
    bool operator<(const Context&) const;

private:
    Kind _kind;
    std::string _name;
};

using Scope = std::vector<Context>;

}
}

#endif