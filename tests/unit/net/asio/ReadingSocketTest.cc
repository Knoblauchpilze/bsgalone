
#include "ReadingSocket.hh"
#include <gtest/gtest.h>

using namespace ::testing;

namespace net::details {
namespace {

using SocketHandler = std::function<void(asio::ip::tcp::socket &)>;

class SocketConnector : public std::enable_shared_from_this<SocketConnector>
{
  public:
  SocketConnector()  = default;
  ~SocketConnector() = default;

  auto connect(asio::io_context &context, const int port) -> SocketShPtr
  {
    std::cout << "creating socket" << std::endl;
    auto socket = std::make_shared<asio::ip::tcp::socket>(context);
    asio::ip::tcp::resolver resolver(context);
    auto endpoints = resolver.resolve("127.0.0.1", std::to_string(port));

    std::cout << "creating future" << std::endl;
    auto result = m_connected.get_future();

    std::cout << "async connect" << std::endl;
    asio::async_connect(*socket,
                        endpoints,
                        std::bind(&SocketConnector::onConnectionEstablished,
                                  shared_from_this(),
                                  std::placeholders::_1,
                                  std::placeholders::_2));

    std::cout << "waiting for future" << std::endl;
    result.get();
    std::cout << "got future" << std::endl;

    return socket;
  }

  private:
  std::promise<bool> m_connected{};

  void onConnectionEstablished(const std::error_code code,
                               const asio::ip::tcp::endpoint & /*endpoint*/)
  {
    if (code)
    {
      FAIL() << "Failed to establish connection to socket: " << code.message();
    }
    std::cout << "established connection" << std::endl;
    m_connected.set_value(true);
  }
};

class TestTcpServer : public std::enable_shared_from_this<TestTcpServer>
{
  public:
  TestTcpServer(const int port, SocketHandler handler)
    : m_port(port)
    , m_acceptor(m_context, asio::ip::tcp::endpoint(asio::ip::tcp::v4(), port))
    , m_handler(std::move(handler))
  {
    std::cout << "constructor" << std::endl;
  }

  ~TestTcpServer()
  {
    std::cout << "destructor" << std::endl;
    stop();
    std::cout << "destroyed" << std::endl;
  }

  void setup()
  {
    if (!m_handler)
    {
      std::cout << "no handler" << std::endl;
      FAIL() << "Please provide a valid connection handler to the test TCP server";
    }

    std::cout << "starting acceptor" << std::endl;
    m_acceptor.async_accept(std::bind(&TestTcpServer::onConnectionRequest,
                                      shared_from_this(),
                                      std::placeholders::_1,
                                      std::placeholders::_2));
    std::cout << "accepting" << std::endl;
  }

  void start()
  {
    auto expected = false;
    if (!m_running.compare_exchange_strong(expected, true))
    {
      std::cout << "already started" << std::endl;
      FAIL() << "Failed to start test TCP server";
      return;
    }

    std::cout << "creating thread" << std::endl;
    m_contextThread = std::thread([this]() { m_context.run(); });
    std::cout << "thread created" << std::endl;
  }

  auto getSocket() -> SocketShPtr
  {
    auto connector = std::make_shared<SocketConnector>();
    return connector->connect(m_context, m_port);
  }

  private:
  int m_port{};
  asio::io_context m_context{};
  asio::ip::tcp::acceptor m_acceptor;

  std::atomic_bool m_running{false};
  std::thread m_contextThread{};

  SocketHandler m_handler{};

  void stop()
  {
    auto expected = true;
    if (!m_running.compare_exchange_strong(expected, false))
    {
      std::cout << "not started" << std::endl;
      FAIL() << "Failed to stop test TCP server";
      return;
    }

    std::cout << "stopping" << std::endl;
    m_context.stop();
    std::cout << "joining" << std::endl;
    m_contextThread.join();
    std::cout << "done" << std::endl;
  }

  void onConnectionRequest(const std::error_code code, asio::ip::tcp::socket socket)
  {
    if (code)
    {
      FAIL() << "Error detected when processing incoming connection: " << code.message();
    }

    std::cout << "received connection" << std::endl;
    m_handler(socket);
    std::cout << "handler done" << std::endl;
  }
};

auto createTestTcpServer(const int port, SocketHandler handler) -> std::shared_ptr<TestTcpServer>
{
  auto server = std::make_shared<TestTcpServer>(port, handler);
  server->setup();
  server->start();
  return server;
}

auto noOpSocketHandler(asio::ip::tcp::socket & /*socket*/) {}
} // namespace

TEST(Unit_Net_Asio_ReadingSocket, ReturnsNothingWhenNoDataReceived)
{
  std::cout << "very start" << std::endl;
  auto server = createTestTcpServer(4567, noOpSocketHandler);
  std::cout << "server started" << std::endl;
  auto asioSocket = server->getSocket();

  std::cout << "creating reading socket" << std::endl;
  ReadingSocket socket(std::move(asioSocket));

  std::cout << "trying to read" << std::endl;
  const auto actual = socket.read();
  std::cout << "read stuff" << std::endl;

  EXPECT_TRUE(actual.empty());
}

namespace {
class DataSender : public std::enable_shared_from_this<DataSender>
{
  public:
  DataSender(const std::string &bytes)
    : DataSender(std::vector(bytes.begin(), bytes.end()))
  {}

  DataSender(std::vector<char> bytes)
    : m_bytes(std::move(bytes))
  {}

  ~DataSender() = default;

  auto handler() -> SocketHandler
  {
    return [this](asio::ip::tcp::socket &socket) {
      auto result = m_sent.get_future();

      std::cout << "writing data" << std::endl;
      asio::async_write(socket,
                        asio::buffer(m_bytes.data(), m_bytes.size()),
                        std::bind(&DataSender::onDataSent,
                                  shared_from_this(),
                                  std::placeholders::_1,
                                  std::placeholders::_2));

      std::cout << "writing done" << std::endl;
      result.get();
      std::cout << "writing confirmed" << std::endl;
    };
  }

  private:
  std::vector<char> m_bytes{};
  std::promise<bool> m_sent{};

  void onDataSent(const std::error_code code, const std::size_t contentLength)
  {
    if (code)
    {
      FAIL() << "Received error when sending data to socket: " << code.message() << "/"
             << code.value();
    }
    if (contentLength != m_bytes.size())
    {
      FAIL() << "Expected to send " << std::to_string(m_bytes.size()) << " byte(s) but only sent "
             << std::to_string(contentLength);
    }
    std::cout << "received confirmation for send" << std::endl;
    m_sent.set_value(true);
  }
};

auto createDataSender(const std::string &data) -> std::shared_ptr<DataSender>
{
  return std::make_shared<DataSender>(data);
}

} // namespace

TEST(Unit_Net_Asio_ReadingSocket, ReturnsReceivedData)
{
  auto sender = createDataSender("test");

  std::cout << "very start" << std::endl;
  auto server = createTestTcpServer(4568, sender->handler());
  std::cout << "server started" << std::endl;
  auto asioSocket = server->getSocket();

  std::cout << "creating socket" << std::endl;
  ReadingSocket socket(std::move(asioSocket));
  std::cout << "socket created" << std::endl;

  std::cout << "socket read" << std::endl;
  const auto actual = socket.read();
  std::cout << "socket read finished" << std::endl;

  EXPECT_TRUE(actual.empty());
}

} // namespace net::details
