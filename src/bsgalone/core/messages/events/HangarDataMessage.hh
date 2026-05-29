
#pragma once

#include "IMessage.hh"
#include "Uuid.hh"

namespace bsgalone::core {

class HangarDataMessage : public IMessage
{
  public:
  HangarDataMessage(const Uuid playerDbId);
  ~HangarDataMessage() override = default;

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

  HangarDataMessage();

  friend auto operator<<(std::ostream &out, const HangarDataMessage &message) -> std::ostream &;
};

auto operator<<(std::ostream &out, const HangarDataMessage &message) -> std::ostream &;

} // namespace bsgalone::core
