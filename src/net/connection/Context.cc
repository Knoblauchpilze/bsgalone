
#include "Context.hh"

namespace net {

Context::Context()
  : utils::CoreObject("context")
{
  setService("net");
}

auto Context::createConnection() -> ConnectionPtr
{
  return std::make_unique<Connection>(m_context);
}

} // namespace net
