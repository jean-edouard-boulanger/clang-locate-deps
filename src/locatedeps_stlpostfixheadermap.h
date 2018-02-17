#ifndef LOCATE_DEPS_STL_POSTFIX_HEADER_MAP_H
#define LOCATE_DEPS_STL_POSTFIX_HEADER_MAP_H

#include <locatedeps_headermapcollector.h>

namespace clang {
namespace find_all_symbols {

const HeaderMapCollector::RegexHeaderMap* getSTLPostfixHeaderMap();

}  // namespace find_all_symbols
}  // namespace clang

#endif  // LOCATE_DEPS_STL_POSTFIX_HEADER_MAP_H
