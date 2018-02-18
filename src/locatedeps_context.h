#ifndef LOCATE_DEPS_CONTEXT
#define LOCATE_DEPS_CONTEXT

#include <string>
#include <vector>

namespace clang {
namespace locate_deps {

class Context {
public:
    enum class Kind {
        Namespace,  // Symbols declared in a namespace.
        Record, // Symbols declared in a class.
        Enum, // Enum constants declared in a enum declaration.
    };
    Context(Kind kind, std::string name);

    Kind kind() const;
    const std::string& name() const;

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