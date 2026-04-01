
#pragma once

#include "CoreObject.hh"
#include "IDataStore.hh"
#include "IUiEventQueue.hh"

namespace bsgalone::client {

class ServerDataStore : public IDataStore, public ::core::CoreObject
{
  public:
  /// @brief - Creates a new data store to receive updates from the server.
  /// When enough data has been accumulated or there's a noteworthy event
  /// the input queue will be used to communicate UI relevant updates.
  /// @param queue - a queue to publish UI relevant updates
  ServerDataStore(IUiEventQueueShPtr queue);

  ~ServerDataStore() override = default;

  void onPlayerLoggedIn(const core::Uuid playerDbId,
                        const core::Uuid systemDbId,
                        const core::GameRole role) override;

  private:
  /// @brief - Holds the data representing the player currently logged in.
  struct PlayerData
  {
    core::Uuid playerDbId{};
    core::Uuid systemDbId{};
    core::GameRole role{};
  };

  /// @brief - If defined, the data for the player currently logged in. When
  /// not defined, indicates that the player has logged out or has not yet
  /// logged in.
  std::optional<PlayerData> m_playerData{};

  /// @brief - A queue to publish UI relevant updates.
  IUiEventQueueShPtr m_queue{};
};

} // namespace bsgalone::client
