
#pragma once

#include "Common.hh"
#include "IMessage.hh"
#include "SerializationUtils.hh"
#include <sstream>

namespace bsgo {

template<typename T>
inline auto serializeAndDeserialize(const T &value, T &output)
{
  std::ostringstream out{};
  core::serialize(out, value);
  std::istringstream in(out.str());

  core::deserialize(in, output);
}

inline auto serializeAndDeserializeMessage(const IMessage &value, IMessage &output)
{
  std::ostringstream out{};
  out << value;
  std::istringstream in(out.str());
  in >> output;
}

} // namespace bsgo
