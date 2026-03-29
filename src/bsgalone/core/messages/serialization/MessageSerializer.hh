
#pragma once

#include "IMessage.hh"
#include "IMessageSerializer.hh"
#include <vector>

namespace bsgalone::core {

class MessageSerializer : public IMessageSerializer
{
  public:
  MessageSerializer()           = default;
  ~MessageSerializer() override = default;

  auto serializeMessage(const IMessage &message) -> std::vector<char> override;
};

} // namespace bsgalone::core
