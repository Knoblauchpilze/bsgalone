
#pragma once

#include "IMessage.hh"
#include <memory>
#include <vector>

namespace bsgalone::core {

class IMessageSerializer
{
  public:
  IMessageSerializer()          = default;
  virtual ~IMessageSerializer() = default;

  virtual auto serializeMessage(const IMessage &message) -> std::vector<char> = 0;
};

using IMessageSerializerPtr = std::unique_ptr<IMessageSerializer>;

} // namespace bsgalone::core
