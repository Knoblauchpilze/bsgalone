
#include "WeaponsUiHandler.hh"
#include "GameColorUtils.hh"
#include "MessageListenerWrapper.hh"
#include "MessageUtils.hh"
#include "ScreenCommon.hh"
#include "SlotComponentUtils.hh"
#include "StringUtils.hh"
#include "UiPictureMenu.hh"

namespace pge {

WeaponsUiHandler::WeaponsUiHandler(const bsgo::Views &views)
  : IUiHandler("weapons")
  , AbstractMessageListener({bsgo::MessageType::ENTITY_REMOVED})
  , m_shipView(views.shipView)
  , m_shipDbView(views.shipDbView)
  , m_playerView(views.playerView)
{
  if (nullptr == m_shipView)
  {
    throw std::invalid_argument("Expected non null ship view");
  }
  if (nullptr == m_shipDbView)
  {
    throw std::invalid_argument("Expected non null ship db view");
  }
  if (nullptr == m_playerView)
  {
    throw std::invalid_argument("Expected non null player view");
  }
}

void WeaponsUiHandler::initializeMenus(const int width,
                                       const int height,
                                       sprites::TexturePack &texturesLoader)
{
  constexpr auto WEAPONS_TEXTURE_PACK_FILE_PATH = "assets/weapons.png";
  const sprites::PackDesc pack{.file   = WEAPONS_TEXTURE_PACK_FILE_PATH,
                               .sSize  = Vec2i{44, 92},
                               .layout = Vec2i{3, 1}};
  m_weaponTexturesPackId = texturesLoader.registerPack(pack);

  generateWeaponsMenus(width, height);
}

bool WeaponsUiHandler::processUserInput(UserInputData &inputData)
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

void WeaponsUiHandler::render(Renderer &engine) const
{
  for (const auto &menu : m_weapons)
  {
    menu->render(engine);
  }
}

void WeaponsUiHandler::updateUi()
{
  if (!m_shipView->isReady() || !m_playerView->isReady())
  {
    return;
  }
  if (!m_initialized)
  {
    initializeWeapons();
  }

  const auto ship = m_shipView->getPlayerShip();

  auto id = 0;
  for (const auto &weapon : ship.weapons)
  {
    updateWeaponMenu(*weapon, id);
    ++id;
  }
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

void WeaponsUiHandler::connectToMessageQueue(bsgo::IMessageQueue &messageQueue)
{
  auto listener = std::make_unique<MessageListenerWrapper>(this);
  messageQueue.addListener(std::move(listener));
}

void WeaponsUiHandler::onMessageReceived(const bsgo::IMessage &message)
{
  if (!m_shipView->isReady())
  {
    return;
  }

  m_disabled = didPlayerShipDied(message.as<bsgo::EntityRemovedMessage>(), *m_shipDbView);
}

namespace {
constexpr auto NUMBER_OF_WEAPONS           = 6;
constexpr auto ABILITIES_PICTURE_FILE_PATH = "assets/slot.png";
} // namespace

void WeaponsUiHandler::generateWeaponsMenus(int width, int height)
{
  constexpr auto WEAPON_MENU_PIXEL_DIMS = 50;
  const Vec2i weaponMenuDims{WEAPON_MENU_PIXEL_DIMS, WEAPON_MENU_PIXEL_DIMS};
  constexpr auto SPACING_IN_PIXELS = 5;
  const Vec2i pos{width - NUMBER_OF_WEAPONS * (weaponMenuDims.x + SPACING_IN_PIXELS), height / 2};

  MenuConfig config{.pos                       = pos,
                    .dims                      = weaponMenuDims,
                    .propagateEventsToChildren = false,
                    .customRenderMode          = CustomRenderMode::PRE_RENDER};
  const PictureConfig bg{.path = ABILITIES_PICTURE_FILE_PATH};

  for (auto id = 0u; id < NUMBER_OF_WEAPONS; ++id)
  {
    auto menu = std::make_unique<UiPictureMenu>(config, bg);
    m_weapons.push_back(std::move(menu));

    config.pos.x += (weaponMenuDims.x + SPACING_IN_PIXELS);
  }
}

namespace {
auto tryGetDbWeapon(const bsgo::Uuid weaponDbId, const std::vector<bsgo::PlayerWeapon> &weapons)
  -> std::optional<bsgo::PlayerWeapon>
{
  for (const auto &weapon : weapons)
  {
    if (weapon.id == weaponDbId)
    {
      return weapon;
    }
  }

  return {};
}

auto spriteIdFromWeapon(const bsgo::PlayerWeapon &weapon) -> Vec2i
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
  const auto ship         = m_shipView->getPlayerShip();
  const auto dbWeapons    = m_shipDbView->getPlayerShipWeapons();
  const auto weaponsCount = ship.weapons.size();
  if (weaponsCount > NUMBER_OF_WEAPONS)
  {
    error("Failed to adapt UI for ship " + ship.str(),
          "Expected maximum " + std::to_string(NUMBER_OF_WEAPONS) + " weapon(s), got "
            + std::to_string(ship.weapons.size()));
  }

  const auto palette = generatePaletteForFaction(m_playerView->getPlayerFaction());

  const MenuConfig config{};
  const auto bg = bgConfigFromColor(colors::BLANK);
  auto textConf = textConfigFromColor("", colors::WHITE);

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
      error("Failed to initialize weapons for ship " + bsgo::str(ship.dbComp().dbId()),
            "Failed to find component for weapon " + bsgo::str(weaponDbId));
    }

    menu->setClickCallback([this, id]() {
      if (!m_shipView->isReady())
      {
        return;
      }
      m_shipView->tryActivateWeapon(id);
    });
    menu->setEnabled(true);

    menu->setSprite(m_weaponTexturesPackId, spriteIdFromWeapon(*dbWeapon));

    auto prop = std::make_unique<UiTextMenu>(config, bg, textConf);
    m_statuses.push_back(prop.get());
    menu->addMenu(std::move(prop));

    ++id;
  }

  m_initialized = true;
}

void WeaponsUiHandler::updateWeaponMenu(const bsgo::WeaponSlotComponent &weapon, const int id)
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
    statusText = bsgo::floatToStr(PERCENTAGE_MULTIPLIER * weapon.reloadPercentage()) + "%";
  }
  m_statuses[id]->setText(statusText);
}

} // namespace pge
