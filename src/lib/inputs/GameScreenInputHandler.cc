
#include "GameScreenInputHandler.hh"
#include "SlotMessage.hh"

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
  if (!m_shipView->isReady())
  {
    return;
  }

  Motion motion{};
  motion.updateFromKeys(controls);

  const auto ship = m_shipView->getPlayerShip();
  moveShip(motion, ship.uuid);
  handleWeapons(controls, ship.uuid);
  handleAbilities(controls, ship.uuid);
  handleJumpState(controls);
  keepShipCentered(frame);
}

void GameScreenInputHandler::performAction(float x, float y, const controls::State & /*state*/)
{
  if (!m_shipView->isReady() || !m_systemView->isReady())
  {
    return;
  }

  const Eigen::Vector3f pos(x, y, 0.0f);
  const auto ent  = m_systemView->getEntityAt(pos);
  auto playerShip = m_shipView->getPlayerShip();

  auto visible{true};
  if (ent && ent->exists<bsgo::StatusComponent>()
      && !bsgo::statusVisibleFromDradis(ent->statusComp().status()))
  {
    visible = false;
  }

  if (ent && visible)
  {
    info("Found target " + ent->str());
    playerShip.targetComp().setTarget(ent->uuid);
  }
  else
  {
    playerShip.targetComp().clearTarget();
  }
}

void GameScreenInputHandler::moveShip(const Motion &motion, const bsgo::Uuid &shipEntityId)
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

  m_shipView->accelerateShip(shipEntityId, direction);
}

void GameScreenInputHandler::keepShipCentered(CoordinateFrame &frame)
{
  const auto ent = m_shipView->getPlayerShip();
  const auto pos = ent.transformComp().position();
  const olc::vf2d pos2d{pos(0), pos(1)};
  frame.moveTo(pos2d);
}

void GameScreenInputHandler::handleWeapons(const controls::State &controls,
                                           const bsgo::Uuid &shipEntityId)
{
  const auto weaponsCount = m_shipView->getWeaponsCount();

  if (controls.released(controls::keys::G))
  {
    for (auto id = 0; id < weaponsCount; ++id)
    {
      m_shipView->tryActivateWeapon(shipEntityId, id);
    }
    return;
  }

  if (controls.released(controls::keys::K1) && weaponsCount > 0)
  {
    m_shipView->tryActivateWeapon(shipEntityId, 0);
  }
  if (controls.released(controls::keys::K2) && weaponsCount > 1)
  {
    m_shipView->tryActivateWeapon(shipEntityId, 1);
  }
  if (controls.released(controls::keys::K3) && weaponsCount > 2)
  {
    m_shipView->tryActivateWeapon(shipEntityId, 2);
  }
}

void GameScreenInputHandler::handleAbilities(const controls::State &controls,
                                             const bsgo::Uuid &shipEntityId)
{
  const auto abilitiesCount = m_shipView->getAbilitiesCount();
  if (controls.released(controls::keys::W) && abilitiesCount > 0)
  {
    m_shipView->tryActivateSlot(shipEntityId, 0);
  }
  if (controls.released(controls::keys::X) && abilitiesCount > 1)
  {
    m_shipView->tryActivateSlot(shipEntityId, 1);
  }
  if (controls.released(controls::keys::C) && abilitiesCount > 2)
  {
    m_shipView->tryActivateSlot(shipEntityId, 2);
  }
  if (controls.released(controls::keys::V) && abilitiesCount > 3)
  {
    m_shipView->tryActivateSlot(shipEntityId, 3);
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
