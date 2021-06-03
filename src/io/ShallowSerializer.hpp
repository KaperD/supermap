#pragma once

#include "SerializeHelper.hpp"

namespace supermap::io {

template <typename T>
struct ShallowSerializer : Serializable<true> {
    static void serialize(const T &value, std::ostream &os) {
        auto posBefore = os.tellp();
        os.write(reinterpret_cast<const char *>(&value), sizeof(T));
        if (os.tellp() - posBefore != sizeof(T)) {
            throw IOException(
                "Attempted to write " + std::to_string(sizeof(T)) +
                    " bytes, but only " + std::to_string(os.tellp() - posBefore) +
                    " succeeded"
            );
        }
    }
};

template <typename T, typename = std::enable_if_t<std::is_default_constructible<T>::value>>
struct ShallowDeserializer : Deserializable<true> {
    static T deserialize(std::istream &is) {
        T obj;
        is.read(reinterpret_cast<char *>(&obj), sizeof(T));
        return obj;
    }
};

template <typename T>
struct ShallowDeserializedSize : FixedDeserializedSize<sizeof(T)> {};

template <typename T>
struct SerializeHelper<T, std::enable_if_t<std::is_integral_v<T>>> : ShallowSerializer<T> {};

template <typename T>
struct DeserializeHelper<T, std::enable_if_t<std::is_integral_v<T>>> : ShallowDeserializer<T> {};

template <typename T>
struct FixedDeserializedSizeRegister<T, std::enable_if_t<std::is_integral_v<T>>> : ShallowDeserializedSize<T> {};

} // supermap::io
