#ifndef LOCATE_DEPS_STL_POSTFIX_HEADER_MAP_H
#define LOCATE_DEPS_STL_POSTFIX_HEADER_MAP_H

#include <locatedeps_headermapcollector.h>

namespace clang {
namespace locate_deps {

const HeaderMapCollector::RegexHeaderMap* getSTLPostfixHeaderMap();

}  // namespace locate_deps
}  // namespace clang

#endif  // LOCATE_DEPS_STL_POSTFIX_HEADER_MAP_H
