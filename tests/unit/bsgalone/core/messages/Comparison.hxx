
#pragma once

#include "Comparison.hh"

namespace bsgalone::core {

template<typename T>
inline void serializeAndDeserializeMessage(const T &value, T &output)
{
  std::ostringstream out{};
  out << value;
  std::istringstream in(out.str());
  in >> output;
}

template<typename T>
inline auto serializeAndDeserializePlayerMessage(const T &value) -> IMessagePtr
{
  std::ostringstream out{};
  out << value;

  std::istringstream in(out.str());
  auto maybeOut = T::readFromStream(in);
  if (!maybeOut.has_value())
  {
    throw std::runtime_error("Failed to deserialize message " + str(value.type()));
  }

  // https://stackoverflow.com/questions/77144003/use-of-template-keyword-before-dependent-template-name
  return std::move(*maybeOut);
}

} // namespace bsgalone::core
