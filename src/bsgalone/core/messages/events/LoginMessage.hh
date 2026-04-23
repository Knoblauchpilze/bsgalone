
#pragma once

#include "ClientId.hh"
#include "Faction.hh"
#include "GameRole.hh"
#include "IMessage.hh"
#include "Uuid.hh"
#include <optional>

namespace bsgalone::core {

class LoginMessage : public IMessage
{
  public:
  LoginMessage();
  ~LoginMessage() override = default;

  bool successfullyLoggedIn() const;
  auto getPlayerDbId() const -> Uuid;
  auto getFaction() const -> Faction;
  auto getRole() const -> GameRole;

  void setPlayerDbId(const Uuid playerDbId);
  void setFaction(const Faction faction);
  void setRole(const GameRole role);

  auto clone() const -> IMessagePtr override;

  /// @brief - Tries to read a message from the input stream. If it
  /// succeeds, a valid message will be returned, otherwise an empty
  /// optional.
  /// The stream will **not be reset** in case the reading fails.
  /// @param in - the stream to read from
  /// @return - the deserialized message or an empty optional if reading
  /// the message's data fails
  static auto readFromStream(std::istream &in) -> std::optional<IMessagePtr>;

  private:
  std::optional<Uuid> m_playerDbId{};
  std::optional<Faction> m_faction{};
  std::optional<GameRole> m_role{};

  friend auto operator<<(std::ostream &out, const LoginMessage &message) -> std::ostream &;
};

auto operator<<(std::ostream &out, const LoginMessage &message) -> std::ostream &;

} // namespace bsgalone::core
