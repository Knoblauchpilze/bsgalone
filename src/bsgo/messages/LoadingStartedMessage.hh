
#pragma once

#include "NetworkMessage.hh"
#include "Uuid.hh"

namespace bsgo {

class LoadingStartedMessage : public NetworkMessage
{
  public:
  LoadingStartedMessage();
  LoadingStartedMessage(const Uuid systemDbId);
  LoadingStartedMessage(const Uuid systemDbId, const Uuid playerDbId);
  ~LoadingStartedMessage() override = default;

  auto getSystemDbId() const -> Uuid;
  auto tryGetPlayerDbId() const -> std::optional<Uuid>;

  auto serialize(std::ostream &out) const -> std::ostream & override;
  bool deserialize(std::istream &in) override;

  auto clone() const -> IMessagePtr override;

  private:
  Uuid m_systemDbId{};
  std::optional<Uuid> m_playerDbId{};
};

} // namespace bsgo
