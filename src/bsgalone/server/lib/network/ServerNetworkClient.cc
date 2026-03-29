
#include "ServerNetworkClient.hh"
#include "AsyncEventQueue.hh"
#include "AsyncMessageQueue.hh"
#include "ClientEventListener.hh"
#include "InputNetworkAdapter.hh"
#include "MessageParser.hh"
#include "MessageSerializer.hh"
#include "ServerEventListener.hh"
#include "SynchronizedEventQueue.hh"
#include "SynchronizedMessageQueue.hh"
#include "TcpServer.hh"

namespace bsgalone::server {

void ServerNetworkClient::start(const int port)
{
  m_eventBus  = net::createAsyncEventQueue(net::createSynchronizedEventQueue());
  m_tcpServer = std::make_shared<net::TcpServer>(m_eventBus);

  m_inputQueue = core::createAsyncMessageQueue(core::createSynchronizedMessageQueue());

  initialize();

  m_tcpServer->start(port);
}

void ServerNetworkClient::stop()
{
  m_tcpServer->stop();
  m_tcpServer.reset();
  m_eventBus.reset();

  m_inputQueue.reset();
}

void ServerNetworkClient::pushEvent(core::IMessagePtr message)
{
  if (!m_started.load())
  {
    throw std::invalid_argument("Failed to send message " + str(message->type())
                                + ", server not started");
  }

  throw std::invalid_argument("Unsupported operation, not implemented");
}

void ServerNetworkClient::addListener(core::IMessageListenerPtr listener)
{
  m_inputQueue->addListener(std::move(listener));
}

bool ServerNetworkClient::empty()
{
  return m_inputQueue->empty();
}

void ServerNetworkClient::processEvents()
{
  m_inputQueue->processEvents();
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
