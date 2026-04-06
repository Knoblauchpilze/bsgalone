
#include "ServerNetworkClient.hh"
#include "AsyncEventQueue.hh"
#include "ClientEventListener.hh"
#include "InputNetworkAdapter.hh"
#include "MessageParser.hh"
#include "ServerEventListener.hh"
#include "SynchronizedEventQueue.hh"
#include "TcpServer.hh"

namespace bsgalone::server {

ServerNetworkClient::ServerNetworkClient(ClientManagerShPtr clientManager)
  : ::core::CoreObject("client")
  , m_clientManager(std::move(clientManager))
{
  setService("network");

  if (m_clientManager == nullptr)
  {
    throw std::invalid_argument("Expected non null client manager");
  }
}

void ServerNetworkClient::start(const int port)
{
  m_eventBus  = net::createAsyncEventQueue(net::createSynchronizedEventQueue());
  m_tcpServer = std::make_shared<net::TcpServer>(m_eventBus);

  initialize();

  m_tcpServer->start(port);
}

void ServerNetworkClient::stop()
{
  m_tcpServer->stop();
  m_tcpServer.reset();
  m_eventBus.reset();
}

auto ServerNetworkClient::trySend(const net::ClientId clientId, std::vector<char> bytes)
  -> std::optional<net::MessageId>
{
  if (!m_started.load())
  {
    throw std::invalid_argument("Failed to send message, server not started");
  }

  return m_tcpServer->trySend(clientId, std::move(bytes));
}

void ServerNetworkClient::addListener(core::IMessageListenerPtr listener)
{
  m_inputQueue->addListener(std::move(listener));
}

void ServerNetworkClient::initialize()
{
  auto eventListener = std::make_unique<ServerEventListener>(m_started);
  m_eventBus->addListener(std::move(eventListener));

  auto clientListener = std::make_unique<ClientEventListener>(m_clientManager);
  m_eventBus->addListener(std::move(clientListener));

  auto inputNetworkAdapter
    = std::make_unique<core::InputNetworkAdapter>(m_inputQueue,
                                                  std::make_unique<core::MessageParser>());
  m_eventBus->addListener(std::move(inputNetworkAdapter));
}

} // namespace bsgalone::server
