
#include "TcpServerFixture.hh"

namespace test {
class Acceptor : public std::enable_shared_from_this<Acceptor>
{
  public:
  Acceptor(asio::io_context &context, const int port)
    : m_acceptor(context, asio::ip::tcp::endpoint(asio::ip::tcp::v4(), port))
  {}

  ~Acceptor() = default;

  void registerAccept()
  {
    m_acceptor.async_accept(std::bind(&Acceptor::onConnectionRequest,
                                      shared_from_this(),
                                      std::placeholders::_1,
                                      std::placeholders::_2));
  }

  auto waitForServerSocket() -> net::SocketShPtr
  {
    std::unique_lock guard(m_locker);

    // This timeout should be enough for most test scenario. If it becomes
    // an issue it can be made configurable.
    constexpr auto REASONABLE_TIMEOUT = std::chrono::seconds(5);
    m_notifier.wait_for(guard, REASONABLE_TIMEOUT, [this] { return m_socket != nullptr; });

    net::SocketShPtr out{};
    std::swap(out, m_socket);

    if (!out)
    {
      throw std::runtime_error("Failed to connect to server, got null socket");
    }

    return out;
  }

  private:
  asio::ip::tcp::acceptor m_acceptor;

  std::mutex m_locker{};
  std::condition_variable m_notifier{};

  net::SocketShPtr m_socket{};

  void onConnectionRequest(const std::error_code &code, asio::ip::tcp::socket socket)
  {
    if (code)
    {
      throw std::runtime_error("Received code " + std::to_string(code.value()) + " (message: \""
                               + code.message() + "\") while processing incoming connection");
    }

    registerAccept();

    std::unique_lock guard(m_locker);
    m_socket = std::make_shared<asio::ip::tcp::socket>(std::move(socket));
    m_notifier.notify_one();
  }
};

auto ConnectedSocket::readServer(const std::size_t length) -> std::string
{
  std::vector<char> actual(length, 0);
  const auto received = asio::read(*server, asio::buffer(actual.data(), actual.size()));

  if (received != length)
  {
    throw std::runtime_error("Failed to receive all bytes, expected " + std::to_string(length)
                             + ", only received " + std::to_string(received));
  }

  return std::string(actual.begin(), actual.end());
}

void ConnectedSocket::writeServer(const std::string &data)
{
  const auto transferred = asio::write(*server, asio::buffer(data.data(), data.size()));
  if (transferred != data.size())
  {
    throw std::runtime_error("Failed to transfer all bytes of " + data + ", only transferred "
                             + std::to_string(transferred));
  }
}

void TcpServerFixture::SetUp()
{
  this->TcpFixture::SetUp();

  m_acceptor = std::make_shared<Acceptor>(m_context, m_port);
  m_acceptor->registerAccept();

  m_contextThread = std::thread([this]() { m_context.run(); });
}

void TcpServerFixture::TearDown()
{
  m_context.stop();
  m_contextThread.join();

  this->TcpFixture::TearDown();
}

auto TcpServerFixture::connectBoth() -> ConnectedSocket
{
  auto client = this->connect();
  auto server = m_acceptor->waitForServerSocket();

  return ConnectedSocket{.client = std::move(client), .server = std::move(server)};
}

} // namespace test
