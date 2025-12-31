
#include "ReadingSocket.hh"
#include "ClientDisconnectedEvent.hh"
#include "DataReceivedEvent.hh"
#include "IEventBus.hh"
#include "TcpServerFixture.hh"
#include <deque>
#include <gmock/gmock-matchers.h>
#include <gtest/gtest.h>

using namespace ::testing;

namespace net::details {
namespace {
using Integration_Net_Sockets_ReadingSocket = test::TcpServerFixture;

class DummyEventBus : public IEventBus
{
  public:
  DummyEventBus()           = default;
  ~DummyEventBus() override = default;

  void pushEvent(IEventPtr event) override
  {
    m_events.push_back(std::move(event));
    m_received.set_value(true);
  }

  void addListener(IEventListenerPtr /*listener*/) override
  {
    throw std::runtime_error("Unexpected call to addListener on DummyEventBus");
  }

  bool empty() override
  {
    throw std::runtime_error("Unexpected call to empty on DummyEventBus");
  }

  void processEvents() override
  {
    throw std::runtime_error("Unexpected call to processEvents on DummyEventBus");
  }

  /// @brief - Will block the calling thread until an event is received.
  /// Calling this function multiple times before it returns will cause
  /// undefined behavior.
  /// @return - the received event
  auto waitForEvent() -> IEventPtr
  {
    m_received         = std::promise<bool>();
    auto eventReceived = m_received.get_future();

    eventReceived.get();

    auto event = std::move(m_events.at(0));
    m_events.pop_front();
    return event;
  }

  private:
  std::promise<bool> m_received{};
  std::deque<IEventPtr> m_events{};
};
} // namespace

TEST_F(Integration_Net_Sockets_ReadingSocket, ThrowsWhenSocketIsNull)
{
  EXPECT_THROW(
    [] {
      DummyEventBus bus;
      ReadingSocket(ClientId{1}, nullptr, &bus);
    }(),
    std::invalid_argument);
}

TEST_F(Integration_Net_Sockets_ReadingSocket, ThrowsWhenEventBusIsNull)
{
  auto tcpSocket = this->connect();
  EXPECT_THROW([&tcpSocket]() { ReadingSocket(ClientId{1}, tcpSocket, nullptr); }(),
               std::invalid_argument);
}

TEST_F(Integration_Net_Sockets_ReadingSocket, ThrowsWhenConnectingTwice)
{
  auto tcpSocket = this->connect();
  DummyEventBus bus;
  auto socket = std::make_shared<ReadingSocket>(ClientId{1}, tcpSocket, &bus);
  socket->connect();

  EXPECT_THROW([&socket]() { socket->connect(); }(), core::CoreException);
}

TEST_F(Integration_Net_Sockets_ReadingSocket, PublishesDataReceivedEvent)
{
  auto tcpSocket = this->connect();
  DummyEventBus bus;
  auto socket = std::make_shared<ReadingSocket>(ClientId{1}, tcpSocket, &bus);
  socket->connect();

  std::string data{"test"};
  this->write(0, data);

  auto actual = bus.waitForEvent();
  EXPECT_EQ(EventType::DATA_RECEIVED, actual->type());
  EXPECT_EQ(ClientId{1}, actual->as<DataReceivedEvent>().clientId());
  const std::vector<char> expectedData(data.begin(), data.end());
  EXPECT_EQ(expectedData, actual->as<DataReceivedEvent>().data());
}

TEST_F(Integration_Net_Sockets_ReadingSocket, PublishesClientDisconnectedEventWhenSocketIsClosed)
{
  auto tcpSocket = this->connect();
  DummyEventBus bus;
  auto socket = std::make_shared<ReadingSocket>(ClientId{1}, tcpSocket, &bus);
  socket->connect();

  // Disconnect the server socket after some time.
  auto cleanup = std::async(std::launch::async, [this]() {
    std::this_thread::sleep_for(std::chrono::milliseconds(10));
    this->serverSocket(0)->close();
  });

  auto actual = bus.waitForEvent();
  EXPECT_EQ(EventType::CLIENT_DISCONNECTED, actual->type());
  EXPECT_EQ(ClientId{1}, actual->as<ClientDisconnectedEvent>().clientId());

  cleanup.get();
}

TEST_F(Integration_Net_Sockets_ReadingSocket, FailsToReconnectWhenSocketIsDisconnected)
{
  auto tcpSocket = this->connect();
  DummyEventBus bus;
  auto socket = std::make_shared<ReadingSocket>(ClientId{1}, tcpSocket, &bus);
  socket->connect();

  this->serverSocket(0)->close();

  auto actual = bus.waitForEvent();
  EXPECT_EQ(EventType::CLIENT_DISCONNECTED, actual->type());

  EXPECT_THAT([&socket]() { socket->connect(); },
              ThrowsMessage<core::CoreException>(HasSubstr("Cannot connect closed socket")));
}

} // namespace net::details
