
#pragma once

#include "GameRole.hh"
#include "Uuid.hh"
#include <memory>

namespace bsgalone::client {

class IDataStore
{
  public:
  IDataStore()          = default;
  virtual ~IDataStore() = default;

  /// @brief - Whether or not the server indicated that the player has
  /// successfully logged in.
  /// @return - true if a successful login operation was confirmed by
  /// the server
  virtual bool isLoggedIn() const = 0;

  /// @brief - Returns the identifier of the player currently logged in. In case
  /// the player is not logged in, an exception should be raised.
  /// @return - the identifier of the player currently logged in
  virtual auto getPlayerDbId() const -> core::Uuid = 0;

  virtual void onPlayerLoggedIn(const core::Uuid playerDbId, const core::GameRole role) = 0;
  virtual void onPlayerLoggedOut(const core::Uuid playerDbId)                           = 0;
};

using IDataStoreShPtr = std::shared_ptr<IDataStore>;

} // namespace bsgalone::client
