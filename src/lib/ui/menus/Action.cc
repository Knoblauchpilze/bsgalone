
#include "Action.hh"

namespace pge {

Action::Action(const action::Process &cb, const std::string &name)
  : utils::CoreObject(name)
  ,

  m_callback(cb)
{
  setService("action");
}

Action::~Action() {}

void Action::apply(Game &g) const
{
  m_callback(g);
}

void Action::setProcess(const action::Process &cb)
{
  m_callback = cb;
}

} // namespace pge
