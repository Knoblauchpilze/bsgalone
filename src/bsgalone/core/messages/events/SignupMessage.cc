
#include "SignupMessage.hh"
#include "SerializationUtils.hh"

namespace bsgalone::core {

SignupMessage::SignupMessage()
  : IMessage(MessageType::SIGNUP)
{}

SignupMessage::SignupMessage(const bool success)
  : IMessage(MessageType::SIGNUP)
  , m_success(success)
{}

bool SignupMessage::successfullySignedUp() const
{
  return m_success;
}

auto SignupMessage::clone() const -> IMessagePtr
{
  return std::make_unique<SignupMessage>(m_success);
}

auto SignupMessage::readFromStream(std::istream &in) -> std::optional<IMessagePtr>
{
  SignupMessage message;

  bool ok{true};
  ok &= ::core::deserialize(in, message.m_type);
  ok &= ::core::deserialize(in, message.m_success);

  if (!ok)
  {
    return {};
  }

  return message.clone();
}

auto operator<<(std::ostream &out, const SignupMessage &message) -> std::ostream &
{
  ::core::serialize(out, message.m_type);
  ::core::serialize(out, message.m_success);

  return out;
}

} // namespace bsgalone::core
