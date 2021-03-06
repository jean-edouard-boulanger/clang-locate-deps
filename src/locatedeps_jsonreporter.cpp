#include <locatedeps_jsonreporter.h>
#include <locatedeps_serialization.h>
#include <locatedeps_dependency.h>
#include <locatedeps_symbol.h>
#include <locatedeps_context.h>
#include <locatedeps_location.h>

#include <llvm/ADT/SmallString.h>
#include <llvm/Support/FileSystem.h>
#include <llvm/Support/raw_ostream.h>
#include <llvm/Support/CommandLine.h>
#include <llvm/Support/Path.h>

#include <fstream>
#include <map>


namespace clang {
namespace locate_deps {

template<>
struct EnumSerializer<Context::Kind> {
    static std::string serialize(Context::Kind value)
    {
        static const std::map<Context::Kind, std::string> mapped = {
            {Context::Kind::Namespace, "namespace"},
            {Context::Kind::Record, "class"},
            {Context::Kind::Enum, "enum"},
        };
        return mapped.at(value);
    }
};

template<>
struct EnumSerializer<Symbol::Kind> {
    static std::string serialize(Symbol::Kind value)
    {
        static const std::map<Symbol::Kind, std::string> mapped = {
            {Symbol::Kind::Function, "function"},
            {Symbol::Kind::Class, "class"},
            {Symbol::Kind::Variable, "variable"},
            {Symbol::Kind::Typedef, "typedef"},
            {Symbol::Kind::Enum, "enum"},
            {Symbol::Kind::EnumConstant, "enum_constant"},
            {Symbol::Kind::Macro, "macro"},
            {Symbol::Kind::Unknown, "unknown"},
        };
        return mapped.at(value);
    }
};

template<typename JsonValue, typename Allocator>
void
serialize(JsonValue& out,
          const Location& loc,
          Allocator& allocator)
{
    out.SetObject();
    map(out, "filename", loc.filename(), allocator);
    map(out, "line", loc.line(), allocator);
    map(out, "column", loc.column(), allocator);
}

template<typename JsonValue, typename Allocator>
void
serialize(JsonValue& out,
          const Context& context,
          Allocator& allocator)
{
    out.SetObject();
    map(out, "kind", context.kind(), allocator);
    map(out, "name", context.name(), allocator);
}


template<typename JsonValue, typename Allocator>
void
serialize(JsonValue& out,
          const Symbol& symbol,
          Allocator& allocator)
{
    out.SetObject();
    map(out, "location", symbol.location(), allocator);
    map(out, "scope", symbol.scope(), allocator);
    map(out, "kind", symbol.kind(), allocator);
    map(out, "name", symbol.name(), allocator);
}

template<typename JsonValue, typename Allocator>
void
serialize(JsonValue& out,
          const Dependency& dep,
          Allocator& allocator)
{
    out.SetObject();
    map(out, "from", dep.from(), allocator);
    map(out, "to", dep.to(), allocator);
}

void
JsonReporter::report(llvm::StringRef fileName,
                     const std::set<Dependency>& dependencies)
{
    int descriptor;
    llvm::SmallString<128> resultPath;
    llvm::sys::fs::createUniqueFile(
        _outputDirectory + "/" + llvm::sys::path::filename(fileName)
            + "-%%%%%%.json",
        descriptor, resultPath);

    std::ofstream os(resultPath.str());

    json::Document doc(json::kObjectType);
    map(doc, "dependencies", dependencies, doc.GetAllocator());

    pretty_print(os, doc);
}

}
}