
#pragma once

#include "TimeUtils.hh"
#include <eigen3/Eigen/Eigen>
#include <istream>
#include <optional>
#include <ostream>
#include <type_traits>

namespace core {

auto serialize(std::ostream &out, const std::string &str) -> std::ostream &;
bool deserialize(std::istream &in, std::string &str);

auto serialize(std::ostream &out, const Eigen::Vector3f &v) -> std::ostream &;
bool deserialize(std::istream &in, Eigen::Vector3f &v);

auto serialize(std::ostream &out, const core::Duration &d) -> std::ostream &;
bool deserialize(std::istream &in, core::Duration &d);

template<typename T>
auto serialize(std::ostream &out, const std::optional<T> &value) -> std::ostream &;
template<typename T>
bool deserialize(std::istream &in, std::optional<T> &value);

template<typename Key, typename Value>
auto serialize(std::ostream &out, const std::unordered_map<Key, int> &m) -> std::ostream &;
template<typename Key, typename Value>
bool deserialize(std::istream &in, std::unordered_map<Key, int> &m);

/// https://cplusplus.com/forum/general/285613/
/// https://en.cppreference.com/w/cpp/language/constraints.html
template<typename T>
concept serializable = requires(T t, std::ostream &out)
{
  {
    t.serialize(out)
    } -> std::convertible_to<std::ostream &>;
};

template<typename T>
concept deserializable = requires(T t, std::istream &in)
{
  {
    t.deserialize(in)
    } -> std::convertible_to<bool>;
};

template<serializable T>
auto serialize(std::ostream &out, const T &e) -> std::ostream &;

template<deserializable T>
bool deserialize(std::istream &in, T &e);

template<typename T, std::enable_if_t<std::is_arithmetic<T>::value, bool> = true>
auto serialize(std::ostream &out, const T &e) -> std::ostream &;
template<typename T, std::enable_if_t<std::is_arithmetic<T>::value, bool> = true>
bool deserialize(std::istream &in, T &e);

template<typename T, std::enable_if_t<std::is_enum<T>::value, bool> = true>
auto serialize(std::ostream &out, const T &e) -> std::ostream &;
template<typename T, std::enable_if_t<std::is_enum<T>::value, bool> = true>
bool deserialize(std::istream &in, T &e);

} // namespace core

#include "SerializationUtils.hxx"
