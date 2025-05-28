
#pragma once

#include "CoreObject.hh"
#include "IMessage.hh"
#include <deque>
#include <istream>
#include <optional>

namespace bsgo {

class MessageParser : public core::CoreObject
{
  public:
  MessageParser();
  ~MessageParser() override = default;

  struct ParsingResult
  {
    std::optional<IMessagePtr> message{};
    int bytesProcessed{0};
  };
  auto tryParseMessage(const std::deque<char> &data) -> ParsingResult;
  auto tryReadMessage(const MessageType &type, std::istream &in) -> std::optional<IMessagePtr>;
};

} // namespace bsgo
