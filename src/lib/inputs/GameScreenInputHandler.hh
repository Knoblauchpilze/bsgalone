
#pragma once

#include "AbstractInputHandler.hh"
#include "Entity.hh"
#include "Motion.hh"
#include "Views.hh"

namespace pge {

class GameScreenInputHandler : public AbstractInputHandler
{
  public:
  GameScreenInputHandler(const bsgo::Views &views, bsgo::IMessageQueue *const messageQueue);
  ~GameScreenInputHandler() override = default;

  void processUserInput(const controls::State &controls, CoordinateFrame &frame) override;
  void performAction(float x, float y, const controls::State &controls) override;

  private:
  bsgo::ShipViewShPtr m_shipView;
  bsgo::SystemViewShPtr m_systemView;

  void moveShip(bsgo::Entity &ship, const Motion &motion);
  void keepShipCentered(CoordinateFrame &frame);
  void handleWeaponsState(const controls::State &controls, bsgo::Entity &ship);
  void handleJumpState(const controls::State &controls);
};

} // namespace pge
