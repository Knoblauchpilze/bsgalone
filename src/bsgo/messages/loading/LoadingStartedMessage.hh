
#pragma once

#include "LoadingTransition.hh"
#include "NetworkMessage.hh"
#include "Uuid.hh"

namespace bsgo {

class LoadingStartedMessage : public NetworkMessage
{
  public:
  LoadingStartedMessage();
  LoadingStartedMessage(const LoadingTransition transition);
  ~LoadingStartedMessage() override = default;

  auto getTransition() const -> LoadingTransition;
  auto getSystemDbId() const -> Uuid;
  auto tryGetSystemDbId() const -> std::optional<Uuid>;
  auto tryGetPlayerDbId() const -> std::optional<Uuid>;

  void setSystemDbId(const Uuid systemDbId);
  void setPlayerDbId(const Uuid playerDbId);

  auto serialize(std::ostream &out) const -> std::ostream & override;
  bool deserialize(std::istream &in) override;

  auto clone() const -> IMessagePtr override;

  private:
  LoadingTransition m_transition{};
  std::optional<Uuid> m_systemDbId{};
  std::optional<Uuid> m_playerDbId{};
};

} // namespace bsgo
