
#include "AsioContext.hh"
#include "TcpAcceptor.hh"
#include "TcpFixture.hh"
#include <gtest/gtest.h>

using namespace test;
using namespace ::testing;

namespace net::details {
using Integration_Net_Server_AsioContext = TcpFixture;

TEST_F(Integration_Net_Server_AsioContext, AllowsExecutionOfNetworkCalls)
{
  AsioContext context{};

  auto acceptor = std::make_shared<TcpAcceptor>(context.get(), this->port());
  acceptor->registerAccept();

  auto client = this->connectToRunningServer();
  auto server = acceptor->waitForServerSocket();
}

TEST_F(Integration_Net_Server_AsioContext, AllowsToBeStartedBeforeAnyWorkIsRegistered)
{
  // The context is started in the constructor.
  AsioContext context{};

  auto acceptor = std::make_shared<TcpAcceptor>(context.get(), this->port());
  acceptor->registerAccept();

  auto client = this->connectToRunningServer();
  auto server = acceptor->waitForServerSocket();
}

} // namespace net::details
