
#include "AsioServer.hh"
#include "ClientConnectedEvent.hh"

#include <iostream>

namespace net::details {
std::atomic<ClientId> AsioServer::NEXT_CLIENT_ID{0};

AsioServer::AsioServer(const int port, IEventBusShPtr eventBus)
  : core::CoreObject("server")
  , m_acceptor(m_context, asio::ip::tcp::endpoint(asio::ip::tcp::v4(), port))
  , m_eventBus(std::move(eventBus))
{
  setService("net");

  if (m_eventBus == nullptr)
  {
    throw std::invalid_argument("Expected non null event bus");
  }
}

void AsioServer::start()
{
  auto expected = false;
  if (!m_running.compare_exchange_strong(expected, true))
  {
    throw std::runtime_error(
      "Got unexpected state for TCP server, did you already call the start method?");
  }

  registerToAsio();

  m_contextThread = std::thread([this]() { m_context.run(); });
}

void AsioServer::stop()
{
  auto expected = true;
  if (!m_running.compare_exchange_strong(expected, false))
  {
    throw std::runtime_error(
      "Got unexpected state for TCP server, did you forget to call the start method?");
  }

  m_context.stop();
  m_contextThread.join();
}

void AsioServer::registerToAsio()
{
  std::cout << "[server] register to asio\n";
  // https://www.boost.org/doc/libs/1_66_0/doc/html/boost_asio/reference/ReadHandler.html
  m_acceptor.async_accept(std::bind(&AsioServer::onConnectionRequest,
                                    shared_from_this(),
                                    std::placeholders::_1,
                                    std::placeholders::_2));
}

void AsioServer::onConnectionRequest(const std::error_code &code, asio::ip::tcp::socket socket)
{
  std::cout << "[server] got connection request\n";
  registerToAsio();

  if (code)
  {
    warn("Error detected when processing incoming connection", code.message());
    return;
  }

  std::cout << "[server] registering connection\n";
  const auto clientId = registerConnection(std::move(socket));
  std::cout << "[server] publishing event\n";
  publishClientConnectedEvent(clientId);
  std::cout << "[server] callback done\n";
}

auto AsioServer::registerConnection(asio::ip::tcp::socket rawSocket) -> ClientId
{
  auto socket = std::make_shared<asio::ip::tcp::socket>(std::move(rawSocket));

  const auto clientId = NEXT_CLIENT_ID.fetch_add(1);

  auto reader = std::make_shared<ReadingSocket>(clientId, socket, m_eventBus);
  auto writer = std::make_shared<WritingSocket>(clientId, socket, m_eventBus);

  reader->connect();

  const std::lock_guard guard(m_connectionsLocker);
  m_readers.emplace(clientId, std::move(reader));
  m_writers.emplace(clientId, std::move(writer));

  return clientId;
}

void AsioServer::publishClientConnectedEvent(const ClientId clientId)
{
  auto event = std::make_unique<ClientConnectedEvent>(clientId);
  m_eventBus->pushEvent(std::move(event));
}

} // namespace net::details
