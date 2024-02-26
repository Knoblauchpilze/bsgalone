
#include "ShipItemUtils.hh"
#include "ScreenCommon.hh"
#include "StringUtils.hh"
#include <core_utils/TimeUtils.hh>

namespace pge {

constexpr auto DEFAULT_MARGIN           = 30;
constexpr auto MILLISECONDS_IN_A_SECOND = 1000.0f;

auto durationToSeconds(const utils::Duration &duration) -> float
{
  return utils::toMilliseconds(duration) / MILLISECONDS_IN_A_SECOND;
}

auto generateTextConfig(const std::string &name,
                        const Color &color = colors::WHITE,
                        const int margin   = DEFAULT_MARGIN) -> TextConfig
{
  return textConfigFromColor(name, color, TextAlignment::LEFT, margin);
}

auto generateWeaponMenu(const std::string &name,
                        const float minDamage,
                        const float maxDamage,
                        const float range,
                        const utils::Duration reloadTime) -> UiMenuPtr
{
  auto menu = generateBlankVerticalMenu();

  const MenuConfig config{.highlightable = false};
  const auto bg = bgConfigFromColor(colors::BLANK);

  auto label = name + " (weapon)";
  auto text  = generateTextConfig(label, colors::GREY, 10);
  auto prop  = std::make_unique<UiTextMenu>(config, bg, text);
  menu->addMenu(std::move(prop));

  label = "Damage: " + bsgo::floatToStr(minDamage, 2) + "-" + bsgo::floatToStr(maxDamage) + "dmg";
  text  = generateTextConfig(label);
  prop  = std::make_unique<UiTextMenu>(config, bg, text);
  menu->addMenu(std::move(prop));

  label = "Range: " + bsgo::floatToStr(range, 0) + "m";
  text  = generateTextConfig(label);
  prop  = std::make_unique<UiTextMenu>(config, bg, text);
  menu->addMenu(std::move(prop));

  const auto secondsToReload = durationToSeconds(reloadTime);
  label                      = "Reload: " + bsgo::floatToStr(secondsToReload, 2) + "s";
  text                       = generateTextConfig(label);
  prop                       = std::make_unique<UiTextMenu>(config, bg, text);
  menu->addMenu(std::move(prop));

  return menu;
}

auto generateWeaponMenu(const bsgo::Weapon &weapon) -> UiMenuPtr
{
  return generateWeaponMenu(weapon.name,
                            weapon.minDamage,
                            weapon.maxDamage,
                            weapon.range,
                            weapon.reloadTime);
}

auto generateWeaponMenu(const bsgo::PlayerWeapon &weapon) -> UiMenuPtr
{
  return generateWeaponMenu(weapon.name,
                            weapon.minDamage,
                            weapon.maxDamage,
                            weapon.range,
                            weapon.reloadTime);
}

auto generateComputerMenu(const bsgo::PlayerComputer &computer) -> UiMenuPtr
{
  auto menu = generateBlankVerticalMenu();

  const MenuConfig config{.highlightable = false};
  const auto bg = bgConfigFromColor(colors::BLANK);

  auto label = computer.name + " (computer)";
  auto text  = generateTextConfig(label, colors::GREY, 10);
  auto prop  = std::make_unique<UiTextMenu>(config, bg, text);
  menu->addMenu(std::move(prop));

  label = "Power cost: " + bsgo::floatToStr(computer.powerCost, 0);
  text  = generateTextConfig(label);
  prop  = std::make_unique<UiTextMenu>(config, bg, text);
  menu->addMenu(std::move(prop));

  if (computer.range)
  {
    label = "Range: " + bsgo::floatToStr(*computer.range, 0) + "m";
    text  = generateTextConfig(label);
    prop  = std::make_unique<UiTextMenu>(config, bg, text);
    menu->addMenu(std::move(prop));
  }

  if (computer.duration)
  {
    const auto secondsToReload = durationToSeconds(*computer.duration);
    label                      = "Duration: " + bsgo::floatToStr(secondsToReload, 2) + "s";
    text                       = generateTextConfig(label);
    prop                       = std::make_unique<UiTextMenu>(config, bg, text);
    menu->addMenu(std::move(prop));
  }

  constexpr auto MILLISECONDS_IN_A_SECOND = 1000.0f;
  const auto secondsToReload              = utils::toMilliseconds(computer.reloadTime)
                               / MILLISECONDS_IN_A_SECOND;
  label = "Reload: " + bsgo::floatToStr(secondsToReload, 2) + "s";
  text  = generateTextConfig(label);
  prop  = std::make_unique<UiTextMenu>(config, bg, text);
  menu->addMenu(std::move(prop));

  return menu;
}

auto generateInteractiveSection(const std::string &buttonText,
                                const ClickCallback &callback,
                                const HorizontalMargin horizontalMargin,
                                const VerticalMargin verticalMargin) -> InteractiveSection
{
  InteractiveSection section{};

  auto middleSection = generateBlankVerticalMenu();
  if (verticalMargin != VerticalMargin::BOTTOM)
  {
    middleSection->addMenu(generateSpacer());
  }

  const MenuConfig config{.clickCallback = callback};

  const auto bg       = bgConfigFromColor(colors::VERY_DARK_GREEN);
  const auto textConf = textConfigFromColor(buttonText, colors::WHITE, colors::WHITE);
  auto button         = std::make_unique<UiTextMenu>(config, bg, textConf);
  section.button      = button.get();

  middleSection->addMenu(std::move(button));

  if (verticalMargin != VerticalMargin::TOP)
  {
    middleSection->addMenu(generateSpacer());
  }

  section.menu = generateBlankHorizontalMenu();
  if (horizontalMargin != HorizontalMargin::RIGHT)
  {
    section.menu->addMenu(generateSpacer());
  }
  section.menu->addMenu(std::move(middleSection));
  if (horizontalMargin != HorizontalMargin::LEFT)
  {
    section.menu->addMenu(generateSpacer());
  }

  return section;
}

} // namespace pge
