
#include "ReadingSocket.hh"
#include "DataSender.hh"
#include "TestTcpServer.hh"
#include <gtest/gtest.h>

using namespace ::testing;
using namespace test;

namespace net::details {

TEST(Unit_Net_Asio_ReadingSocket, ReturnsNothingWhenNoDataReceived)
{
  std::cout << "very start\n";
  auto server = TestTcpServer::create(4567);
  std::cout << "server started\n";
  auto asioSocket = server->connect();

  std::cout << "creating reading socket\n";
  auto socket = std::make_shared<ReadingSocket>(std::move(asioSocket));
  socket->connect();

  std::cout << "trying to read\n";
  const auto actual = socket->read();
  std::cout << "read stuff\n";

  EXPECT_TRUE(actual.empty());
}

TEST(Unit_Net_Asio_ReadingSocket, ReturnsReceivedData)
{
  std::cout << "[test] very start\n";
  auto server = TestTcpServer::create(4568);
  std::cout << "[test] server started\n";
  auto asioSocket = server->connect();

  std::cout << "[test] creating reading socket in test body\n";
  auto socket = std::make_shared<ReadingSocket>(std::move(asioSocket));
  std::cout << "[test] connecting reading socket\n";
  socket->connect();
  std::cout << "[test] socket created\n";

  std::string data("test");
  auto sender = DataSender::create(data);
  std::cout << "[test] sending data to socket\n";
  sender->writeTo(*server->socket(0));

  std::this_thread::sleep_for(std::chrono::milliseconds(100));

  std::cout << "[test] socket read\n";
  const auto actual = socket->read();
  std::cout << "[test] socket read finished\n";

  std::vector<char> expected(data.begin(), data.end());
  EXPECT_EQ(expected, actual);
}

} // namespace net::details
