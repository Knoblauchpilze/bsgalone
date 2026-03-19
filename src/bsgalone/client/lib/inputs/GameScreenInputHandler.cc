
#include "GameScreenInputHandler.hh"

namespace bsgalone::client {

GameScreenInputHandler::GameScreenInputHandler(const Views &views)
  : IInputHandler("game")
  , m_gameView(views.gameView)
{
  if (nullptr == m_gameView)
  {
    throw std::invalid_argument("Expected non null game view");
  }
}

void GameScreenInputHandler::processUserInput(const pge::controls::State &controls,
                                              pge::CoordinateFrame &frame)
{
  if (!m_gameView->isReady())
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

void GameScreenInputHandler::performAction(float x, float y, const pge::controls::State & /*state*/)
{
  if (!m_gameView->isReady())
  {
    return;
  }

  const Eigen::Vector3f pos(x, y, 0.0f);

  m_gameView->tryAcquireTarget(pos);
}

namespace {
bool motionStateChanged(const Motion &motion, const std::optional<Motion> &lastMotion)
{
  if (!motion.hasMotion())
  {
    // Case were we don't have motion anymore: it depends whether we did not have
    // motion the last frame already.
    return lastMotion.has_value();
  }

  // We do have motion.
  if (!lastMotion.has_value())
  {
    // If the last frame did not have motion then it's different.
    return true;
  }

  // We do have motion and it was already the case last frame: in this case we
  // need to check which motion we had.
  return motion.x != lastMotion->x || motion.y != lastMotion->y || motion.z != lastMotion->z;
}
} // namespace

void GameScreenInputHandler::moveShip(const Motion &motion)
{
  if (!motionStateChanged(motion, m_lastMotion))
  {
    return;
  }

  if (motion.hasMotion())
  {
    m_lastMotion = motion;
  }
  else
  {
    m_lastMotion.reset();
  }

  Eigen::Vector3f direction = Eigen::Vector3f::Zero();
  direction(0)              = motion.x;
  direction(1)              = motion.y;
  direction(2)              = motion.z;
  direction.normalize();

  m_gameView->accelerateShip(direction);
}

void GameScreenInputHandler::keepShipCentered(pge::CoordinateFrame &frame)
{
  const auto ent = m_gameView->getPlayerShip();
  const auto pos = ent.transformComp().position();
  const pge::Vec2f pos2d{pos(0), pos(1)};
  frame.moveTo(pos2d);
}

void GameScreenInputHandler::handleWeapons(const pge::controls::State &controls)
{
  const auto weaponsCount = m_gameView->getWeaponsCount();

  if (controls.released(pge::controls::keys::G))
  {
    for (auto id = 0; id < weaponsCount; ++id)
    {
      m_gameView->tryActivateWeapon(id);
    }
    return;
  }

  if (controls.released(pge::controls::keys::K1) && weaponsCount > 0)
  {
    m_gameView->tryActivateWeapon(0);
  }
  if (controls.released(pge::controls::keys::K2) && weaponsCount > 1)
  {
    m_gameView->tryActivateWeapon(1);
  }
  if (controls.released(pge::controls::keys::K3) && weaponsCount > 2)
  {
    m_gameView->tryActivateWeapon(2);
  }
}

void GameScreenInputHandler::handleAbilities(const pge::controls::State &controls)
{
  const auto abilitiesCount = m_gameView->getAbilitiesCount();
  if (controls.released(pge::controls::keys::W) && abilitiesCount > 0)
  {
    m_gameView->tryActivateSlot(0);
  }
  if (controls.released(pge::controls::keys::X) && abilitiesCount > 1)
  {
    m_gameView->tryActivateSlot(1);
  }
  if (controls.released(pge::controls::keys::C) && abilitiesCount > 2)
  {
    m_gameView->tryActivateSlot(2);
  }
  if (controls.released(pge::controls::keys::V) && abilitiesCount > 3)
  {
    m_gameView->tryActivateSlot(3);
  }
}

void GameScreenInputHandler::handleJumpState(const pge::controls::State &controls)
{
  if (controls.released(pge::controls::keys::J))
  {
    m_gameView->startJump();
  }
  if (controls.released(pge::controls::keys::K))
  {
    m_gameView->cancelJump();
  }
}

} // namespace bsgalone::client
