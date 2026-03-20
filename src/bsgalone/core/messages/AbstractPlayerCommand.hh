
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
  /// @brief - Creates a new command with the specified type. This constructor
  /// is protected to only allow subclasses to use it.
  /// @param type - the type of the command to create.
  /// @param playerDbId - the identifier of the player who initiated the command.
  /// @param systemDbId - the system in which the player is currently located. It
  /// allows the server to route the command to the correct processor.
  AbstractPlayerCommand(const MessageType &type, const Uuid playerDbId, const Uuid systemDbId);

  Uuid m_playerDbId{};
  Uuid m_systemDbId{};
};

} // namespace bsgalone::core
