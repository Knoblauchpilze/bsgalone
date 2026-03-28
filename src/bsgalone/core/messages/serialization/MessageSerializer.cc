
#include "MessageSerializer.hh"
#include "LoginRequest.hh"
#include "SignupRequest.hh"
#include <sstream>

namespace bsgalone::core {
namespace {
template<typename T>
auto serialize(std::ostream &out, const T &message) -> std::ostream &
{
  out << message;
  return out;
}
} // namespace

// TODO: Add tests for this class
auto MessageSerializer::serializeMessage(const IMessage &message) -> std::vector<char>
{
  std::ostringstream out{};

  switch (message.type())
  {
    case MessageType::LOGIN_REQUEST:
      serialize(out, message.as<LoginRequest>());
      break;
    case MessageType::SIGNUP_REQUEST:
      serialize(out, message.as<SignupRequest>());
      break;
    default:
      throw std::invalid_argument("Unsupported message type " + str(message.type()));
      break;
  }

  const auto &rawMessage = out.str();
  return std::vector<char>(rawMessage.begin(), rawMessage.end());
}

} // namespace bsgalone::core
