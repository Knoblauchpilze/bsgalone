
#pragma once

#include "NetworkMessage.hh"
#include "Uuid.hh"

namespace bsgo {

class LoadingFinishedMessage : public NetworkMessage
{
  public:
  LoadingFinishedMessage();
  LoadingFinishedMessage(const Uuid systemDbId);
  LoadingFinishedMessage(const Uuid systemDbId, const Uuid playerDbId);
  ~LoadingFinishedMessage() override = default;

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
