
#include "GameScreenInputHandler.hh"

namespace pge {

GameScreenInputHandler::GameScreenInputHandler(const bsgo::Views &views)
  : IInputHandler("game")
  , m_shipView(views.shipView)
{
  if (nullptr == m_shipView)
  {
    throw std::invalid_argument("Expected non null ship view");
  }
}

void GameScreenInputHandler::processUserInput(const controls::State &controls,
                                              CoordinateFrame &frame)
{
  if (!m_shipView->isReady())
  {
    return;
  }

  Motion motion{};
  motion.updateFromKeys(controls);

  moveShip(motion);
  handleWeapons(controls);
  handleAbilities(controls);
  handleJumpState(controls);
  keepShipCentered(frame);
}

void GameScreenInputHandler::performAction(float x, float y, const controls::State & /*state*/)
{
  if (!m_shipView->isReady())
  {
    return;
  }

  const Eigen::Vector3f pos(x, y, 0.0f);

  m_shipView->tryAcquireTarget(pos);
}

void GameScreenInputHandler::moveShip(const Motion &motion)
{
  if (!m_shipView->isReady())
  {
    return;
  }

  Eigen::Vector3f direction = Eigen::Vector3f::Zero();
  direction(0)              = motion.x;
  direction(1)              = motion.y;
  direction(2)              = motion.z;
  direction.normalize();

  m_shipView->accelerateShip(direction);
}

void GameScreenInputHandler::keepShipCentered(CoordinateFrame &frame)
{
  const auto ent = m_shipView->getPlayerShip();
  const auto pos = ent.transformComp().position();
  const Vec2f pos2d{pos(0), pos(1)};
  frame.moveTo(pos2d);
}

void GameScreenInputHandler::handleWeapons(const controls::State &controls)
{
  const auto weaponsCount = m_shipView->getWeaponsCount();

  if (controls.released(controls::keys::G))
  {
    for (auto id = 0; id < weaponsCount; ++id)
    {
      m_shipView->tryActivateWeapon(id);
    }
    return;
  }

  if (controls.released(controls::keys::K1) && weaponsCount > 0)
  {
    m_shipView->tryActivateWeapon(0);
  }
  if (controls.released(controls::keys::K2) && weaponsCount > 1)
  {
    m_shipView->tryActivateWeapon(1);
  }
  if (controls.released(controls::keys::K3) && weaponsCount > 2)
  {
    m_shipView->tryActivateWeapon(2);
  }
}

void GameScreenInputHandler::handleAbilities(const controls::State &controls)
{
  const auto abilitiesCount = m_shipView->getAbilitiesCount();
  if (controls.released(controls::keys::W) && abilitiesCount > 0)
  {
    m_shipView->tryActivateSlot(0);
  }
  if (controls.released(controls::keys::X) && abilitiesCount > 1)
  {
    m_shipView->tryActivateSlot(1);
  }
  if (controls.released(controls::keys::C) && abilitiesCount > 2)
  {
    m_shipView->tryActivateSlot(2);
  }
  if (controls.released(controls::keys::V) && abilitiesCount > 3)
  {
    m_shipView->tryActivateSlot(3);
  }
}

void GameScreenInputHandler::handleJumpState(const controls::State &controls)
{
  if (controls.released(controls::keys::J))
  {
    m_shipView->startJump();
  }
  if (controls.released(controls::keys::K))
  {
    m_shipView->cancelJump();
  }
}

} // namespace pge
