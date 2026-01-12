
#include "AsioContext.hh"

namespace net::details {

AsioContext::AsioContext()
  : core::CoreObject("context")
{
  setService("net");
  start();
}

AsioContext::~AsioContext()
{
  stop();
}

auto AsioContext::get() -> asio::io_context &
{
  return m_context;
}

auto AsioContext::resolve(const std::string &url, const int port)
  -> asio::ip::tcp::resolver::results_type
{
  asio::ip::tcp::resolver resolver(m_context);
  return resolver.resolve(url, std::to_string(port));
}

void AsioContext::start()
{
  auto expected = false;
  if (!m_running.compare_exchange_strong(expected, true))
  {
    throw std::runtime_error(
      "Got unexpected state for asio context, did you already call the start method?");
  }

  m_contextThread = std::thread(&AsioContext::asyncRunningLoop, this);

  debug("Successfully started asio context");
}

void AsioContext::stop()
{
  auto expected = true;
  if (!m_running.compare_exchange_strong(expected, false))
  {
    throw std::runtime_error(
      "Got unexpected state for TCP server, did you forget to call the start method?");
  }

  m_context.stop();
  m_contextThread.join();

  debug("Successfully stopped asio context");
}

void AsioContext::asyncRunningLoop()
{
  bool running = m_running.load();

  while (running)
  {
    m_context.run();

    running = m_running.load();
    if (running)
    {
      m_context.restart();
    }
  }
}

} // namespace net::details
