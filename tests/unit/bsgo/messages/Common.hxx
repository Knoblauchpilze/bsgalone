
#pragma once

#include "Common.hh"
#include "IMessage.hh"
#include <core_utils/SerializationUtils.hh>
#include <sstream>

namespace bsgo {

template<typename T>
inline auto serializeAndDeserialize(const T &value, T &output)
{
  std::ostringstream out{};
  utils::serialize(out, value);
  std::istringstream in(out.str());

  utils::deserialize(in, output);
}

inline auto serializeAndDeserializeMessage(const IMessage &value, IMessage &output)
{
  std::ostringstream out{};
  out << value;
  std::istringstream in(out.str());
  in >> output;
}

} // namespace bsgo
