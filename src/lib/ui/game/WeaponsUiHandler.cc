
#include "WeaponsUiHandler.hh"
#include "ScreenCommon.hh"
#include "StringUtils.hh"

namespace pge {

WeaponsUiHandler::WeaponsUiHandler(const bsgo::Views &views)
  : IUiHandler("weapons")
  , m_shipView(views.shipView)
  , m_targetView(views.targetView)
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

void WeaponsUiHandler::initializeMenus(const int width, const int height)
{
  generateWeaponMenus(width, height);
}

auto WeaponsUiHandler::processUserInput(const controls::State &c, std::vector<ActionShPtr> &actions)
  -> menu::InputHandle
{
  auto relevant{false};
  auto selected{false};

  for (const auto &menu : m_menus)
  {
    const auto ih = menu->processUserInput(c, actions);
    relevant      = (relevant || ih.relevant);
    selected      = (selected || ih.selected);
  }

  return menu::InputHandle{.relevant = relevant, .selected = selected};
}

void WeaponsUiHandler::render(SpriteRenderer &engine) const
{
  for (const auto &menu : m_menus)
  {
    menu->render(engine.getRenderer());
  }
}

void WeaponsUiHandler::updateUi() {}

constexpr auto WEAPONS_COUNT = 4;

void WeaponsUiHandler::generateWeaponMenus(int width, int height)
{
  olc::vi2d dims{50, 50};
  constexpr auto SPACING_IN_PIXELS = 5;
  olc::vi2d pos;
  pos.x = width - WEAPONS_COUNT * (dims.x + SPACING_IN_PIXELS);
  pos.y = height / 2;

  olc::Pixel color = olc::VERY_DARK_RED;
  color.a          = alpha::SemiOpaque;

  for (auto id = 0; id < WEAPONS_COUNT; ++id)
  {
    const auto text = "W" + std::to_string(id);
    const auto name = "weapon_" + std::to_string(id);

    auto menu = generateMenu(pos, dims, text, name, color, {olc::WHITE}, true);
    m_menus.push_back(menu);

    pos.x += (dims.x + SPACING_IN_PIXELS);
  }
}

} // namespace pge
