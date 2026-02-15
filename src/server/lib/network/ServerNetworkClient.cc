
#include "ServerNetworkClient.hh"
#include "AsyncEventBus.hh"
#include "AsyncMessageQueue.hh"
#include "BroadcastMessageListener.hh"
#include "ClientEventListener.hh"
#include "MessageParser.hh"
#include "NetworkAdapter.hh"
#include "ServerEventListener.hh"
#include "SynchronizedEventBus.hh"
#include "SynchronizedMessageQueue.hh"
#include "TcpServer.hh"

namespace bsgo {

ServerNetworkClient::ServerNetworkClient(bsgalone::server::ClientManagerShPtr clientManager)
  : m_clientManager(std::move(clientManager))
  , m_inputQueue(std::make_shared<AsyncMessageQueue>(
      std::make_unique<SynchronizedMessageQueue>("synchronized-queue-for-input")))
{}

void ServerNetworkClient::start(const int port)
{
  m_eventBus  = std::make_shared<net::AsyncEventBus>(std::make_unique<net::SynchronizedEventBus>());
  m_tcpServer = std::make_shared<net::TcpServer>(m_eventBus);

  initialize();
  m_tcpServer->start(port);
}

void ServerNetworkClient::stop()
{
  m_tcpServer->stop();
  m_tcpServer.reset();
  m_eventBus.reset();

  m_outputQueue.reset();
}

void ServerNetworkClient::pushMessage(bsgalone::core::IMessagePtr message)
{
  if (!m_started.load())
  {
    throw std::invalid_argument("Failed to send message, server not started");
  }

  m_outputQueue->pushMessage(std::move(message));
}

void ServerNetworkClient::addListener(bsgalone::core::IMessageListenerPtr listener)
{
  m_inputQueue->addListener(std::move(listener));
}

bool ServerNetworkClient::empty()
{
  return m_inputQueue->empty();
}

void ServerNetworkClient::processMessages()
{
  m_inputQueue->processMessages();
}

void ServerNetworkClient::initialize()
{
  auto eventListener = std::make_unique<bsgalone::server::ServerEventListener>(m_started);
  m_eventBus->addListener(std::move(eventListener));

  auto networkAdapter
    = std::make_unique<bsgalone::core::NetworkAdapter>(m_inputQueue,
                                                       std::make_unique<bsgo::MessageParser>());
  m_eventBus->addListener(std::move(networkAdapter));

  auto syncQueue = std::make_unique<bsgo::SynchronizedMessageQueue>("synchronized-queue-for-output");
  m_outputQueue  = std::make_shared<AsyncMessageQueue>(std::move(syncQueue));

  auto broadcastListener
    = std::make_unique<bsgalone::server::BroadcastMessageListener>(m_clientManager, m_tcpServer);
  m_outputQueue->addListener(std::move(broadcastListener));

  auto clientListener = std::make_unique<bsgalone::server::ClientEventListener>(m_clientManager,
                                                                                m_inputQueue);
  m_eventBus->addListener(std::move(clientListener));
}

} // namespace bsgo
