
#include "AsioContext.hh"
#include "TcpAcceptor.hh"
#include "TcpFixture.hh"
#include <gtest/gtest.h>

using namespace test;
using namespace ::testing;

namespace net::details {
using Integration_Net_Server_AsioContext = TcpFixture;

TEST_F(Integration_Net_Server_AsioContext, ThrowsWhenStartedTwice)
{
  AsioContext context{};
  context.start();

  EXPECT_THROW([&context]() { context.start(); }(), std::runtime_error);

  context.stop();
}

TEST_F(Integration_Net_Server_AsioContext, ThrowsWhenStoppedWithoutBeingStarted)
{
  EXPECT_THROW(
    []() {
      AsioContext context{};
      context.stop();
    }(),
    std::runtime_error);
}

TEST_F(Integration_Net_Server_AsioContext, ThrowsWhenStoppedTwice)
{
  AsioContext context{};
  context.start();
  context.stop();

  EXPECT_THROW([&context]() { context.stop(); }(), std::runtime_error);
}

TEST_F(Integration_Net_Server_AsioContext, AllowsExecutionOfNetworkCalls)
{
  AsioContext context{};

  auto acceptor = std::make_shared<TcpAcceptor>(context.get(), this->port());
  acceptor->registerAccept();

  context.start();

  auto client = this->connectToRunningServer();
  auto server = acceptor->waitForServerSocket();

  context.stop();
}

TEST_F(Integration_Net_Server_AsioContext, AllowsToBeStartedBeforeAnyWorkIsRegistered)
{
  AsioContext context{};
  context.start();

  auto acceptor = std::make_shared<TcpAcceptor>(context.get(), this->port());
  acceptor->registerAccept();

  auto client = this->connectToRunningServer();
  auto server = acceptor->waitForServerSocket();

  context.stop();
}

} // namespace net::details
