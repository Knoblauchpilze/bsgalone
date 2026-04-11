
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

  /// @brief - Associates the client identifier with a player and system. This
  /// association can be used to route messages to the right client when they
  /// are produced by the use cases or the game.
  /// @param clientId - the client identifier to link
  /// @param playerDbId - the identifier of the player logged in through the
  /// client
  /// @param playerSystemDbId - the system the player is in
  virtual void registerPlayer(const net::ClientId clientId,
                              const core::Uuid playerDbId,
                              const core::Uuid playerSystemDbId)
    = 0;

  /// @brief - Removes an existing association between a client and the player
  /// provided in input. This is typically used when a player logs out of the
  /// game but stays connected to the server: the client persists but the player
  /// might change.
  /// @param playerDbId - the identifier of the player to remove
  virtual void removePlayer(const core::Uuid playerDbId) = 0;
};

using ForManagingClientShPtr = std::shared_ptr<ForManagingClient>;

} // namespace bsgalone::server
