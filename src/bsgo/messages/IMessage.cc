
#include "IMessage.hh"

namespace bsgo {

IMessage::IMessage(const std::string &name)
  : utils::CoreObject(name)
{
  setService("message");
}

auto operator<<(std::ostream &out, const IMessage &message) -> std::ostream &
{
  message.serialize(out);
  return out;
}

auto operator>>(std::istream &in, IMessage &message) -> std::istream &
{
  message.deserialize(in);
  return in;
}

} // namespace bsgo
