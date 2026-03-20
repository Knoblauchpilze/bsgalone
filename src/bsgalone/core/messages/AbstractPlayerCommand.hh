
#pragma once

#include "IMessage.hh"
#include "Uuid.hh"

namespace bsgalone::core {

/// @brief - Describes a command sent by the client applications to perform an
/// action on the server. Such a command can only be sent by the server and is
/// interpreted by the server. The server typically sends back a response which
/// will be derived from the `AbstractPlayerMessage` class.
class AbstractPlayerCommand : public IMessage
{
  public:
  ~AbstractPlayerCommand() override = default;

  auto getPlayerDbId() const -> Uuid;
  auto getSystemDbId() const -> Uuid;

  protected:
  AbstractPlayerCommand(const MessageType &type, const Uuid playerDbId, const Uuid systemDbId);

  Uuid m_playerDbId{};
  Uuid m_systemDbId{};
};

} // namespace bsgalone::core
