
#pragma once

#include "ClientId.hh"
#include "GameRole.hh"
#include "IMessage.hh"
#include "Uuid.hh"
#include <optional>

namespace bsgalone::core {

class LogoutMessage : public IMessage
{
  public:
  LogoutMessage();
  ~LogoutMessage() override = default;

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
  friend auto operator<<(std::ostream &out, const LogoutMessage &message) -> std::ostream &;
};

auto operator<<(std::ostream &out, const LogoutMessage &message) -> std::ostream &;

} // namespace bsgalone::core
