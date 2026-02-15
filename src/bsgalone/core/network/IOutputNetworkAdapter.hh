
#pragma once

#include "ClientId.hh"
#include "IMessage.hh"
#include <memory>

namespace bsgalone::core {

class IOutputNetworkAdapter
{
  public:
  virtual ~IOutputNetworkAdapter() = default;

  virtual void sendMessage(const net::ClientId clientId, const IMessage &message) = 0;
  virtual void sendMessage(const IMessage &message)                               = 0;
};

using IOutputNetworkAdapterPtr   = std::unique_ptr<IOutputNetworkAdapter>;
using IOutputNetworkAdapterShPtr = std::shared_ptr<IOutputNetworkAdapter>;

} // namespace bsgalone::core
