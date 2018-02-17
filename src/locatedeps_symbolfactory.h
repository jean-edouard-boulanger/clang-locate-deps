#ifndef LOCATE_DEPS_SYMBOL_FACTORY
#define LOCATE_DEPS_SYMBOL_FACTORY

#include <locatedeps_symbol.h>
#include <llvm/ADT/Optional.h>


namespace clang { class NamedDecl; }
namespace clang { class SourceManager; }

namespace clang {
namespace find_all_symbols {

class HeaderMapCollector;

llvm::Optional<Symbol>
build_symbol(const NamedDecl&,
             const SourceManager&,
             const HeaderMapCollector* = nullptr);

}
}

#endif