
#include "GameRenderer.hh"
#include "ScreenCommon.hh"

namespace pge {

GameRenderer::GameRenderer(const bsgo::Views & /*views*/, int width, int height)
{
  create(width, height);
}

void GameRenderer::render(SpriteRenderer &engine,
                          const RenderState &state,
                          const RenderingPass pass) const
{
  switch (pass)
  {
    case RenderingPass::DECAL:
      renderDecal(engine, state);
      break;
    case RenderingPass::UI:
      renderUi(engine, state);
      break;
    default:
      break;
  }
}

auto GameRenderer::processUserInput(const controls::State &c, std::vector<ActionShPtr> &actions)
  -> menu::InputHandle
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

void GameRenderer::updateUi() {}

void GameRenderer::create(int width, int height)
{
  m_menus.resize(MenuItem::COUNT);

  olc::vi2d pos{5, 5};
  olc::vi2d dims{200, 15};
  m_menus[HEALTH]
    = generateMenu(pos, dims, "Health: N/A", "health", olc::VERY_DARK_RED, {olc::WHITE});

  pos            = olc::vi2d{5, 25};
  m_menus[POWER] = generateMenu(pos, dims, "Power: N/A", "power", olc::DARK_CYAN, {olc::WHITE});

  generateAbilityMenus(width, height);
  generateWeaponMenus(width, height);
}

void GameRenderer::generateAbilityMenus(int width, int height)
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

void GameRenderer::generateWeaponMenus(int width, int height)
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

void GameRenderer::renderDecal(SpriteRenderer & /*engine*/
                               ,
                               const RenderState & /*state*/) const
{}

void GameRenderer::renderUi(SpriteRenderer &engine, const RenderState & /*state*/) const
{
  for (const auto &menu : m_menus)
  {
    menu->render(engine.getRenderer());
  }
}

} // namespace pge
