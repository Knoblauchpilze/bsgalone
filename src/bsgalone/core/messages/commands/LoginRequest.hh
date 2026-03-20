
#pragma once

#include "ClientId.hh"
#include "GameRole.hh"
#include "IMessage.hh"
#include <string>

namespace bsgalone::core {

class LoginRequest : public IMessage
{
  public:
  LoginRequest(const std::string &username, const std::string &password, const GameRole role);
  ~LoginRequest() override = default;

  auto getUsername() const -> std::string;
  auto getPassword() const -> std::string;
  auto getRole() const -> GameRole;
  auto tryGetClientId() const -> std::optional<net::ClientId>;

  void setClientId(const net::ClientId clientId);

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
  std::string m_username{};
  std::string m_password{};
  GameRole m_role{};

  std::optional<net::ClientId> m_clientId{};

  LoginRequest();

  friend auto operator<<(std::ostream &out, const LoginRequest &message) -> std::ostream &;
};

auto operator<<(std::ostream &out, const LoginRequest &message) -> std::ostream &;

} // namespace bsgalone::core
