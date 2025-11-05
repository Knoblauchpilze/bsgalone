
#pragma once

#include "NetworkMessage.hh"
#include "Uuid.hh"

namespace bsgo {

class AiBehaviorSyncMessage : public NetworkMessage
{
  public:
  AiBehaviorSyncMessage();
  AiBehaviorSyncMessage(const Uuid shipDbId);
  ~AiBehaviorSyncMessage() override = default;

  auto getShipDbId() const -> Uuid;
  auto tryGetSystemDbId() const -> std::optional<Uuid>;
  auto tryGetTargetIndex() const -> std::optional<int>;

  void setSystemDbId(const Uuid systemDbId);
  void setTargetIndex(const int targetIndex);

  auto serialize(std::ostream &out) const -> std::ostream & override;
  bool deserialize(std::istream &in) override;

  auto clone() const -> IMessagePtr override;

  private:
  Uuid m_shipDbId{};
  std::optional<Uuid> m_systemDbId{};
  std::optional<int> m_targetIndex{};
};

} // namespace bsgo
