
#include "WeaponsUiHandler.hh"
#include "GameColorUtils.hh"
#include "IViewListenerProxy.hh"
#include "MessageListenerWrapper.hh"
#include "MessageUtils.hh"
#include "ScreenCommon.hh"
#include "SlotComponentUtils.hh"
#include "StringUtils.hh"
#include "UiPictureMenu.hh"

namespace bsgalone::client {

WeaponsUiHandler::WeaponsUiHandler(const Views &views)
  : AbstractUiHandler("weapons")
  , AbstractMessageListener({core::MessageType::ENTITY_REMOVED})
  , m_gameView(views.gameView)
{
  if (nullptr == m_gameView)
  {
    throw std::invalid_argument("Expected non null game view");
  }

  subscribeToViews();
}

void WeaponsUiHandler::initializeMenus(const int width,
                                       const int height,
                                       pge::sprites::TexturePack &texturesLoader)
{
  constexpr auto WEAPONS_TEXTURE_PACK_FILE_PATH = "assets/weapons.png";
  const pge::sprites::PackDesc pack{.file   = WEAPONS_TEXTURE_PACK_FILE_PATH,
                                    .sSize  = pge::Vec2i{44, 92},
                                    .layout = pge::Vec2i{3, 1}};
  m_weaponTexturesPackId = texturesLoader.registerPack(pack);

  generateWeaponsMenus(width, height);
}

bool WeaponsUiHandler::processUserInput(ui::UserInputData &inputData)
{
  if (m_disabled)
  {
    return false;
  }

  auto out{false};
  for (const auto &menu : m_weapons)
  {
    out |= menu->processUserInput(inputData);
  }

  return out;
}

void WeaponsUiHandler::render(pge::Renderer &engine) const
{
  for (const auto &menu : m_weapons)
  {
    menu->render(engine);
  }
}

void WeaponsUiHandler::updateUi()
{
  if (!m_gameView->isReady())
  {
    return;
  }
  if (!m_initialized)
  {
    initializeWeapons();
  }

  const auto ship = m_gameView->getPlayerShip();

  auto id = 0;
  for (const auto &weapon : ship.weapons)
  {
    updateWeaponMenu(*weapon, id);
    ++id;
  }
}

void WeaponsUiHandler::connectToMessageQueue(core::IMessageQueue &messageQueue)
{
  auto listener = std::make_unique<MessageListenerWrapper>(this);
  messageQueue.addListener(std::move(listener));
}

void WeaponsUiHandler::onEventReceived(const core::IMessage &message)
{
  if (!m_gameView->isReady())
  {
    return;
  }

  m_disabled = didPlayerShipDie(message.as<core::EntityRemovedMessage>(), *m_gameView);
}

void WeaponsUiHandler::subscribeToViews()
{
  auto consumer = [this]() { reset(); };

  auto listener = std::make_unique<IViewListenerProxy>(consumer);
  m_gameView->addListener(std::move(listener));
}

void WeaponsUiHandler::reset()
{
  m_statuses.clear();

  for (auto &weapon : m_weapons)
  {
    weapon->clearChildren();
  }

  m_initialized = false;
  m_disabled    = false;
}

namespace {
constexpr auto NUMBER_OF_WEAPONS           = 6;
constexpr auto ABILITIES_PICTURE_FILE_PATH = "assets/slot.png";
} // namespace

void WeaponsUiHandler::generateWeaponsMenus(int width, int height)
{
  constexpr auto WEAPON_MENU_PIXEL_DIMS = 50;
  const pge::Vec2i weaponMenuDims{WEAPON_MENU_PIXEL_DIMS, WEAPON_MENU_PIXEL_DIMS};
  constexpr auto SPACING_IN_PIXELS = 5;
  const pge::Vec2i pos{width - NUMBER_OF_WEAPONS * (weaponMenuDims.x + SPACING_IN_PIXELS),
                       height / 2};

  ui::MenuConfig config{.pos                       = pos,
                        .dims                      = weaponMenuDims,
                        .propagateEventsToChildren = false,
                        .customRenderMode          = ui::CustomRenderMode::PRE_RENDER};
  const ui::PictureConfig bg{.path = ABILITIES_PICTURE_FILE_PATH};

  for (auto id = 0u; id < NUMBER_OF_WEAPONS; ++id)
  {
    auto menu = std::make_unique<ui::UiPictureMenu>(config, bg);
    m_weapons.push_back(std::move(menu));

    config.pos.x += (weaponMenuDims.x + SPACING_IN_PIXELS);
  }
}

namespace {
auto tryGetDbWeapon(const core::Uuid weaponDbId, const std::vector<core::PlayerWeaponData> &weapons)
  -> std::optional<core::PlayerWeaponData>
{
  for (const auto &weapon : weapons)
  {
    if (weapon.dbId == weaponDbId)
    {
      return weapon;
    }
  }

  return {};
}

auto spriteIdFromWeapon(const core::PlayerWeaponData &weapon) -> pge::Vec2i
{
  if (weapon.name == "Short range cannon")
  {
    return {0, 0};
  }

  if (weapon.name == "Medium range cannon")
  {
    return {1, 0};
  }

  if (weapon.name == "Long range cannon")
  {
    return {2, 0};
  }

  throw std::invalid_argument("Unsupported weapon " + weapon.name);
}
} // namespace

void WeaponsUiHandler::initializeWeapons()
{
  const auto ship         = m_gameView->getPlayerShip();
  const auto dbWeapons    = m_gameView->getPlayerShipWeapons();
  const auto weaponsCount = ship.weapons.size();
  if (weaponsCount > NUMBER_OF_WEAPONS)
  {
    error("Failed to adapt UI for ship " + ship.str(),
          "Expected maximum " + std::to_string(NUMBER_OF_WEAPONS) + " weapon(s), got "
            + std::to_string(ship.weapons.size()));
  }

  const auto palette = generatePaletteForFaction(m_gameView->getPlayerFaction());

  const ui::MenuConfig config{};
  const auto bg = ui::bgConfigFromColor(pge::colors::BLANK);
  auto textConf = ui::textConfigFromColor("", pge::colors::WHITE);

  for (const auto &menu : m_weapons)
  {
    resetPictureMenuToDefault(*menu, palette.defaultColor);
  }

  auto id = 0;
  for (const auto &weapon : ship.weapons)
  {
    auto &menu            = m_weapons[id];
    const auto weaponDbId = weapon->dbId();
    const auto dbWeapon   = tryGetDbWeapon(weaponDbId, dbWeapons);
    if (!dbWeapon)
    {
      error("Failed to initialize weapons for ship " + core::str(ship.dbComp().dbId()),
            "Failed to find component for weapon " + core::str(weaponDbId));
    }

    menu->setClickCallback([this, id]() {
      if (!m_gameView->isReady())
      {
        return;
      }
      m_gameView->tryActivateWeapon(id);
    });
    menu->setEnabled(true);

    menu->setSprite(m_weaponTexturesPackId, spriteIdFromWeapon(*dbWeapon));

    auto prop = std::make_unique<ui::UiTextMenu>(config, bg, textConf);
    m_statuses.push_back(prop.get());
    menu->addMenu(std::move(prop));

    ++id;
  }

  m_initialized = true;
}

void WeaponsUiHandler::updateWeaponMenu(const core::WeaponSlotComponent &weapon, const int id)
{
  auto &menu = *m_weapons[id];

  auto bgColor = semiOpaque(bgColorFromFiringState(weapon));
  menu.updateBgColor(bgColor);

  std::string statusText("ready");
  const auto percentage = weapon.reloadPercentage();

  if (!weapon.active())
  {
    statusText = "off";
  }
  else if (percentage < 1.0f)
  {
    constexpr auto PERCENTAGE_MULTIPLIER = 100.0f;
    statusText = core::floatToStr(PERCENTAGE_MULTIPLIER * weapon.reloadPercentage()) + "%";
  }
  m_statuses[id]->setText(statusText);
}

} // namespace bsgalone::client
