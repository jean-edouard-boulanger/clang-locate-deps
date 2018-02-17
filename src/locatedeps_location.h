#ifndef LOCATE_DEPS_LOCATION
#define LOCATE_DEPS_LOCATION

#include <string>
#include <cstddef>

namespace clang {
namespace find_all_symbols {

class Location {
public:
    Location(const std::string& filename, size_t line, size_t column);

    const std::string& filename() const;
    std::size_t line() const;
    std::size_t column() const;

    bool operator==(const Location& other) const;
    bool operator<(const Location& other) const;

private:
    std::string _filename;
    std::size_t _line;
    std::size_t _column;
};

}
}

#endif