
#include "TestTcpServer.hh"
#include "SocketConnector.hh"
#include <stdexcept>

namespace test {

std::atomic_int TestTcpServer::NEXT_PORT{3000};

TestTcpServer::TestTcpServer()
  : m_port(NEXT_PORT.fetch_add(1))
  , m_acceptor(m_context, asio::ip::tcp::endpoint(asio::ip::tcp::v4(), m_port))
{}

TestTcpServer::~TestTcpServer()
{
  stop();
}

void TestTcpServer::registerAccept()
{
  m_acceptor.async_accept(std::bind(&TestTcpServer::onConnectionRequest,
                                    shared_from_this(),
                                    std::placeholders::_1,
                                    std::placeholders::_2));
}

auto TestTcpServer::port() const -> int
{
  return m_port;
}

void TestTcpServer::start()
{
  auto expected = false;
  if (!m_running.compare_exchange_strong(expected, true))
  {
    throw std::runtime_error(
      "Got unexpected state for TCP server, did you already call the start method?");
  }

  m_contextThread = std::thread([this]() { m_context.run(); });
}

void TestTcpServer::stop()
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

auto TestTcpServer::connect() -> net::SocketShPtr
{
  auto connector = std::make_shared<SocketConnector>();
  return connector->connect(m_context, m_port);
}

auto TestTcpServer::socket(const std::size_t index) -> net::SocketShPtr
{
  if (index >= m_sockets.size())
  {
    throw std::invalid_argument("Failed to access socket " + std::to_string(index) + ", only "
                                + std::to_string(m_sockets.size()) + " socket(s) connected");
  }

  return m_sockets[index];
}

auto TestTcpServer::create() -> TestTcpServerShPtr
{
  auto server = std::make_shared<TestTcpServer>();
  server->registerAccept();
  server->start();
  return server;
}

void TestTcpServer::onConnectionRequest(const std::error_code &code, asio::ip::tcp::socket socket)
{
  if (code)
  {
    throw std::runtime_error("Received code " + std::to_string(code.value()) + " (message: \""
                             + code.message() + "\") while processing incoming connection");
  }

  m_sockets.push_back(std::make_shared<asio::ip::tcp::socket>(std::move(socket)));
  registerAccept();
}

} // namespace test
