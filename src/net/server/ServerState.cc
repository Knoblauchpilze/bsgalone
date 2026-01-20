
#include "ServerState.hh"

namespace net::details {

bool ServerState::isRegistered()
{
  const std::lock_guard guard(m_locker);
  return m_registered;
}

bool ServerState::isAccepting()
{
  const std::lock_guard guard(m_locker);
  return m_accepting;
}

bool ServerState::markAsRegistered()
{
  const std::lock_guard guard(m_locker);
  if (m_registered)
  {
    return false;
  }

  m_registered = true;
  m_accepting  = true;

  return true;
}

bool ServerState::markAsTerminating()
{
  const std::lock_guard guard(m_locker);
  if (!m_accepting)
  {
    return false;
  }

  m_accepting = false;

  return true;
}

} // namespace net::details
