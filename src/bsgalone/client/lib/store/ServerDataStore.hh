
#pragma once

#include "CoreObject.hh"
#include "IDataStore.hh"

namespace bsgalone::client {

class ServerDataStore : public IDataStore, public ::core::CoreObject
{
  public:
  ServerDataStore();

  ~ServerDataStore() override = default;

  void onPlayerLoggedIn(const core::Uuid playerDbId, const core::GameRole role) override;

  private:
  /// @brief - Holds the data representing the player currently logged in.
  struct PlayerData
  {
    core::Uuid playerDbId{};
    core::GameRole role{};
  };

  /// @brief - If defined, the data for the player currently logged in. When
  /// not defined, indicates that the player has logged out or has not yet
  /// logged in.
  std::optional<PlayerData> m_playerData{};
};

} // namespace bsgalone::client
