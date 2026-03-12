
#pragma once

#include "LoadingTransition.hh"
#include "NetworkMessage.hh"
#include "Uuid.hh"

namespace bsgalone::core {

class LoadingFinishedMessage : public NetworkMessage
{
  public:
  LoadingFinishedMessage();
  LoadingFinishedMessage(const LoadingTransition transition, const Uuid playerDbId);
  ~LoadingFinishedMessage() override = default;

  auto getTransition() const -> LoadingTransition;
  auto tryGetSystemDbId() const -> std::optional<Uuid>;
  auto getPlayerDbId() const -> Uuid;

  void setSystemDbId(const Uuid systemDbId);

  auto clone() const -> IMessagePtr override;

  private:
  LoadingTransition m_transition{};
  std::optional<Uuid> m_systemDbId{};
  std::optional<Uuid> m_playerDbId{};

  friend auto operator<<(std::ostream &out, const LoadingFinishedMessage &message)
    -> std::ostream &;
  friend auto operator>>(std::istream &in, LoadingFinishedMessage &message) -> std::istream &;
};

auto operator<<(std::ostream &out, const LoadingFinishedMessage &message) -> std::ostream &;
auto operator>>(std::istream &in, LoadingFinishedMessage &message) -> std::istream &;

} // namespace bsgalone::core
