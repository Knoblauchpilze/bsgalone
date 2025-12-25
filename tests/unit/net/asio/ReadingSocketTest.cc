
#include "ReadingSocket.hh"
#include <gtest/gtest.h>

using namespace ::testing;

namespace net::details {
namespace {

class SocketConnector : public std::enable_shared_from_this<SocketConnector>
{
  public:
  SocketConnector()  = default;
  ~SocketConnector() = default;

  auto connect(asio::io_context &context, const int port) -> SocketShPtr
  {
    std::cout << "[connector] creating socket\n";
    auto socket = std::make_shared<asio::ip::tcp::socket>(context);
    asio::ip::tcp::resolver resolver(context);
    auto endpoints = resolver.resolve("127.0.0.1", std::to_string(port));

    std::cout << "[connector] creating future\n";
    auto result = m_connected.get_future();

    std::cout << "[connector] async connect\n";
    asio::async_connect(*socket,
                        endpoints,
                        std::bind(&SocketConnector::onConnectionEstablished,
                                  shared_from_this(),
                                  std::placeholders::_1,
                                  std::placeholders::_2));

    std::cout << "[connector] waiting for future\n";
    result.get();
    std::cout << "[connector] got future\n";

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
    std::cout << "[connector] established connection\n";
    m_connected.set_value(true);
  }
};

class TestTcpServer : public std::enable_shared_from_this<TestTcpServer>
{
  public:
  TestTcpServer(const int port)
    : m_port(port)
    , m_acceptor(m_context, asio::ip::tcp::endpoint(asio::ip::tcp::v4(), port))
  {
    std::cout << "[tcp server] constructor\n";
  }

  ~TestTcpServer()
  {
    std::cout << "[tcp server] destructor\n";
    stop();
    std::cout << "[tcp server] destroyed\n";
  }

  void setup()
  {
    std::cout << "[tcp server] starting acceptor\n";
    m_acceptor.async_accept(std::bind(&TestTcpServer::onConnectionRequest,
                                      shared_from_this(),
                                      std::placeholders::_1,
                                      std::placeholders::_2));
    std::cout << "[tcp server] accepting\n";
  }

  void start()
  {
    auto expected = false;
    if (!m_running.compare_exchange_strong(expected, true))
    {
      std::cout << "[tcp server] already started\n";
      FAIL() << "Failed to start test TCP server";
      return;
    }

    std::cout << "[tcp server] creating thread\n";
    m_contextThread = std::thread([this]() { m_context.run(); });
    std::cout << "[tcp server] thread created\n";
  }

  auto getSocket() -> SocketShPtr
  {
    auto connector = std::make_shared<SocketConnector>();
    return connector->connect(m_context, m_port);
  }

  public:
  int m_port{};
  asio::io_context m_context{};
  asio::ip::tcp::acceptor m_acceptor;

  std::atomic_bool m_running{false};
  std::thread m_contextThread{};

  std::vector<SocketShPtr> m_sockets{};

  void stop()
  {
    auto expected = true;
    if (!m_running.compare_exchange_strong(expected, false))
    {
      std::cout << "[tcp server] not started\n";
      FAIL() << "Failed to stop test TCP server";
      return;
    }

    std::cout << "[tcp server] stopping\n";
    m_context.stop();
    std::cout << "[tcp server] joining\n";
    m_contextThread.join();
    std::cout << "[tcp server] done\n";
  }

  void onConnectionRequest(const std::error_code code, asio::ip::tcp::socket socket)
  {
    if (code)
    {
      FAIL() << "Error detected when processing incoming connection: " << code.message();
    }

    std::cout << "[tcp server] received connection\n";
    m_sockets.push_back(std::make_shared<asio::ip::tcp::socket>(std::move(socket)));
  }
};

auto createTestTcpServer(const int port) -> std::shared_ptr<TestTcpServer>
{
  auto server = std::make_shared<TestTcpServer>(port);
  server->setup();
  server->start();
  return server;
}
} // namespace

TEST(Unit_Net_Asio_ReadingSocket, ReturnsNothingWhenNoDataReceived)
{
  std::cout << "very start\n";
  auto server = createTestTcpServer(4567);
  std::cout << "server started\n";
  auto asioSocket = server->getSocket();

  std::cout << "creating reading socket\n";
  auto socket = std::make_shared<ReadingSocket>(std::move(asioSocket));
  socket->connect();

  std::cout << "trying to read\n";
  const auto actual = socket->read();
  std::cout << "read stuff\n";

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

  void handle(asio::ip::tcp::socket &socket)
  {
    auto result = m_sent.get_future();

    std::cout << "[data sender] writing data\n";
    asio::async_write(socket,
                      asio::buffer(m_bytes.data(), m_bytes.size()),
                      std::bind(&DataSender::onDataSent,
                                shared_from_this(),
                                std::placeholders::_1,
                                std::placeholders::_2));

    std::cout << "[data sender] writing done\n";
    result.get();
    std::cout << "[data sender] writing confirmed\n";
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
    std::cout << "[data sender] received confirmation for send\n";
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
  std::cout << "[test] very start\n";
  auto server = createTestTcpServer(4568);
  std::cout << "[test] server started\n";
  auto asioSocket = server->getSocket();

  std::cout << "[test] creating reading socket in test body\n";
  auto socket = std::make_shared<ReadingSocket>(asioSocket);
  std::cout << "[test] connecting reading socket\n";
  socket->connect();
  std::cout << "[test] socket created\n";

  std::string data("test");
  auto sender = createDataSender(data);
  std::cout << "[test] sending data to socket out of " << server->m_sockets.size() << "\n";
  sender->handle(*server->m_sockets.front());

  std::cout << "[test] socket read\n";
  const auto actual = socket->read();
  std::cout << "[test] socket read finished\n";

  std::vector<char> expected(data.begin(), data.end());
  EXPECT_EQ(expected, actual);
}

} // namespace net::details
