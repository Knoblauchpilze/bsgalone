
#pragma once

#include "CoreObject.hh"
#include "IMessage.hh"
#include "IMessageParser.hh"
#include <deque>
#include <istream>
#include <optional>

namespace bsgo {

class MessageParser : public bsgalone::core::IMessageParser, public core::CoreObject
{
  public:
  MessageParser();
  ~MessageParser() override = default;

  auto tryParseMessage(const std::deque<char> &data) -> ParsingResult override;

  private:
  auto tryReadMessage(const MessageType &type, std::istream &in) -> std::optional<IMessagePtr>;
};

} // namespace bsgo
