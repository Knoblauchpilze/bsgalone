
#pragma once

#include "AbstractMessage.hh"
#include "Uuid.hh"

namespace bsgalone::core {

class NetworkMessage : public AbstractMessage
{
  public:
  NetworkMessage(const MessageType &type);
  ~NetworkMessage() override = default;

  // TODO: This should be changed to a net::ClientId
  void setClientId(const bsgo::Uuid clientId);

  auto getClientId() const -> bsgo::Uuid;
  auto tryGetClientId() const -> std::optional<bsgo::Uuid>;

  void copyClientIdIfDefined(const NetworkMessage &source);

  protected:
  std::optional<bsgo::Uuid> m_clientId{};
};

} // namespace bsgalone::core
