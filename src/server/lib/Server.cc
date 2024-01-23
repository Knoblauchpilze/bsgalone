

#include "Server.hh"
#include "AsyncMessageQueue.hh"
#include "MessageConsumerUtils.hh"
#include "MessageQueue.hh"
#include "SynchronizedMessageQueue.hh"
#include <core_utils/TimeUtils.hh>

namespace bsgo {

Server::Server()
  : utils::CoreObject("server")
{
  setService("server");
  initialize();
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

namespace {
auto createSynchronizedMessageQueue() -> IMessageQueuePtr
{
  auto messageQueue = std::make_unique<MessageQueue>();
  return std::make_unique<SynchronizedMessageQueue>(std::move(messageQueue));
}
} // namespace

void Server::initialize()
{
  const auto repositories = m_dataSource.repositories();

  m_inputMessagesQueue  = std::make_unique<AsyncMessageQueue>(createSynchronizedMessageQueue());
  m_outputMessagesQueue = createSynchronizedMessageQueue();

  m_coordinator      = std::make_shared<bsgo::Coordinator>(m_inputMessagesQueue.get());
  m_services         = createServices(repositories, m_coordinator);
  m_messageConsumers = registerAllConsumersToQueue(m_inputMessagesQueue.get(),
                                                   m_outputMessagesQueue.get(),
                                                   m_services);

  /// TODO: Should not simulate a single system.
  m_dataSource.setSystemDbId(Uuid{0});
  m_dataSource.initialize(*m_coordinator);
}

void Server::setup(const int port)
{
  const net::ServerConfig
    config{.acceptor =
             [this](const net::Connection &connection) { return onConnectionReceived(connection); },
           .disconnectHandler =
             [this](const net::ConnectionId connectionId) { return onConnectionLost(connectionId); },
           .connectionReadyHandler =
             [this](const net::Connection &connection) { onConnectionReady(connection); },
           .connectionDataHandler =
             [this](const net::ConnectionId connectionId, const std::deque<char> &data) {
               return onDataReceived(connectionId, data);
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
  return true;
}

void Server::onConnectionLost(const net::ConnectionId connectionId)
{
  m_clientManager.removeConnection(connectionId);
}

void Server::onConnectionReady(const net::Connection &connection)
{
  m_clientManager.registerConnection(connection.id());
}

auto Server::onDataReceived(const net::ConnectionId /*connectionId*/, const std::deque<char> &data)
  -> int
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
    handleReceivedMessages(std::move(messages));
  }

  return processedBytes;
}

void Server::handleReceivedMessages(std::vector<IMessagePtr> &&messages)
{
  for (auto &message : messages)
  {
    m_inputMessagesQueue->pushMessage(std::move(message));
  }
}

} // namespace bsgo
