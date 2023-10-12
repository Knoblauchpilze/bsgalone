
#include "GameScreenInputHandler.hh"

namespace pge {

GameScreenInputHandler::GameScreenInputHandler(const bsgo::Views &views)
  : IInputHandler("game")
  , m_shipView(views.shipView)
  , m_systemView(views.systemView)
  , m_targetView(views.targetView)
{
  if (nullptr == m_shipView)
  {
    throw std::invalid_argument("Expected non null ship view");
  }
  if (nullptr == m_systemView)
  {
    throw std::invalid_argument("Expected non null system view");
  }
  if (nullptr == m_targetView)
  {
    throw std::invalid_argument("Expected non null target view");
  }
}

void GameScreenInputHandler::processUserInput(const controls::State &c, CoordinateFrame &frame)
{
  Motion motion{};
  motion.updateFromKeys(c);
  auto ship = m_shipView->getPlayerShip();
  moveShip(ship, motion);

  keepShipCentered(frame);
}

void GameScreenInputHandler::performAction(float x, float y, const controls::State & /*state*/)
{
  const Eigen::Vector3f pos(x, y, 0.0f);
  const auto ent = m_systemView->getEntityAt(pos);

  if (ent)
  {
    log("Found target " + ent->str());
    m_targetView->setTarget(ent->uuid);
  }
  else
  {
    m_targetView->clearTarget();
  }
}

void GameScreenInputHandler::moveShip(bsgo::Entity &ship, const Motion &motion)
{
  Eigen::Vector3f delta = Eigen::Vector3f::Zero();
  delta(0)              = motion.x;
  delta(1)              = motion.y;
  delta(2)              = motion.z;
  delta.normalize();

  ship.access<bsgo::Velocity>().accelerate(delta);
}

void GameScreenInputHandler::keepShipCentered(CoordinateFrame &frame)
{
  const auto ent = m_shipView->getPlayerShip();
  const auto pos = ent.access<bsgo::Transform>().position();
  const olc::vf2d pos2d{pos(0), pos(1)};
  frame.moveTo(pos2d);
}

} // namespace pge
