#include <locatedeps_pathconfig.h>

#include <llvm/Support/Path.h>

namespace clang {
namespace locate_deps {

std::string
getIncludePath(const SourceManager& sourceManager,
               SourceLocation location,
               const HeaderMapCollector* collector)
{
    llvm::StringRef filePath;

    // Walk up the include stack to skip .inc files.
    while (true)
    {
        if (!location.isValid() || sourceManager.isInMainFile(location))
        {
            return std::string();
        }

        filePath = sourceManager.getFilename(location);
        if (filePath.empty())
        {
            return std::string();
        }

        if (!filePath.endswith(".inc"))
        {
            break;
        }

        location = sourceManager.getIncludeLoc(
            sourceManager.getFileID(location));
    }

    if (collector)
    {
        filePath = collector->getMappedHeader(filePath);
    }

    const bool removeDotDot = false;
    SmallString<256> cleanFilePath = filePath;
    llvm::sys::path::remove_dots(cleanFilePath, removeDotDot);

    return cleanFilePath.str();
}

}  // namespace locate_deps
}  // namespace clang
