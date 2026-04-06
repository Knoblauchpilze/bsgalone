
#pragma once

#include "ClientId.hh"
#include "Uuid.hh"
#include <memory>

namespace bsgalone::server {

class ForManagingClient
{
  public:
  ForManagingClient()          = default;
  virtual ~ForManagingClient() = default;

  virtual void registerPlayer(const net::ClientId clientId,
                              const core::Uuid playerDbId,
                              const core::Uuid playerSystemDbId)
    = 0;
};

using ForManagingClientShPtr = std::shared_ptr<ForManagingClient>;

} // namespace bsgalone::server
