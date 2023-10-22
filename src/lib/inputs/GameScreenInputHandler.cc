
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

void GameScreenInputHandler::processUserInput(const controls::State &c, CoordinateFrame &frame)
{
  Motion motion{};
  motion.updateFromKeys(c);

  auto ship = m_shipView->getPlayerShip();
  moveShip(ship, motion);
  keepShipCentered(frame);
  handleWeaponsState(c, ship);
}

void GameScreenInputHandler::performAction(float x, float y, const controls::State & /*state*/)
{
  const Eigen::Vector3f pos(x, y, 0.0f);
  const auto ent  = m_systemView->getEntityAt(pos);
  auto playerShip = m_shipView->getPlayerShip();

  if (ent)
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
  if (!c.keys[controls::keys::G])
  {
    return;
  }

  for (const auto &weapon : ship.weapons)
  {
    weapon->toggle();
  }
}

} // namespace pge
