
#pragma once

#include "ClientId.hh"
#include "Faction.hh"
#include "IMessage.hh"
#include <optional>
#include <string>

namespace bsgalone::core {

class SignupRequest : public IMessage
{
  public:
  SignupRequest(const std::string &username, const std::string &password, const Faction faction);
  ~SignupRequest() override = default;

  auto getUsername() const -> std::string;
  auto getPassword() const -> std::string;
  auto getFaction() const -> Faction;
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
  Faction m_faction{};

  std::optional<net::ClientId> m_clientId{};

  SignupRequest();

  friend auto operator<<(std::ostream &out, const SignupRequest &message) -> std::ostream &;
};

} // namespace bsgalone::core
