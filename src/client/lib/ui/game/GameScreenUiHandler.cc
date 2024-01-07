
#include "GameScreenUiHandler.hh"
#include "AbilitiesUiHandler.hh"
#include "EntityUiHandler.hh"
#include "GameOverUiHandler.hh"
#include "LogUiHandler.hh"
#include "ScreenCommon.hh"
#include "ShipStatusUiHandler.hh"
#include "StatusUiHandler.hh"
#include "StringUtils.hh"
#include "WeaponsUiHandler.hh"

namespace pge {

GameScreenUiHandler::GameScreenUiHandler(const bsgo::Views &views)
  : IUiHandler("game")
{
  initializeUis(views);
}

void GameScreenUiHandler::initializeMenus(const int width, const int height)
{
  for (const auto &ui : m_uis)
  {
    ui->initializeMenus(width, height);
  }
}

bool GameScreenUiHandler::processUserInput(UserInputData &inputData)
{
  for (const auto &ui : m_uis)
  {
    if (ui->processUserInput(inputData))
    {
      return true;
    }
  }

  return false;
}

void GameScreenUiHandler::render(Renderer &engine) const
{
  for (const auto &ui : m_uis)
  {
    ui->render(engine);
  }
}

void GameScreenUiHandler::updateUi()
{
  for (const auto &ui : m_uis)
  {
    ui->updateUi();
  }
}

void GameScreenUiHandler::reset()
{
  for (const auto &ui : m_uis)
  {
    ui->reset();
  }
}

void GameScreenUiHandler::connectToMessageQueue(bsgo::IMessageQueue &messageQueue)
{
  for (const auto &ui : m_uis)
  {
    ui->connectToMessageQueue(messageQueue);
  }
}

namespace {
const olc::vi2d STATUS_UI_PIXEL_POS{5, 5};
const olc::vi2d SHIP_UI_PIXEL_POS{5, 30};
const olc::vi2d TARGET_UI_PIXEL_POS{400, 30};
} // namespace

void GameScreenUiHandler::initializeUis(const bsgo::Views &views)
{
  auto statusUi = std::make_unique<StatusUiHandler>(STATUS_UI_PIXEL_POS, views);
  m_uis.emplace_back(std::move(statusUi));

  auto shipUi
    = std::make_unique<EntityUiHandler>(EntityUiConfig{.offset          = SHIP_UI_PIXEL_POS,
                                                       .displayDistance = false,
                                                       .getEntity =
                                                         [](const bsgo::ShipView &shipView) {
                                                           return shipView.getPlayerShip();
                                                         }},
                                        views);
  m_uis.emplace_back(std::move(shipUi));

  auto targetUi
    = std::make_unique<EntityUiHandler>(EntityUiConfig{.offset          = TARGET_UI_PIXEL_POS,
                                                       .displayDistance = true,
                                                       .getEntity =
                                                         [](const bsgo::ShipView &shipView) {
                                                           return shipView.getPlayerTarget();
                                                         }},
                                        views);
  m_uis.emplace_back(std::move(targetUi));

  auto weaponsUi = std::make_unique<WeaponsUiHandler>(views);
  m_uis.emplace_back(std::move(weaponsUi));

  auto abilitiesUi = std::make_unique<AbilitiesUiHandler>(views);
  m_uis.emplace_back(std::move(abilitiesUi));

  auto gameOverUi = std::make_unique<GameOverUiHandler>(views);
  m_uis.emplace_back(std::move(gameOverUi));

  auto shipStatusUi = std::make_unique<ShipStatusUiHandler>(views);
  m_uis.emplace_back(std::move(shipStatusUi));

  auto logUi = std::make_unique<LogUiHandler>(views);
  m_uis.emplace_back(std::move(logUi));
}

} // namespace pge
