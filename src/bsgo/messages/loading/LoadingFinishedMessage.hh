
#pragma once

#include "LoadingTransition.hh"
#include "NetworkMessage.hh"
#include "Uuid.hh"

namespace bsgo {

class LoadingFinishedMessage : public NetworkMessage
{
  public:
  LoadingFinishedMessage();
  LoadingFinishedMessage(const LoadingTransition transition, const Uuid systemDbId);
  LoadingFinishedMessage(const LoadingTransition transition,
                         const Uuid systemDbId,
                         const Uuid playerDbId);
  ~LoadingFinishedMessage() override = default;

  auto getTransition() const -> LoadingTransition;
  auto getSystemDbId() const -> Uuid;
  auto tryGetPlayerDbId() const -> std::optional<Uuid>;

  auto serialize(std::ostream &out) const -> std::ostream & override;
  bool deserialize(std::istream &in) override;

  auto clone() const -> IMessagePtr override;

  private:
  LoadingTransition m_transition{};
  Uuid m_systemDbId{};
  std::optional<Uuid> m_playerDbId{};
};

} // namespace bsgo
