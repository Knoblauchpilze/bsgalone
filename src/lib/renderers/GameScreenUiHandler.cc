
#include "GameScreenUiHandler.hh"
#include "ScreenCommon.hh"
#include "StringUtils.hh"

namespace pge {

GameScreenUiHandler::GameScreenUiHandler(const bsgo::Views &views)
  : m_shipView(views.shipView)
{
  if (nullptr == m_shipView)
  {
    throw std::invalid_argument("Expected non null ship view");
  }
}

void GameScreenUiHandler::loadResources(int width,
                                        int height,
                                        sprites::TexturePack & /*texturesLoader*/)
{
  m_menus.resize(MenuItem::COUNT);

  generateShipMenus(width, height);
  generateAbilityMenus(width, height);
  generateWeaponMenus(width, height);
  generateTargetMenus(width, height);
}

void GameScreenUiHandler::render(SpriteRenderer &engine,
                                 const RenderState & /*state*/,
                                 const RenderingPass pass) const
{
  if (pass != RenderingPass::UI)
  {
    return;
  }

  for (const auto &menu : m_menus)
  {
    menu->render(engine.getRenderer());
  }
}

auto GameScreenUiHandler::processUserInput(const controls::State &c,
                                           std::vector<ActionShPtr> &actions,
                                           CoordinateFrame & /*frame*/) -> menu::InputHandle
{
  bool relevant{false};
  bool selected{false};

  for (const auto &menu : m_menus)
  {
    menu::InputHandle ih = menu->processUserInput(c, actions);
    relevant             = (relevant || ih.relevant);
    selected             = (selected || ih.selected);
  }

  return menu::InputHandle{.relevant = relevant, .selected = selected};
}

void GameScreenUiHandler::updateUi()
{
  updateShipUi();
  updateTargetUi();
}

void GameScreenUiHandler::performAction(float /*x*/, float /*y*/, const controls::State & /*state*/)
{}

void GameScreenUiHandler::generateShipMenus(int /*width*/, int /*height*/)
{
  olc::vi2d pos{5, 5};
  olc::vi2d dims{200, 15};
  m_menus[HEALTH]
    = generateMenu(pos, dims, "Health: N/A", "health", olc::VERY_DARK_RED, {olc::WHITE});

  pos            = olc::vi2d{5, 25};
  m_menus[POWER] = generateMenu(pos, dims, "Power: N/A", "power", olc::DARK_CYAN, {olc::WHITE});
}

void GameScreenUiHandler::generateAbilityMenus(int width, int height)
{
  olc::vi2d dims{50, 50};
  constexpr auto SPACING_IN_PIXELS = 5;
  olc::vi2d pos;
  pos.x = width - ABILITY_COUNT * (dims.x + SPACING_IN_PIXELS);
  pos.y = height - SPACING_IN_PIXELS - dims.y;

  olc::Pixel color = olc::VERY_DARK_RED;
  color.a          = alpha::SemiOpaque;

  for (auto id = 0; id < ABILITY_COUNT; ++id)
  {
    const auto text = "A" + std::to_string(id);
    const auto name = "ability_" + std::to_string(id);

    m_menus[ABILITY_0 + id] = generateMenu(pos, dims, text, name, color, {olc::WHITE}, true);

    pos.x += (dims.x + SPACING_IN_PIXELS);
  }
}

void GameScreenUiHandler::generateWeaponMenus(int width, int height)
{
  olc::vi2d dims{50, 50};
  constexpr auto SPACING_IN_PIXELS = 5;
  olc::vi2d pos;
  pos.x = width - WEAPON_COUNT * (dims.x + SPACING_IN_PIXELS);
  pos.y = height / 2;

  olc::Pixel color = olc::VERY_DARK_RED;
  color.a          = alpha::SemiOpaque;

  for (auto id = 0; id < WEAPON_COUNT; ++id)
  {
    const auto text = "W" + std::to_string(id);
    const auto name = "weapon_" + std::to_string(id);

    m_menus[WEAPON_0 + id] = generateMenu(pos, dims, text, name, color, {olc::WHITE}, true);

    pos.x += (dims.x + SPACING_IN_PIXELS);
  }
}

void GameScreenUiHandler::generateTargetMenus(int width, int /*height*/)
{
  olc::vi2d pos{width / 2, 5};
  olc::vi2d dims{200, 15};
  m_menus[TARGET_HEALTH]
    = generateMenu(pos, dims, "Health: N/A", "health", olc::VERY_DARK_RED, {olc::WHITE});

  pos = olc::vi2d{width / 2, 25};
  m_menus[TARGET_POWER]
    = generateMenu(pos, dims, "Power: N/A", "power", olc::DARK_CYAN, {olc::WHITE});
}

void GameScreenUiHandler::updateShipUi()
{
  std::string text;

  const auto ship = m_shipView->getPlayerShip(m_shipView->getPlayerShipId());

  text = "Health: ";
  text += floatToStr(std::floor(ship.hullPoints), 0);
  text += "/";
  text += floatToStr(ship.maxHullPoints, 0);
  m_menus[HEALTH]->setText(text);

  text = "Power: ";
  text += floatToStr(std::floor(ship.powerPoints), 0);
  text += "/";
  text += floatToStr(std::floor(ship.maxPowerPoints), 0);
  m_menus[POWER]->setText(text);
}

void GameScreenUiHandler::updateTargetUi()
{
  m_menus[TARGET_HEALTH]->setVisible(m_shipView->hasTarget());
  m_menus[TARGET_POWER]->setVisible(m_shipView->hasTarget());

  if (!m_shipView->hasTarget())
  {
    return;
  }

  std::string text;

  text = "Health: ";
  text += floatToStr(std::floor(m_shipView->getTargetHealth()), 0);
  text += "/";
  text += floatToStr(std::floor(m_shipView->getTargetMaxHealth()), 0);
  m_menus[TARGET_HEALTH]->setText(text);

  text = "Power: ";
  text += floatToStr(std::floor(m_shipView->getTargetPower()), 0);
  text += "/";
  text += floatToStr(std::floor(m_shipView->getTargetMaxPower()), 0);
  m_menus[TARGET_POWER]->setText(text);
}

} // namespace pge
