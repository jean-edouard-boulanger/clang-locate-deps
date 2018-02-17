#ifndef LOCATE_DEPS_CONTEXT_FACTORY
#define LOCATE_DEPS_CONTEXT_FACTORY

#include <locatedeps_context.h>

namespace clang { class NamedDecl; }
namespace clang {
namespace locate_deps {

Scope
build_scope(const NamedDecl&);

}
}

#endif