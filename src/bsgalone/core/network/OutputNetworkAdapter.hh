
#pragma once

#include "INetworkClient.hh"
#include "INetworkServer.hh"
#include "IOutputNetworkAdapter.hh"

namespace bsgalone::core {

class OutputNetworkAdapter : public IOutputNetworkAdapter
{
  public:
  OutputNetworkAdapter(net::INetworkServerShPtr server);
  OutputNetworkAdapter(net::INetworkClientShPtr client);
  ~OutputNetworkAdapter() override = default;

  void sendMessage(const net::ClientId clientId, const IMessage &message);
  void sendMessage(const IMessage &message);

  private:
  net::INetworkServerShPtr m_server{};
  net::INetworkClientShPtr m_client{};

  auto serializeMessage(const IMessage &message) const -> std::vector<char>;
};

} // namespace bsgalone::core
