#include <locatedeps_location.h>

#include <tuple>

namespace clang {
namespace find_all_symbols {

Location::Location(const std::string& filename, size_t line, size_t column):
    _filename(filename),
    _line(line),
    _column(column)
{}

const std::string&
Location::filename() const
{
    return _filename;
}

std::size_t
Location::line() const
{
    return _line;
}

std::size_t
Location::column() const
{
    return _column;
}

bool
Location::operator==(const Location& other) const
{
    return std::tie(_filename, _line, _column) ==
        std::tie(other._filename, other._line, other._column);
}

bool
Location::operator<(const Location& other) const
{
    return std::tie(_filename, _line, _column) <
        std::tie(other._filename, other._line, other._column);
}

}
}