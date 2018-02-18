#include <locatedeps_findalldependenciesaction.h>
#include <locatedeps_stlpostfixheadermap.h>
#include <locatedeps_symbolinfo.h>
#include <locatedeps_jsonreporter.h>

#include <clang/ASTMatchers/ASTMatchFinder.h>
#include <clang/ASTMatchers/ASTMatchers.h>
#include <clang/Frontend/CompilerInstance.h>
#include <clang/Frontend/FrontendActions.h>
#include <clang/Lex/Preprocessor.h>
#include <clang/Tooling/CommonOptionsParser.h>
#include <clang/Tooling/Tooling.h>

#include <llvm/ADT/ArrayRef.h>
#include <llvm/ADT/SmallString.h>
#include <llvm/ADT/StringRef.h>
#include <llvm/Support/CommandLine.h>
#include <llvm/Support/FileSystem.h>
#include <llvm/Support/MemoryBuffer.h>
#include <llvm/Support/Path.h>
#include <llvm/Support/ThreadPool.h>
#include <llvm/Support/raw_ostream.h>

#include <map>
#include <mutex>
#include <set>
#include <string>
#include <system_error>
#include <vector>

using namespace clang::tooling;
using namespace llvm;
using SymbolInfo = clang::locate_deps::SymbolInfo;

// Apply a custom category to all command-line options so that they are the
// only ones displayed.
static cl::OptionCategory findAllDependenciesCategory("locate_deps options");

// CommonOptionsParser declares HelpMessage with a description of the common
// command-line options related to the compilation database and input files.
// It's nice to have this help message in all tools.
static cl::extrahelp commonHelp(CommonOptionsParser::HelpMessage);

static cl::opt<std::string> outputDir("output-dir", cl::desc(R"(
The output directory for saving the results.)"),
                                      cl::init("."),
                                      cl::cat(findAllDependenciesCategory));

namespace clang {
namespace locate_deps {

/*
class YamlReporter : public SymbolReporter {
public:
    void reportSymbols(StringRef FileName,
                       const SymbolInfo::SignalMap& symbols) override
    {
        int descriptor;
        SmallString<128> resultPath;
        llvm::sys::fs::createUniqueFile(
            outputDir + "/" + llvm::sys::path::filename(FileName) +
                "-%%%%%%.yaml",
            descriptor, resultPath);

        const bool shouldClose = true;
        llvm::raw_fd_ostream os(descriptor, shouldClose);
        WriteSymbolInfosToStream(os, symbols);
    }
};
*/

}  // namespace locate_deps
}  // namespace clang

int main(int argc, const char** argv) {
    CommonOptionsParser optionsParser(argc, argv, findAllDependenciesCategory);

    ClangTool tool(optionsParser.getCompilations(),
                   optionsParser.getSourcePathList());

    const std::vector<std::string> sources = optionsParser.getSourcePathList();
    if (sources.empty())
    {
        llvm::errs() << "Must specify at least one one source file.\n";
        return 1;
    }

    clang::locate_deps::JsonReporter reporter;

    auto Factory =
        llvm::make_unique<clang::locate_deps::FindAllDependenciesActionFactory>(
            reporter, clang::locate_deps::getSTLPostfixHeaderMap());

    return tool.run(Factory.get());
}
