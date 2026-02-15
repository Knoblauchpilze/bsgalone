
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
  auto tryReadMessage(const bsgalone::core::MessageType &type, std::istream &in)
    -> std::optional<bsgalone::core::IMessagePtr>;
};

} // namespace bsgo
