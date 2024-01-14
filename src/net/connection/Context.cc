
#include "Context.hh"
#include <core_utils/TimeUtils.hh>

namespace net {

Context::Context()
  : utils::CoreObject("context")
{
  setService("net");
}

auto Context::get() -> asio::io_context &
{
  return m_asioContext;
}

auto Context::createConnection(const std::string &url, const int port) -> ConnectionPtr
{
  return std::make_unique<Connection>(url, port, m_asioContext);
}

void Context::start()
{
  const std::lock_guard guard(m_asioContextLocker);
  if (m_running.load())
  {
    warn("The asio context is already started");
    return;
  }

  m_running.store(true);
  m_contextThread = std::thread([this]() { m_asioContext.run(); });

  debug("Successfully started asio context");
}

void Context::stop()
{
  const std::lock_guard guard(m_asioContextLocker);
  if (!m_running.load())
  {
    warn("The asio context was not started");
    return;
  }

  m_running.store(false);
  m_asioContext.stop();

  waitForThreadToFinish();

  debug("Successfully stopped asio context");
}

void Context::waitForThreadToFinish()
{
  constexpr auto SLEEP_DURATION_WHILE_WAITING = utils::Milliseconds(100);
  while (!m_contextThread.joinable())
  {
    std::this_thread::sleep_for(SLEEP_DURATION_WHILE_WAITING);
  }

  m_contextThread.join();
}

} // namespace net
