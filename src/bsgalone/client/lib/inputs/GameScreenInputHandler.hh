
#pragma once

#include "Entity.hh"
#include "IInputHandler.hh"
#include "Motion.hh"
#include "Views.hh"
#include <optional>

namespace bsgalone::client {

class GameScreenInputHandler : public IInputHandler
{
  public:
  GameScreenInputHandler(const Views &views);
  ~GameScreenInputHandler() override = default;

  void processUserInput(const pge::controls::State &controls, pge::CoordinateFrame &frame) override;
  void performAction(float x, float y, const pge::controls::State &controls) override;

  private:
  GameViewShPtr m_gameView{};
  std::optional<Motion> m_lastMotion{};

  void moveShip(const Motion &motion);
  void keepShipCentered(pge::CoordinateFrame &frame);
  void handleWeapons(const pge::controls::State &controls);
  void handleAbilities(const pge::controls::State &controls);
  void handleJumpState(const pge::controls::State &controls);
};

} // namespace bsgalone::client
