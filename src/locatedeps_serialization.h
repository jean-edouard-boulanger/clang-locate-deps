#ifndef LOCATE_DEPS_SERIALIZATION
#define LOCATE_DEPS_SERIALIZATION

#include <locatedeps_assert.h>

#include <rapidjson/document.h>
#include <rapidjson/stringbuffer.h>
#include <rapidjson/writer.h>
#include <rapidjson/prettywriter.h>
#include <rapidjson/ostreamwrapper.h>

#include <fstream>
#include <string>
#include <set>
#include <type_traits>
#include <vector>


namespace json = rapidjson;

namespace clang {
namespace locate_deps {

template<typename T, typename RT>
using allow_numericals =
    typename std::enable_if<std::is_arithmetic<T>::value, RT>::type;

template<typename T, typename RT>
using allow_enums =
    typename std::enable_if<std::is_enum<T>::value, RT>::type;

template<typename EnumType>
struct EnumSerializer {
    static std::string serialize(EnumType value)
    {
        return std::to_string(static_cast<int>(value));
    }
};

template<typename JsonValue, typename Allocator>
void serialize(JsonValue& output,
               const std::string& value,
               Allocator& allocator)
{
    output.SetString(value.c_str(), allocator);
}

template<typename JsonValue, typename T, typename Allocator>
allow_numericals<T, void>
serialize(JsonValue& output, const T& value, Allocator& allocator)
{
    output = value;
}

template<typename JsonValue, typename T, typename Allocator>
allow_enums<T, void>
serialize(JsonValue& output, T value, Allocator& allocator)
{
    serialize(output, EnumSerializer<T>::serialize(value), allocator);
}

template<typename JsonValue, typename T, typename Allocator>
void serialize(JsonValue& output,
               const std::vector<T>& values,
               Allocator& allocator)
{
    output.SetArray();
    output.Reserve(values.size(), allocator);

    for(const auto& value: values)
    {
        json::Value jValue;
        serialize(jValue, value, allocator);

        output.PushBack(jValue.Move(), allocator);
    }
}

template<typename JsonValue, typename T, typename Allocator>
void serialize(JsonValue& output,
               const std::set<T>& values,
               Allocator& allocator)
{
    output.SetArray();
    output.Reserve(values.size(), allocator);

    for(const auto& value: values)
    {
        json::Value jValue;
        serialize(jValue, value, allocator);

        output.PushBack(jValue.Move(), allocator);
    }
}

template<typename JsonValue, typename T, typename Allocator>
void map(JsonValue& output,
         const std::string& key,
         const T& value,
         Allocator& allocator)
{
    UNSAFE_ASSERT(output.IsObject());

    json::Value jValue;
    serialize(jValue, value, allocator);

    output.AddMember(
        json::Value(key.c_str(), allocator),
        jValue,
        allocator);
}

template<typename Stream>
Stream& pretty_print(Stream& os, const json::Document& document)
{
    json::StringBuffer buffer;
    json::PrettyWriter<json::StringBuffer> writer(buffer);
    document.Accept(writer);

    os << buffer.GetString();
    return os;
}

inline
std::ofstream& pretty_print(std::ofstream& os, const json::Document& document)
{
    json::OStreamWrapper osw(os);
    json::PrettyWriter<json::OStreamWrapper> writer(osw);
    document.Accept(writer);

    return os;
}

}
}

#endif