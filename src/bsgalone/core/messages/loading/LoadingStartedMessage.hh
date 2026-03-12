
#pragma once

#include "LoadingTransition.hh"
#include "NetworkMessage.hh"
#include "Uuid.hh"

namespace bsgalone::core {

class LoadingStartedMessage : public NetworkMessage
{
  public:
  LoadingStartedMessage();
  LoadingStartedMessage(const LoadingTransition transition, const Uuid playerDbId);
  ~LoadingStartedMessage() override = default;

  auto getTransition() const -> LoadingTransition;
  auto getSystemDbId() const -> Uuid;
  auto tryGetSystemDbId() const -> std::optional<Uuid>;
  auto getPlayerDbId() const -> Uuid;

  void setSystemDbId(const Uuid systemDbId);

  auto clone() const -> IMessagePtr override;

  private:
  LoadingTransition m_transition{};
  std::optional<Uuid> m_systemDbId{};
  std::optional<Uuid> m_playerDbId{};

  friend auto operator<<(std::ostream &out, const LoadingStartedMessage &message) -> std::ostream &;
  friend auto operator>>(std::istream &in, LoadingStartedMessage &message) -> std::istream &;
};

auto operator<<(std::ostream &out, const LoadingStartedMessage &message) -> std::ostream &;
auto operator>>(std::istream &in, LoadingStartedMessage &message) -> std::istream &;

} // namespace bsgalone::core
