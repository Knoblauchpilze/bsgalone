
#pragma once

#include "SerializationUtils.hh"
#include "TimeUtils.hh"

namespace core {

template<typename T, std::enable_if_t<std::is_enum<T>::value, bool>>
inline auto serialize(std::ostream &out, const T &e) -> std::ostream &
{
  const auto eAsChar = reinterpret_cast<const char *>(&e);
  const auto size    = sizeof(typename std::underlying_type<T>::type);
  out.write(eAsChar, size);

  return out;
}

template<typename T, std::enable_if_t<!std::is_enum<T>::value, bool>>
inline auto serialize(std::ostream &out, const T &value) -> std::ostream &
{
  const auto valueAsChar = reinterpret_cast<const char *>(&value);
  const auto size        = sizeof(T);
  out.write(valueAsChar, size);

  return out;
}

template<typename T, std::enable_if_t<std::is_enum<T>::value, bool>>
inline bool deserialize(std::istream &in, T &e)
{
  const auto eAsChar = reinterpret_cast<char *>(&e);
  const auto size    = sizeof(typename std::underlying_type<T>::type);
  in.read(eAsChar, size);

  return in.good();
}

template<typename T, std::enable_if_t<!std::is_enum<T>::value, bool>>
inline bool deserialize(std::istream &in, T &value)
{
  const auto valueAsChar = reinterpret_cast<char *>(&value);
  const auto size        = sizeof(T);
  in.read(valueAsChar, size);

  return in.good();
}

inline auto serialize(std::ostream &out, const std::string &str) -> std::ostream &
{
  const std::size_t size{str.size()};
  out.write(reinterpret_cast<const char *>(&size), sizeof(std::size_t));
  if (!str.empty())
  {
    out << str;
  }

  return out;
}

inline bool deserialize(std::istream &in, std::string &str)
{
  std::size_t size{};
  in.read(reinterpret_cast<char *>(&size), sizeof(std::size_t));
  str.resize(size);
  in.read(str.data(), size);

  return in.good();
}

template<typename T, std::enable_if_t<std::is_enum<T>::value, bool>>
inline auto serialize(std::ostream &out, const std::optional<T> &value) -> std::ostream &
{
  const auto hasValue = value.has_value();
  out.write(reinterpret_cast<const char *>(&hasValue), sizeof(bool));
  if (hasValue)
  {
    serialize(out, *value);
  }

  return out;
}

template<typename T, std::enable_if_t<!std::is_enum<T>::value, bool>>
inline auto serialize(std::ostream &out, const std::optional<T> &value) -> std::ostream &
{
  const auto hasValue = value.has_value();
  out.write(reinterpret_cast<const char *>(&hasValue), sizeof(bool));
  if (hasValue)
  {
    serialize(out, *value);
  }

  return out;
}

template<typename T, std::enable_if_t<std::is_enum<T>::value, bool>>
inline bool deserialize(std::istream &in, std::optional<T> &value)
{
  bool hasValue{false};
  in.read(reinterpret_cast<char *>(&hasValue), sizeof(bool));
  if (hasValue)
  {
    T raw{};
    deserialize(in, raw);
    value = raw;
  }
  else
  {
    value.reset();
  }

  return in.good();
}

template<typename T, std::enable_if_t<!std::is_enum<T>::value, bool>>
inline bool deserialize(std::istream &in, std::optional<T> &value)
{
  bool hasValue{false};
  in.read(reinterpret_cast<char *>(&hasValue), sizeof(bool));
  if (hasValue)
  {
    T raw{};
    deserialize(in, raw);
    value = raw;
  }
  else
  {
    value.reset();
  }

  return in.good();
}

inline auto serialize(std::ostream &out, const Duration &d) -> std::ostream &
{
  const auto ticks = d.count();
  out.write(reinterpret_cast<const char *>(&ticks), sizeof(ticks));
  return out;
}

inline bool deserialize(std::istream &in, Duration &d)
{
  std::chrono::system_clock::rep ticks;

  d = core::toMilliseconds(0);
  in.read(reinterpret_cast<char *>(&ticks), sizeof(ticks));
  if (in)
  {
    d = core::Duration(ticks);
  }

  return in.good();
}

inline auto serialize(std::ostream &out, const Eigen::Vector3f &v) -> std::ostream &
{
  const float values[3] = {v(0), v(1), v(2)};
  out.write(reinterpret_cast<const char *>(values), 3 * sizeof(float));

  return out;
}

inline bool deserialize(std::istream &in, Eigen::Vector3f &v)
{
  float values[3] = {0.0f, 0.0f, 0.0f};

  in.read(reinterpret_cast<char *>(values), 3 * sizeof(float));
  if (in)
  {
    v = Eigen::Vector3f(values[0], values[1], values[2]);
  }

  return in.good();
}

} // namespace core
