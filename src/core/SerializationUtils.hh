
#pragma once

#include "TimeUtils.hh"
#include <eigen3/Eigen/Eigen>
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

template<typename Key, std::enable_if_t<std::is_enum<Key>::value, bool> = true>
auto serialize(std::ostream &out, const std::unordered_map<Key, int> &m) -> std::ostream &;

template<typename Key, std::enable_if_t<std::is_enum<Key>::value, bool> = true>
bool deserialize(std::istream &in, std::unordered_map<Key, int> &m);

template<typename Key, std::enable_if_t<!std::is_enum<Key>::value, bool> = true>
auto serialize(std::ostream &out, const std::unordered_map<Key, float> &m) -> std::ostream &;

template<typename Key, std::enable_if_t<!std::is_enum<Key>::value, bool> = true>
bool deserialize(std::istream &in, std::unordered_map<Key, float> &m);

auto serialize(std::ostream &out, const core::Duration &d) -> std::ostream &;

bool deserialize(std::istream &in, core::Duration &d);

auto serialize(std::ostream &out, const Eigen::Vector3f &v) -> std::ostream &;

bool deserialize(std::istream &in, Eigen::Vector3f &v);

} // namespace core

#include "SerializationUtils.hxx"
