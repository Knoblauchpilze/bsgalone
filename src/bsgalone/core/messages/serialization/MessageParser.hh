
#pragma once

#include "CoreObject.hh"
#include "IMessage.hh"
#include "IMessageParser.hh"
#include <deque>
#include <istream>
#include <optional>

namespace bsgalone::core {

class MessageParser : public IMessageParser, public ::core::CoreObject
{
  public:
  MessageParser();
  ~MessageParser() override = default;

  auto tryParseMessage(const std::deque<char> &data) -> ParsingResult override;

  private:
  auto tryReadMessage(const MessageType &type, std::istream &in) -> std::optional<IMessagePtr>;
};

} // namespace bsgalone::core
