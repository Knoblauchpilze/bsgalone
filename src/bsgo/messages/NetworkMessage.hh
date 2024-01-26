
#pragma once

#include "AbstractMessage.hh"
#include "Uuid.hh"

namespace bsgo {

class NetworkMessage : public AbstractMessage
{
  public:
  NetworkMessage(const MessageType &type);
  ~NetworkMessage() override = default;

  void setClientId(const Uuid clientId);

  auto getClientId() const -> Uuid;
  auto tryGetClientId() const -> std::optional<Uuid>;

  void copyClientIdIfDefined(const NetworkMessage &source);

  protected:
  std::optional<Uuid> m_clientId{};
};

} // namespace bsgo
