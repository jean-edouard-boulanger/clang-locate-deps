#include <locatedeps_headermapcollector.h>
#include <locatedeps_assert.h>

#include <llvm/Support/Regex.h>

namespace clang {
namespace locate_deps {

HeaderMapCollector::HeaderMapCollector(
    const RegexHeaderMap* RegexHeaderMappingTable)
{
    UNSAFE_ASSERT(RegexHeaderMappingTable);
    this->RegexHeaderMappingTable.reserve(RegexHeaderMappingTable->size());
    for (const auto& Entry: *RegexHeaderMappingTable)
    {
        this->RegexHeaderMappingTable.emplace_back(
            llvm::Regex(Entry.first), Entry.second);
    }
}

llvm::StringRef
HeaderMapCollector::getMappedHeader(llvm::StringRef Header) const
{
    auto Iter = HeaderMappingTable.find(Header);
    if (Iter != HeaderMappingTable.end()) return Iter->second;
    // If there is no complete header name mapping for this header, check the
    // regex header mapping.
    for (auto& Entry : RegexHeaderMappingTable)
    {
        if (Entry.first.match(Header)) return Entry.second;
    }
    return Header;
}

}  // namespace locate_deps
}  // namespace clang
