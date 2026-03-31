
#include "GameNetworkClient.hh"
#include "AsyncEventQueue.hh"
#include "InputNetworkAdapter.hh"
#include "MessageParser.hh"
#include "MessageSerializer.hh"
#include "NetworkEventListener.hh"
#include "OutputNetworkAdapter.hh"
#include "SynchronizedEventQueue.hh"
#include "SynchronizedMessageQueue.hh"
#include "TcpClient.hh"

namespace bsgalone::client {
constexpr auto DEFAULT_SERVER_URL = "127.0.0.1";

GameNetworkClient::GameNetworkClient()
  : ::core::CoreObject("client")
{
  setService("network");
}

void GameNetworkClient::start(const int port)
{
  m_eventBus  = net::createAsyncEventQueue(net::createSynchronizedEventQueue());
  m_tcpClient = std::make_shared<net::TcpClient>(m_eventBus);
  m_adapter
    = std::make_shared<core::OutputNetworkAdapter>(m_tcpClient,
                                                   std::make_unique<core::MessageSerializer>());

  m_inputQueue = core::createSynchronizedMessageQueue();

  initialize();

  m_tcpClient->connect(DEFAULT_SERVER_URL, port);
}

void GameNetworkClient::stop()
{
  m_tcpClient->disconnect();
  m_adapter.reset();
  m_tcpClient.reset();
  m_eventBus.reset();

  m_inputQueue.reset();
}

void GameNetworkClient::pushEvent(core::IMessagePtr message)
{
  if (!m_connected.load())
  {
    warn("Discarding message " + str(message->type()) + ", not connected to server");
    return;
  }

  m_adapter->sendMessage(*message);
}

void GameNetworkClient::addListener(core::IMessageListenerPtr listener)
{
  m_inputQueue->addListener(std::move(listener));
}

bool GameNetworkClient::empty()
{
  return m_inputQueue->empty();
}

void GameNetworkClient::processEvents()
{
  m_inputQueue->processEvents();
}

void GameNetworkClient::initialize()
{
  auto eventListener = std::make_unique<NetworkEventListener>(m_connected);
  m_eventBus->addListener(std::move(eventListener));

  auto networkAdapter
    = std::make_unique<core::InputNetworkAdapter>(m_inputQueue,
                                                  std::make_unique<core::MessageParser>());
  m_eventBus->addListener(std::move(networkAdapter));
}

} // namespace bsgalone::client
