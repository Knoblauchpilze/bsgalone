
#pragma once

#include "ClientId.hh"
#include "GameRole.hh"
#include "IMessage.hh"
#include "Uuid.hh"
#include <optional>

namespace bsgalone::core {

class LoginMessage : public IMessage
{
  public:
  LoginMessage(const net::ClientId clientId);
  ~LoginMessage() override = default;

  auto getClientId() const -> net::ClientId;
  bool successfullyLoggedIn() const;
  auto getPlayerDbId() const -> Uuid;
  auto getRole() const -> GameRole;
  auto getSystemDbId() const -> Uuid;

  void setPlayerDbId(const Uuid playerDbId);
  void setRole(const GameRole role);
  void setSystemDbId(const Uuid systemDbId);

  auto clone() const -> IMessagePtr override;

  /// @brief - Tries to read a message from the input stream. If it
  /// succeeds, a valid dock message will be returned, otherwise an
  /// empty optional.
  /// The stream will **not be reset** in case the reading fails.
  /// @param in - the stream to read from
  /// @return - the deserialized message or an empty optional if reading
  /// the message's data fails
  static auto readFromStream(std::istream &in) -> std::optional<IMessagePtr>;

  private:
  net::ClientId m_clientId{};
  std::optional<Uuid> m_playerDbId{};
  std::optional<GameRole> m_role{};
  std::optional<Uuid> m_systemDbId{};

  LoginMessage();

  friend auto operator<<(std::ostream &out, const LoginMessage &message) -> std::ostream &;
};

auto operator<<(std::ostream &out, const LoginMessage &message) -> std::ostream &;

} // namespace bsgalone::core
