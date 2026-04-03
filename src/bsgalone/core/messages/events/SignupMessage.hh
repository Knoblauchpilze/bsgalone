
#pragma once

#include "ClientId.hh"
#include "GameRole.hh"
#include "IMessage.hh"
#include "Uuid.hh"
#include <optional>

namespace bsgalone::core {

class SignupMessage : public IMessage
{
  public:
  SignupMessage(const bool success);
  ~SignupMessage() override = default;

  bool successfullySignedUp() const;

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
  bool m_success{false};

  SignupMessage();

  friend auto operator<<(std::ostream &out, const SignupMessage &message) -> std::ostream &;
};

auto operator<<(std::ostream &out, const SignupMessage &message) -> std::ostream &;

} // namespace bsgalone::core
