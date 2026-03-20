
#pragma once

#include "IMessage.hh"
#include "Uuid.hh"

namespace bsgalone::core {

/// @brief - Describes a message sent by the server to a specific client. The
/// routing is done by providing the player identifier. This type of message
/// and its subclasses is only be sent by the server.
class AbstractPlayerMessage : public IMessage
{
  public:
  ~AbstractPlayerMessage() override = default;

  auto getPlayerDbId() const -> Uuid;

  protected:
  /// @brief - Creates a new message with the specified type. This constructor
  /// is protected to only allow subclasses to use it.
  /// @param type - the type of the message to create.
  /// @param playerDbId - the identifier of the player to whom the message should
  /// be delivered.
  AbstractPlayerMessage(const MessageType &type, const Uuid playerDbId);

  Uuid m_playerDbId{};
};

} // namespace bsgalone::core
