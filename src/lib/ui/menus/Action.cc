
#include "Action.hh"

namespace pge {

Action::Action(const GameCallback &callback)
  : utils::CoreObject("action")
  , m_callback(callback)
{
  setService("action");
}

void Action::apply(Game &g) const
{
  m_callback(g);
}

} // namespace pge
