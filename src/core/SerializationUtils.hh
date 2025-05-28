
#pragma once

#include <istream>
#include <optional>
#include <ostream>
#include <type_traits>

namespace core {

/// https://stackoverflow.com/questions/55647741/template-specialization-with-enable-if
/// https://en.cppreference.com/w/cpp/types/enable_if

template<typename T, std::enable_if_t<std::is_enum<T>::value, bool> = true>
auto serialize(std::ostream &out, const T &e) -> std::ostream &;

template<typename T, std::enable_if_t<!std::is_enum<T>::value, bool> = true>
auto serialize(std::ostream &out, const T &e) -> std::ostream &;

template<typename T, std::enable_if_t<std::is_enum<T>::value, bool> = true>
bool deserialize(std::istream &in, T &e);

template<typename T, std::enable_if_t<!std::is_enum<T>::value, bool> = true>
bool deserialize(std::istream &in, T &e);

auto serialize(std::ostream &out, const std::string &str) -> std::ostream &;

bool deserialize(std::istream &in, std::string &str);

template<typename T, std::enable_if_t<std::is_enum<T>::value, bool> = true>
auto serialize(std::ostream &out, const std::optional<T> &value) -> std::ostream &;

template<typename T, std::enable_if_t<!std::is_enum<T>::value, bool> = true>
auto serialize(std::ostream &out, const std::optional<T> &value) -> std::ostream &;

template<typename T, std::enable_if_t<std::is_enum<T>::value, bool> = true>
bool deserialize(std::istream &in, std::optional<T> &value);

template<typename T, std::enable_if_t<!std::is_enum<T>::value, bool> = true>
bool deserialize(std::istream &in, std::optional<T> &value);

} // namespace core

#include "SerializationUtils.hxx"
