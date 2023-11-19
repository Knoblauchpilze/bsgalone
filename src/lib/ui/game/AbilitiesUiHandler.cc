
#include "AbilitiesUiHandler.hh"
#include "ScreenCommon.hh"
#include "SlotComponentUtils.hh"
#include "StringUtils.hh"

namespace pge {

AbilitiesUiHandler::AbilitiesUiHandler(const bsgo::Views &views)
  : IUiHandler("abilities")
  , m_shipView(views.shipView)
{
  if (nullptr == m_shipView)
  {
    throw std::invalid_argument("Expected non null ship view");
  }
}

void AbilitiesUiHandler::initializeMenus(const int width, const int height)
{
  generateComputersMenus(width, height);
}

bool AbilitiesUiHandler::processUserInput(UserInputData &inputData)
{
  auto relevant{false};

  for (const auto &menu : m_computers)
  {
    const auto ih = menu->processUserInput(inputData.controls, inputData.actions);
    relevant      = (relevant || ih.relevant);
  }

  return relevant;
}

void AbilitiesUiHandler::render(SpriteRenderer &engine) const
{
  for (const auto &menu : m_computers)
  {
    menu->render(engine.getRenderer());
  }
}

void AbilitiesUiHandler::updateUi()
{
  if (!m_shipView->isReady())
  {
    return;
  }
  if (!m_initialized)
  {
    initializeAbilities();
  }

  const auto ship = m_shipView->getPlayerShip();

  auto id = 0;
  for (const auto &computer : ship.computers)
  {
    updateComputerMenu(*computer, id);
    ++id;
  }
}

constexpr auto NUMBER_OF_ABILITIES = 4;

void AbilitiesUiHandler::generateComputersMenus(int width, int height)
{
  olc::vi2d dims{70, 50};
  constexpr auto SPACING_IN_PIXELS = 5;
  olc::vi2d pos;
  pos.x = width - NUMBER_OF_ABILITIES * (dims.x + SPACING_IN_PIXELS);
  pos.y = height - SPACING_IN_PIXELS - dims.y;
  const olc::Pixel transparentBg{0, 0, 0, alpha::SemiOpaque};

  for (auto id = 0u; id < NUMBER_OF_ABILITIES; ++id)
  {
    const auto name = "ability_" + std::to_string(id);
    auto menu       = generateSlotMenu(pos, dims, "", name, transparentBg, {olc::WHITE}, false);
    m_computers.push_back(menu);

    pos.x += (dims.x + SPACING_IN_PIXELS);
  }
}

void AbilitiesUiHandler::initializeAbilities()
{
  const auto ship           = m_shipView->getPlayerShip();
  const auto shipId         = ship.uuid;
  const auto computersCount = ship.computers.size();
  if (computersCount > NUMBER_OF_ABILITIES)
  {
    error("Failed to adapt UI for ship " + ship.str(),
          "Expected maximum " + std::to_string(NUMBER_OF_ABILITIES) + " computer(s), got "
            + std::to_string(ship.computers.size()));
  }

  const olc::vi2d dummyPos{0, 0};
  const olc::vi2d dummyDims{10, 10};
  const olc::Pixel transparentBg{0, 0, 0, alpha::SemiOpaque};
  for (auto id = 0u; id < computersCount; ++id)
  {
    auto &menu = m_computers[id];

    menu->setSimpleAction([shipId, id](Game &g) { g.tryActivateSlot(shipId, id); });
    menu->setEnabled(true);

    MenuShPtr range;
    if (ship.computers[id]->maybeRange())
    {
      range = generateMenu(dummyPos, dummyDims, "range", "range", transparentBg, {olc::WHITE});
      menu->addMenu(range);
    }
    m_ranges.push_back(range);

    MenuShPtr damage;
    if (ship.computers[id]->damageModifier())
    {
      damage = generateMenu(dummyPos, dummyDims, "damage", "damage", transparentBg, {olc::WHITE});
      menu->addMenu(damage);
    }
    m_damages.push_back(damage);

    const auto status
      = generateMenu(dummyPos, dummyDims, "status", "status", transparentBg, {olc::WHITE});
    menu->addMenu(status);
    m_statuses.push_back(status);
  }

  m_initialized = true;
}

void AbilitiesUiHandler::updateComputerMenu(const bsgo::ComputerSlotComponent &computer,
                                            const int id)
{
  auto &menu = *m_computers[id];

  auto bgColor = bgColorFromFiringState(computer);
  bgColor.a    = alpha::SemiOpaque;
  menu.setBackgroundColor(bgColor);

  const auto range = computer.maybeRange();
  if (range)
  {
    m_ranges[id]->setText(floatToStr(*range, 0) + "m");
  }

  const auto damage = computer.damageModifier();
  if (damage)
  {
    constexpr auto PERCENTAGE_MULTIPLIER = 100.0f;
    const auto multiplier                = PERCENTAGE_MULTIPLIER * (*damage - 1.0f);
    m_damages[id]->setText("Dmg:" + floatToSignedStr(multiplier, 0) + "%");
  }

  std::string statusText("ready");
  if (!computer.canFire())
  {
    constexpr auto PERCENTAGE_MULTIPLIER = 100.0f;
    statusText = floatToStr(PERCENTAGE_MULTIPLIER * computer.reloadPercentage()) + "%";
  }
  m_statuses[id]->setText(statusText);
}

} // namespace pge
