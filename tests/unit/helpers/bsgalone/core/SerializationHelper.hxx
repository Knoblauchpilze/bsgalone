
#pragma once

#include "SerializationHelper.hh"
#include <sstream>

namespace test {

template<typename T>
inline auto serializeAndDeserializeMessage(const T &value) -> bsgalone::core::IMessagePtr
{
  std::ostringstream out{};
  out << value;

  std::istringstream in(out.str());
  auto maybeOut = T::readFromStream(in);
  if (!maybeOut.has_value())
  {
    throw std::runtime_error("Failed to deserialize message " + str(value.type()));
  }

  return std::move(*maybeOut);
}

} // namespace test
