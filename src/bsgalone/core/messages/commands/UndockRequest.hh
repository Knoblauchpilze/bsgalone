
#pragma once

#include "IMessage.hh"
#include "Uuid.hh"
#include <optional>

namespace bsgalone::core {

class UndockRequest : public IMessage
{
  public:
  explicit UndockRequest(const Uuid playerDbId);
  ~UndockRequest() override = default;

  auto getPlayerDbId() const -> Uuid;

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
  Uuid m_playerDbId{};

  UndockRequest();

  friend auto operator<<(std::ostream &out, const UndockRequest &message) -> std::ostream &;
};

} // namespace bsgalone::core
