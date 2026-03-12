
#pragma once

#include "NetworkMessage.hh"
#include "Uuid.hh"

namespace bsgalone::core {

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

  auto clone() const -> IMessagePtr override;

  private:
  Uuid m_shipDbId{};
  std::optional<Uuid> m_systemDbId{};
  std::optional<int> m_targetIndex{};

  friend auto operator<<(std::ostream &out, const AiBehaviorSyncMessage &message) -> std::ostream &;
  friend auto operator>>(std::istream &in, AiBehaviorSyncMessage &message) -> std::istream &;
};

auto operator<<(std::ostream &out, const AiBehaviorSyncMessage &message) -> std::ostream &;
auto operator>>(std::istream &in, AiBehaviorSyncMessage &message) -> std::istream &;

} // namespace bsgalone::core
