
#include "GameScreenUiHandler.hh"
#include "ScreenCommon.hh"
#include "StringUtils.hh"

namespace pge {

GameScreenUiHandler::GameScreenUiHandler(const bsgo::Views &views)
  : IUiHandler("game")
  , m_shipView(views.shipView)
  , m_targetView(views.targetView)
  , m_weaponsUi(std::make_unique<WeaponsUiHandler>(views))
{
  if (nullptr == m_shipView)
  {
    throw std::invalid_argument("Expected non null ship view");
  }
  if (nullptr == m_targetView)
  {
    throw std::invalid_argument("Expected non null target view");
  }
}

void GameScreenUiHandler::initializeMenus(const int width, const int height)
{
  m_menus.resize(MenuItem::COUNT);

  generateShipMenus(width, height);
  generateAbilityMenus(width, height);
  generateTargetMenus(width, height);
  generateOutpostMenus(width, height);

  m_weaponsUi->initializeMenus(width, height);
}

auto GameScreenUiHandler::processUserInput(const controls::State &c,
                                           std::vector<ActionShPtr> &actions) -> menu::InputHandle
{
  auto out = m_weaponsUi->processUserInput(c, actions);
  if (!out.relevant && !out.selected)
  {
    for (const auto &menu : m_menus)
    {
      const auto ih = menu->processUserInput(c, actions);
      out.relevant  = (out.relevant || ih.relevant);
      out.selected  = (out.selected || ih.selected);
    }
  }

  return out;
}

void GameScreenUiHandler::render(SpriteRenderer &engine) const
{
  for (const auto &menu : m_menus)
  {
    menu->render(engine.getRenderer());
  }

  m_weaponsUi->render(engine);
}

void GameScreenUiHandler::updateUi()
{
  updateShipUi();
  updateTargetUi();
  updateOutpostUi();

  m_weaponsUi->updateUi();
}

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

void GameScreenUiHandler::generateTargetMenus(int width, int /*height*/)
{
  olc::vi2d pos{width / 2, 5};
  olc::vi2d dims{200, 15};
  m_menus[TARGET_HEALTH]
    = generateMenu(pos, dims, "Health: N/A", "health", olc::VERY_DARK_RED, {olc::WHITE});

  constexpr auto REASONABLE_GAP = 15;
  pos.y += REASONABLE_GAP;
  m_menus[TARGET_POWER]
    = generateMenu(pos, dims, "Power: N/A", "power", olc::DARK_CYAN, {olc::WHITE});

  pos.y += REASONABLE_GAP;
  m_menus[TARGET_DISTANCE] = generateMenu(pos,
                                          dims,
                                          "N/A m",
                                          "distance",
                                          olc::Pixel(0, 0, 0, alpha::Transparent),
                                          {olc::WHITE});
}

void GameScreenUiHandler::generateOutpostMenus(int width, int /*height*/)
{
  olc::vi2d pos{width / 2, 55};
  olc::vi2d dims{100, 25};

  m_menus[DOCK] = generateMenu(pos, dims, "Dock", "dock", olc::VERY_DARK_GREY, {olc::WHITE}, true);
  m_menus[DOCK]->setSimpleAction([](Game &g) { g.setScreen(Screen::OUTPOST); });
}

void GameScreenUiHandler::updateShipUi()
{
  std::string text;

  const auto ship = m_shipView->getPlayerShip();

  text = "Health: ";
  text += floatToStr(std::floor(ship.access<bsgo::Health>().value()), 0);
  text += "/";
  text += floatToStr(ship.access<bsgo::Health>().max(), 0);
  m_menus[HEALTH]->setText(text);

  text = "Power: ";
  text += floatToStr(std::floor(ship.access<bsgo::Power>().value()), 0);
  text += "/";
  text += floatToStr(std::floor(ship.access<bsgo::Power>().max()), 0);
  m_menus[POWER]->setText(text);
}

void GameScreenUiHandler::updateTargetUi()
{
  const auto target = m_targetView->getTarget();
  m_menus[TARGET_HEALTH]->setVisible(target.has_value());
  m_menus[TARGET_POWER]->setVisible(target.has_value());
  m_menus[TARGET_DISTANCE]->setVisible(target.has_value());

  if (!target)
  {
    return;
  }

  std::string text;

  text = "Health: ";
  if (!target->exists<bsgo::Health>())
  {
    text += "N/A";
  }
  else
  {
    text += floatToStr(std::floor(target->access<bsgo::Health>().value()), 0);
    text += "/";
    text += floatToStr(std::floor(target->access<bsgo::Health>().max()), 0);
  }
  m_menus[TARGET_HEALTH]->setText(text);

  text = "Power: ";
  if (!target->exists<bsgo::Power>())
  {
    text += "N/A";
  }
  else
  {
    text += floatToStr(std::floor(target->access<bsgo::Power>().value()), 0);
    text += "/";
    text += floatToStr(std::floor(target->access<bsgo::Power>().max()), 0);
  }
  m_menus[TARGET_POWER]->setText(text);

  const auto d = m_targetView->distanceToTarget();
  text         = floatToStr(d, 1) + "m";
  m_menus[TARGET_DISTANCE]->setText(text);
}

void GameScreenUiHandler::updateOutpostUi()
{
  auto dockButtonVisible{false};

  const auto target                       = m_targetView->getTarget();
  constexpr auto MAXIMUM_DISTANCE_TO_DOCK = 5.0f;
  if (target && bsgo::EntityKind::OUTPOST == target->kind)
  {
    dockButtonVisible = m_targetView->distanceToTarget() <= MAXIMUM_DISTANCE_TO_DOCK;
  }

  m_menus[DOCK]->setVisible(dockButtonVisible);
}

} // namespace pge
