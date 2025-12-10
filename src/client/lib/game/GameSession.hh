
#pragma once

#include "CoreObject.hh"
#include "Faction.hh"
#include "GameRole.hh"
#include "LoadingTransition.hh"
#include "Screen.hh"
#include "TimeStep.hh"
#include "Uuid.hh"
#include <memory>
#include <optional>

namespace pge {

class GameSession : public core::CoreObject
{
  public:
  GameSession();
  ~GameSession() override = default;

  void startLoadingTransition(const Screen currentScreen, const bsgo::LoadingTransition transition);

  struct ScreenTransition
  {
    Screen previous{};
    Screen next{};
  };
  auto finishLoadingTransition(const bsgo::LoadingTransition transition) -> ScreenTransition;

  void onPlayerLoggedIn(const bsgo::Uuid playerDbId, const bsgo::GameRole role);
  void onPlayerLoggedOut();

  void onTimeStepReceived(const chrono::TimeStep &timeStep);

  void setPlayerFaction(const bsgo::Faction faction);

  /// @brief - Returns true whenever the player DB identifier is set. This allows
  /// to safely call getPlayerDbId.
  /// When this method returns false, calling getPlayerDbId will raise an error.
  bool hasPlayerDbId() const;

  /// @brief - Returns the DB identifier of the player currently logged in.
  /// If no player is logged in, an error is raised.
  auto getPlayerDbId() const -> bsgo::Uuid;

  /// @brief - Returns the faction of the player currently logged in. If no faction
  /// is defined, an error is raised.
  auto getFaction() const -> bsgo::Faction;

  /// @brief - Returns the game role of the player currently logged in. If no role
  /// is defined, an error is raised.
  auto getRole() const -> bsgo::GameRole;

  /// @brief - Returns true whenever the player's system DB identifier is set. This
  /// allows to safely call getSystemDbId.
  /// When this method returns false, calling getSystemDbId will raise an error.
  bool hasSystemDbId() const;

  /// @brief - Returns the DB identifier of the system the player is currently in.
  /// If no player is logged in or if the system is not defined, an error is raised.
  auto getSystemDbId() const -> bsgo::Uuid;

  /// @brief - Returns true whenever the time step for the current system is set.
  /// It allows to safely call getTimeStep.
  bool hasTimeStep() const;

  /// @brief - Returns the time step attached to the system the player is currently in.
  /// If no time step is defined an error is raised.
  auto getTimeStep() const -> chrono::TimeStep;

  /// @brief - Returns true whenever the DB identifier of the active ship for the
  /// player currently logged in is set. This allows to safely call getPlayerActiveShipDbId.
  /// When this method returns false, calling getPlayerActiveShipDbId will raise an
  /// error.
  bool hasPlayerActiveShipDbId() const;

  /// @brief - Returns the DB identifier of the active ship for the player
  /// currently logged in. If no ship is active, an error is raised.
  auto getPlayerActiveShipDbId() const -> bsgo::Uuid;

  void onActiveSystemChanged(const bsgo::Uuid systemDbId);
  void onActiveShipChanged(const bsgo::Uuid shipDbId);

  private:
  /// @brief - if defined, the DB identifier of the player currently logged in.
  std::optional<bsgo::Uuid> m_playerDbId{};

  /// @brief - if defined, the faction of the player currently logged in.
  std::optional<bsgo::Faction> m_faction{};

  /// @brief - if defined, the role of the player currently logged in.
  std::optional<bsgo::GameRole> m_role{};

  /// @brief - if defined, the DB identifier of the system the player is currently
  /// in. It is populated when the player logs in and changed when the player jumps
  /// to another system.
  std::optional<bsgo::Uuid> m_systemDbId{};

  /// @brief - if defined, the time step of the system the player is currently in.
  /// It is populated when the player undocks or jumps to another system.
  /// It can be used to convert game duration in ticks to real world seconds.
  std::optional<chrono::TimeStep> m_timeStep{};

  /// @brief - if defined, the DB identifier of the active ship for the player. It
  /// is not defined when the player initially logs in. It gets assigned when the
  /// player leaves the outpost and changed when the player changes ship in the
  /// hangar view in the outpost.
  std::optional<bsgo::Uuid> m_playerShipDbId{};

  struct LoadingData
  {
    /// @brief - holds the screen that was displayed before the current loading
    /// phase.
    Screen previousScreen{};

    /// @brief - holds the screen that should be displayed after the loading phase
    /// is finished.
    Screen nextScreen{};

    /// @brief - if defined, represents the loading transition received from the
    /// server. It is set upon receiving a `LoadingStartedMessage` and is unset
    /// upon receiving a `LoadingFinishedMessage`.
    std::optional<bsgo::LoadingTransition> transition{};
  };

  /// @brief - if defined, the current loading transition. The process is that:
  /// 1. the client application requests a change of screen (which will set both
  /// the value of `m_previousScreen` and `m_nextScreen`).
  /// 2. the server acknowledges the request and sends a `LoadingStartedMessage`
  ///    with the transition.
  /// 3. the client controls the value sent in the `LoadingStartedMessage` and
  ///    sets thie value. It will remain active for the duration of the loading
  ///    process.
  /// 4. some data is sent by the server corresponding to the current loading
  ///    phase.
  /// 5. the server sends a `LoadingFinishedMessage` with the same transition.
  /// 6. the client checks that the transition matches the one set in step 3.
  /// 7. the screen to transition to is made available as a result of calling
  ///   `finishLoadingTransition`.
  /// This logic is encapsulated in three methods:
  /// - `setupLoadingScreen`: prepares for a loading transition
  /// - `startLoadingTransition`: acknowledges that the transition mathces what
  ///   was setup
  /// - `finishLoadingTransition`: moves to the next screen and resets the
  ///   transition
  std::optional<LoadingData> m_loading{};
};

using GameSessionShPtr = std::shared_ptr<GameSession>;

} // namespace pge
