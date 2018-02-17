#ifndef LOCATE_DEPS_SYMBOL_REPORTER_H
#define LOCATE_DEPS_SYMBOL_REPORTER_H

#include <locatedeps_symbolinfo.h>

namespace clang {
namespace locate_deps {

/// \brief An interface for classes that collect symbols.
class SymbolReporter {
public:
    virtual ~SymbolReporter() = default;

    virtual void reportSymbols(llvm::StringRef FileName,
                               const SymbolInfo::SignalMap& Symbols) = 0;
};

}  // namespace locate_deps
}  // namespace clang

#endif  // LOCATE_DEPS_SYMBOL_REPORTER_H
