
#pragma once

#include "Entity.hh"
#include "IInputHandler.hh"
#include "Motion.hh"
#include "Views.hh"
#include <optional>

namespace pge {

class GameScreenInputHandler : public IInputHandler
{
  public:
  GameScreenInputHandler(const Views &views);
  ~GameScreenInputHandler() override = default;

  void processUserInput(const controls::State &controls, CoordinateFrame &frame) override;
  void performAction(float x, float y, const controls::State &controls) override;

  private:
  ShipViewShPtr m_shipView{};
  ShipDbViewShPtr m_shipDbView{};
  std::optional<Motion> m_lastMotion{};

  void moveShip(const Motion &motion);
  void keepShipCentered(CoordinateFrame &frame);
  void handleWeapons(const controls::State &controls);
  void handleAbilities(const controls::State &controls);
  void handleJumpState(const controls::State &controls);
};

} // namespace pge
