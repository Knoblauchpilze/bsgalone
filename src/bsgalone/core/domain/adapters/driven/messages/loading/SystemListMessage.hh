
#pragma once

#include "AbstractPlayerMessage.hh"
#include "System.hh"

namespace bsgalone::core {

class SystemListMessage : public AbstractPlayerMessage
{
  public:
  SystemListMessage(const Uuid playerDbId, const std::vector<System> &systemsData);
  ~SystemListMessage() override = default;

  auto getSystemsData() const -> const std::vector<System> &;

  auto clone() const -> IMessagePtr override;

  /// @brief - Tries to read a system list message from the input stream.
  /// If it succeeds, a valid message will be returned, otherwise an empty
  /// optional.
  /// The stream will **not be reset** in case the reading fails.
  /// @param in - the stream to read from
  /// @return - the deserialized message or an empty optional if reading
  /// the message's data fails
  static auto readFromStream(std::istream &in) -> std::optional<IMessagePtr>;

  private:
  std::vector<System> m_systemsData{};

  SystemListMessage();

  friend auto operator<<(std::ostream &out, const SystemListMessage &message) -> std::ostream &;
};

auto operator<<(std::ostream &out, const SystemListMessage &message) -> std::ostream &;

} // namespace bsgalone::core
