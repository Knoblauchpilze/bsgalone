

#include "Server.hh"
#include <core_utils/TimeUtils.hh>

namespace bsgo {

Server::Server()
  : utils::CoreObject("server")
{
  setService("server");
}

void Server::run(const int port)
{
  debug("Starting server...");
  setup(port);

  activeRunLoop();

  debug("Shutting down server...");
  shutdown();
}

void Server::requestStop()
{
  m_running.store(false);
}

void Server::setup(const int port)
{
  const net::ServerConfig config{.acceptor =
                                   [this](const net::Connection &connection) {
                                     return onConnectionReceived(connection);
                                   },
                                 .connectionDataHandler =
                                   [this](const std::deque<char> &data) {
                                     return onDataReceived(data);
                                   }};

  m_tcpServer = std::make_unique<net::TcpServer>(m_context, port, config);

  info("Starting listening on port " + std::to_string(m_tcpServer->port()));
  m_context.start();
}

void Server::activeRunLoop()
{
  m_running.store(true);
  bool running{true};
  while (running)
  {
    constexpr auto SLEEP_DURATION_WHEN_POLLING_STATUS = utils::Milliseconds(100);
    std::this_thread::sleep_for(SLEEP_DURATION_WHEN_POLLING_STATUS);

    running = m_running.load();
  }
}

void Server::shutdown()
{
  m_context.stop();
}

bool Server::onConnectionReceived(const net::Connection & /*connection*/) const
{
  /// TODO: Handle connection approval/denial.
  warn("should approve or veto connection");
  return true;
}

auto Server::onDataReceived(const std::deque<char> &data) -> int
{
  bool processedSomeBytes{true};
  auto processedBytes{0};
  std::vector<IMessagePtr> messages{};

  std::deque<char> workingData(data);

  while (processedSomeBytes)
  {
    auto result = m_messageParser.tryParseMessage(workingData);
    if (result.message)
    {
      messages.emplace_back(std::move(*result.message));
    }

    processedSomeBytes = (result.bytesProcessed > 0);
    processedBytes += result.bytesProcessed;

    workingData.erase(workingData.begin(), workingData.begin() + result.bytesProcessed);
  }

  if (!messages.empty())
  {
    handleReceivedMessages(messages);
  }

  return processedBytes;
}

void Server::handleReceivedMessages(const std::vector<IMessagePtr> &messages)
{
  warn("should process " + std::to_string(messages.size()) + " message(s)");
}

} // namespace bsgo
