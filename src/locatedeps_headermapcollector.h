#ifndef LOCATE_DEPS_HEADER_MAP_COLLECTOR_H
#define LOCATE_DEPS_HEADER_MAP_COLLECTOR_H

#include <llvm/ADT/StringMap.h>
#include <llvm/Support/Regex.h>

#include <string>
#include <vector>

namespace clang {
namespace locate_deps {

/// \brief HeaderMappCollector collects all remapping header files. This maps
/// complete header names or header name regex patterns to header names.
class HeaderMapCollector {
public:
    typedef llvm::StringMap<std::string> HeaderMap;
    typedef std::vector<std::pair<const char*, const char*>> RegexHeaderMap;

    HeaderMapCollector() = default;
    explicit HeaderMapCollector(const RegexHeaderMap* RegexHeaderMappingTable);

    void addHeaderMapping(llvm::StringRef OrignalHeaderPath,
                          llvm::StringRef MappingHeaderPath) {
        HeaderMappingTable[OrignalHeaderPath] = MappingHeaderPath;
    };

    /// Check if there is a mapping from \p Header or a regex pattern that
    /// matches it to another header name. \param Header A header name. \return
    /// \p Header itself if there is no mapping for it; otherwise, return a
    /// mapped header name.
    llvm::StringRef getMappedHeader(llvm::StringRef Header) const;

private:
    /// A string-to-string map saving the mapping relationship.
    HeaderMap HeaderMappingTable;

    // A map from header patterns to header names.
    // The header names are not owned. This is only threadsafe because the
    // regexes never fail.
    mutable std::vector<std::pair<llvm::Regex, const char*>>
        RegexHeaderMappingTable;
};

}  // namespace locate_deps
}  // namespace clang

#endif  // LOCATE_DEPS_HEADER_MAP_COLLECTOR_H
