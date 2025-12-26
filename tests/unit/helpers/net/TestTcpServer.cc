
#include "TestTcpServer.hh"
#include "SocketConnector.hh"
#include <stdexcept>

// TODO: Should be removed.
#include <iostream>

namespace test {

TestTcpServer::TestTcpServer(const int port)
  : m_port(port)
  , m_acceptor(m_context, asio::ip::tcp::endpoint(asio::ip::tcp::v4(), port))
{
  std::cout << "[tcp server] constructor\n";
}

TestTcpServer::~TestTcpServer()
{
  std::cout << "[tcp server] destructor\n";
  stop();
  std::cout << "[tcp server] destroyed\n";
}

void TestTcpServer::registerAccept()
{
  std::cout << "[tcp server] starting acceptor\n";
  m_acceptor.async_accept(std::bind(&TestTcpServer::onConnectionRequest,
                                    shared_from_this(),
                                    std::placeholders::_1,
                                    std::placeholders::_2));
  std::cout << "[tcp server] accepting\n";
}

void TestTcpServer::start()
{
  auto expected = false;
  if (!m_running.compare_exchange_strong(expected, true))
  {
    std::cout << "[tcp server] already started\n";
    throw std::runtime_error(
      "Got unexpected state for TCP server, did you already call the start method?");
  }

  std::cout << "[tcp server] creating thread\n";
  m_contextThread = std::thread([this]() {
    m_context.run();
    std::cout << "[tcp server] Ran out of work!!\n";
  });
  std::cout << "[tcp server] thread created\n";
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

auto TestTcpServer::create(const int port) -> TestTcpServerShPtr
{
  auto server = std::make_shared<TestTcpServer>(port);
  server->registerAccept();
  server->start();
  return server;
}

void TestTcpServer::stop()
{
  auto expected = true;
  if (!m_running.compare_exchange_strong(expected, false))
  {
    std::cout << "[tcp server] not started\n";
    throw std::runtime_error(
      "Got unexpected state for TCP server, did you forget to call the start method?");
  }

  std::cout << "[tcp server] stopping\n";
  m_context.stop();
  std::cout << "[tcp server] joining\n";
  m_contextThread.join();
  std::cout << "[tcp server] done\n";
}

void TestTcpServer::onConnectionRequest(const std::error_code code, asio::ip::tcp::socket socket)
{
  if (code)
  {
    throw std::runtime_error("Received code " + std::to_string(code.value()) + "(message: \""
                             + code.message() + "\") while processing incoming connection");
  }

  std::cout << "[tcp server] received connection\n";
  m_sockets.push_back(std::make_shared<asio::ip::tcp::socket>(std::move(socket)));
  registerAccept();
}

} // namespace test
