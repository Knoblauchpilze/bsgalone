
#include "IMessage.hh"

namespace bsgo {

IMessage::IMessage(const std::string &name)
  : utils::CoreObject(name)
{
  setService("message");
}

} // namespace bsgo
