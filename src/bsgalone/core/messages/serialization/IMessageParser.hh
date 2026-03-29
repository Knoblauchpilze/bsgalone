
#pragma once

#include "IMessage.hh"
#include <deque>
#include <memory>
#include <optional>

namespace bsgalone::core {

class IMessageParser
{
  public:
  IMessageParser()          = default;
  virtual ~IMessageParser() = default;

  struct ParsingResult
  {
    std::optional<IMessagePtr> message{};
    int bytesProcessed{0};
  };
  virtual auto tryParseMessage(const std::deque<char> &data) -> ParsingResult = 0;
};

using IMessageParserPtr = std::unique_ptr<IMessageParser>;

} // namespace bsgalone::core
