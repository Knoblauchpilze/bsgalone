
#pragma once

#include "Entity.hh"
#include "IInputHandler.hh"
#include "Motion.hh"
#include "Views.hh"

namespace pge {

class GameScreenInputHandler : public IInputHandler
{
  public:
  GameScreenInputHandler(const bsgo::Views &views);
  ~GameScreenInputHandler() override = default;

  void processUserInput(const controls::State &c, CoordinateFrame &frame) override;
  void performAction(float x, float y, const controls::State &state) override;

  private:
  bsgo::ShipViewShPtr m_shipView;
  bsgo::SystemViewShPtr m_systemView;

  void moveShip(bsgo::Entity &ship, const Motion &motion);
  void keepShipCentered(CoordinateFrame &frame);
  void handleWeaponsState(const controls::State &c, bsgo::Entity &ship);
};

} // namespace pge
