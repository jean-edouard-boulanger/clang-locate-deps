#ifndef LOCATE_DEPS_PATH_CONFIG_H
#define LOCATE_DEPS_PATH_CONFIG_H

#include <locatedeps_headermapcollector.h>

#include <clang/Basic/SourceManager.h>

#include <string>

namespace clang {
namespace locate_deps {

/// \brief This calculates the include path for \p Loc.
///
/// \param SM SourceManager.
/// \param Loc A SourceLocation.
/// \param Collector An optional header mapping collector.
///
/// \return The file path (or mapped file path if Collector is provided) of the
/// header that includes \p Loc. If \p Loc comes from .inc header file, \p Loc
/// is set to the location from which the .inc header file is included. If \p
/// Loc is invalid or comes from a main file, this returns an empty string.
std::string getIncludePath(const SourceManager& SM, SourceLocation Loc,
                           const HeaderMapCollector* Collector = nullptr);

}  // namespace locate_deps
}  // namespace clang

#endif  // LOCATE_DEPS_PATH_CONFIG_H
