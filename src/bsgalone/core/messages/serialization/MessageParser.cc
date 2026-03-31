
#include "MessageParser.hh"
#include "LoginMessage.hh"
#include "LoginRequest.hh"
#include "SerializationUtils.hh"
#include "SignupRequest.hh"
#include <sstream>

namespace bsgalone::core {

MessageParser::MessageParser()
  : ::core::CoreObject("message")
{
  addModule("parser");
  setService("server");
}

namespace {
constexpr auto MESSAGE_SIZE = sizeof(std::underlying_type_t<MessageType>);

auto tryReadMessageType(std::istream &in) -> std::optional<MessageType>
{
  MessageType type{};
  if (!::core::deserialize(in, type))
  {
    return {};
  }

  return type;
}
} // namespace

auto MessageParser::tryParseMessage(const std::deque<char> &data) -> ParsingResult
{
  if (data.size() < MESSAGE_SIZE)
  {
    return {};
  }

  std::string input(data.begin(), data.end());
  std::istringstream in(input);

  const auto maybeMessageType = tryReadMessageType(in);
  if (!maybeMessageType)
  {
    warn("Failed to read message type from " + std::to_string(data.size()) + " byte(s)");
    return {};
  }

  in.seekg(0, std::ios::beg);

  ParsingResult out{};
  out.message = tryReadMessage(*maybeMessageType, in);
  if (out.message)
  {
    out.bytesProcessed = static_cast<int>(in.tellg());
  }

  return out;
}

namespace {
template<typename MessageType>
auto readMessage(std::istream &in) -> std::optional<IMessagePtr>
{
  auto message = std::make_unique<MessageType>();
  in >> *message;

  if (!in.good())
  {
    return {};
  }

  return message;
}
} // namespace

auto MessageParser::tryReadMessage(const MessageType &type, std::istream &in)
  -> std::optional<IMessagePtr>
{
  switch (type)
  {
    case MessageType::LOGIN:
      return LoginMessage::readFromStream(in);
    case MessageType::LOGIN_REQUEST:
      return LoginRequest::readFromStream(in);
    case MessageType::SIGNUP_REQUEST:
      return SignupRequest::readFromStream(in);
    default:
      error("Unsupported message type " + str(type));
      break;
  }

  // Redundant because of the error call above.
  return {};
}

} // namespace bsgalone::core
