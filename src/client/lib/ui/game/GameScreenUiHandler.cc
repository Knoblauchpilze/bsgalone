
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

GameScreenUiHandler::GameScreenUiHandler(const Views &views)
  : IUiHandler("game")
{
  initializeUis(views);
}

void GameScreenUiHandler::initializeMenus(const int width,
                                          const int height,
                                          sprites::TexturePack &texturesLoader)
{
  for (const auto &ui : m_uis)
  {
    ui->initializeMenus(width, height, texturesLoader);
  }
}

bool GameScreenUiHandler::processUserInput(UserInputData &inputData)
{
  auto out{false};
  for (const auto &ui : m_uis)
  {
    out |= ui->processUserInput(inputData);
  }

  return out;
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

void GameScreenUiHandler::connectToMessageQueue(bsgalone::core::IMessageQueue &messageQueue)
{
  for (const auto &ui : m_uis)
  {
    ui->connectToMessageQueue(messageQueue);
  }
}

namespace {
const Vec2i STATUS_UI_PIXEL_POS{5, 5};
const Vec2i SHIP_UI_PIXEL_POS{5, 30};
const Vec2i TARGET_UI_PIXEL_POS{400, 30};
} // namespace

void GameScreenUiHandler::initializeUis(const Views &views)
{
  auto statusUi = std::make_unique<StatusUiHandler>(STATUS_UI_PIXEL_POS, views);
  m_uis.emplace_back(std::move(statusUi));

  auto shipUi = std::make_unique<EntityUiHandler>(EntityUiConfig{.offset = SHIP_UI_PIXEL_POS,
                                                                 .displayDistance = false,
                                                                 .getEntity =
                                                                   [](const ShipView &shipView) {
                                                                     return shipView.getPlayerShip();
                                                                   }},
                                                  views);
  m_uis.emplace_back(std::move(shipUi));

  auto targetUi = std::make_unique<EntityUiHandler>(EntityUiConfig{.offset = TARGET_UI_PIXEL_POS,
                                                                   .displayDistance = true,
                                                                   .getEntity =
                                                                     [](const ShipView &shipView) {
                                                                       return shipView
                                                                         .getPlayerTarget();
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
