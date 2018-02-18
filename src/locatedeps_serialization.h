#ifndef LOCATE_DEPS_SERIALIZATION
#define LOCATE_DEPS_SERIALIZATION

#include <locatedeps_assert.h>

#include <rapidjson/document.h>
#include <rapidjson/stringbuffer.h>
#include <rapidjson/writer.h>
#include "rapidjson/prettywriter.h"

#include <string>
#include <vector>
#include <set>
#include <type_traits>


namespace json = rapidjson;

namespace clang {
namespace locate_deps {

template<typename T>
using AllowArithmeticTypes =
    typename std::enable_if<std::is_arithmetic<T>::value, T>::type;

template<typename T>
using AllowEnumTypes =
    typename std::enable_if<std::is_enum<T>::value, T>::type;

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

template<typename JsonValue, typename T, typename Allocator,
         typename Policy=AllowArithmeticTypes<T>>
void serialize(JsonValue& output, const T& value, Allocator& allocator)
{
    output = value;
}

template<typename JsonValue, typename T, typename Allocator,
         typename Policy=AllowEnumTypes<T>>
void serialize(JsonValue& output, T value, Allocator& allocator)
{
    serialize(output, EnumSerializer<T>::serialize(value), allocator);
}

template<typename JsonValue, typename T, typename Allocator>
void serialize(JsonValue& output,
               const std::vector<T>& values,
               Allocator& allocator)
{
    output.SetArray();
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
Stream& print(Stream& os, const json::Document& document)
{
    json::StringBuffer buffer;
    json::Writer<json::StringBuffer> writer(buffer);
    document.Accept(writer);

    os << buffer.GetString();
    return os;
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

}
}

#endif