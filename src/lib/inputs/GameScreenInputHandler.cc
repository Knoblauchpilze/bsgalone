
#include "GameScreenInputHandler.hh"

namespace pge {

GameScreenInputHandler::GameScreenInputHandler(const bsgo::Views &views)
  : IInputHandler("game")
  , m_shipView(views.shipView)
  , m_systemView(views.systemView)
{
  if (nullptr == m_shipView)
  {
    throw std::invalid_argument("Expected non null ship view");
  }
  if (nullptr == m_systemView)
  {
    throw std::invalid_argument("Expected non null system view");
  }
}

void GameScreenInputHandler::processUserInput(const controls::State &controls,
                                              CoordinateFrame &frame)
{
  Motion motion{};
  motion.updateFromKeys(controls);

  auto ship = m_shipView->getPlayerShip();
  moveShip(ship, motion);
  keepShipCentered(frame);
  handleWeaponsState(controls, ship);
}

void GameScreenInputHandler::performAction(float x, float y, const controls::State & /*state*/)
{
  const Eigen::Vector3f pos(x, y, 0.0f);
  const auto ent  = m_systemView->getEntityAt(pos);
  auto playerShip = m_shipView->getPlayerShip();

  auto visible{true};
  if (ent && ent->exists<bsgo::StatusComponent>()
      && bsgo::Status::VISIBLE != ent->statusComp().status())
  {
    visible = false;
  }

  if (ent && visible)
  {
    log("Found target " + ent->str());
    playerShip.targetComp().setTarget(ent->uuid);
  }
  else
  {
    playerShip.targetComp().clearTarget();
  }
}

void GameScreenInputHandler::moveShip(bsgo::Entity &ship, const Motion &motion)
{
  Eigen::Vector3f direction = Eigen::Vector3f::Zero();
  direction(0)              = motion.x;
  direction(1)              = motion.y;
  direction(2)              = motion.z;
  direction.normalize();

  ship.velocityComp().accelerate(direction);
}

void GameScreenInputHandler::keepShipCentered(CoordinateFrame &frame)
{
  const auto ent = m_shipView->getPlayerShip();
  const auto pos = ent.transformComp().position();
  const olc::vf2d pos2d{pos(0), pos(1)};
  frame.moveTo(pos2d);
}

void GameScreenInputHandler::handleWeaponsState(const controls::State &c, bsgo::Entity &ship)
{
  if (c.keys[controls::keys::G])
  {
    for (const auto &weapon : ship.weapons)
    {
      weapon->toggle();
    }
    return;
  }

  if (c.keys[controls::keys::K1] && !ship.weapons.empty())
  {
    ship.weapons[0]->toggle();
  }
  if (c.keys[controls::keys::K2] && ship.weapons.size() > 1)
  {
    ship.weapons[1]->toggle();
  }

  if (c.keys[controls::keys::W] && m_shipView->getAbilitiesCount() > 0)
  {
    m_shipView->tryActivateSlot(ship.uuid, 0);
  }
  if (c.keys[controls::keys::X] && m_shipView->getAbilitiesCount() > 1)
  {
    m_shipView->tryActivateSlot(ship.uuid, 1);
  }
  if (c.keys[controls::keys::C] && m_shipView->getAbilitiesCount() > 2)
  {
    m_shipView->tryActivateSlot(ship.uuid, 2);
  }
  if (c.keys[controls::keys::V] && m_shipView->getAbilitiesCount() > 3)
  {
    m_shipView->tryActivateSlot(ship.uuid, 3);
  }
  if (c.keys[controls::keys::W] && m_shipView->getAbilitiesCount() > 0)
  {
    m_shipView->tryActivateSlot(ship.uuid, 0);
  }
}

} // namespace pge
