
#pragma once

#include "LoadingTransition.hh"
#include "NetworkMessage.hh"
#include "Uuid.hh"

namespace bsgo {

class LoadingFinishedMessage : public bsgalone::core::NetworkMessage
{
  public:
  LoadingFinishedMessage();
  LoadingFinishedMessage(const LoadingTransition transition, const Uuid playerDbId);
  ~LoadingFinishedMessage() override = default;

  auto getTransition() const -> LoadingTransition;
  auto tryGetSystemDbId() const -> std::optional<Uuid>;
  auto getPlayerDbId() const -> Uuid;

  void setSystemDbId(const Uuid systemDbId);

  auto serialize(std::ostream &out) const -> std::ostream & override;
  bool deserialize(std::istream &in) override;

  auto clone() const -> bsgalone::core::IMessagePtr override;

  private:
  LoadingTransition m_transition{};
  std::optional<Uuid> m_systemDbId{};
  std::optional<Uuid> m_playerDbId{};
};

} // namespace bsgo
