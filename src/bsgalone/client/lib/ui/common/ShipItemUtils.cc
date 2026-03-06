
#include "ShipItemUtils.hh"
#include "ScreenCommon.hh"
#include "StringUtils.hh"
#include "TimeStepUtils.hh"
#include "TimeUtils.hh"

namespace bsgalone::client {

auto generateTextConfig(const std::string &name, const pge::Color &color, const int margin)
  -> ui::TextConfig
{
  return textConfigFromColor(name, color, ui::TextAlignment::LEFT, margin);
}

auto generateWeaponMenu(const std::string &name,
                        const float minDamage,
                        const float maxDamage,
                        const float range,
                        const chrono::TickDuration &reloadTime,
                        const chrono::TimeStep &step) -> ui::UiMenuPtr
{
  auto menu = generateBlankVerticalMenu();

  const ui::MenuConfig config{.highlightable = false};
  const auto bg = ui::bgConfigFromColor(pge::colors::BLANK);

  auto label = name + " (weapon)";
  auto text  = generateTextConfig(label, pge::colors::GREY, 10);
  auto prop  = std::make_unique<ui::UiTextMenu>(config, bg, text);
  menu->addMenu(std::move(prop));

  label = "Damage: " + core::floatToStr(minDamage, 2) + "-" + core::floatToStr(maxDamage) + "dmg";
  text  = generateTextConfig(label);
  prop  = std::make_unique<ui::UiTextMenu>(config, bg, text);
  menu->addMenu(std::move(prop));

  label = "Range: " + core::floatToStr(range, 0) + "m";
  text  = generateTextConfig(label);
  prop  = std::make_unique<ui::UiTextMenu>(config, bg, text);
  menu->addMenu(std::move(prop));

  const auto secondsToReload = ::core::toSeconds(convertTickToDuration(reloadTime, step));
  label                      = "Reload: " + core::floatToStr(secondsToReload, 2) + "s";
  text                       = generateTextConfig(label);
  prop                       = std::make_unique<ui::UiTextMenu>(config, bg, text);
  menu->addMenu(std::move(prop));

  return menu;
}

auto generateWeaponMenu(const core::WeaponData &weapon, const chrono::TimeStep &step)
  -> ui::UiMenuPtr
{
  return generateWeaponMenu(weapon.name,
                            weapon.minDamage,
                            weapon.maxDamage,
                            weapon.range,
                            weapon.reloadTime,
                            step);
}

auto generateWeaponMenu(const core::PlayerWeaponData &weapon, const chrono::TimeStep &step)
  -> ui::UiMenuPtr
{
  return generateWeaponMenu(weapon.name,
                            weapon.minDamage,
                            weapon.maxDamage,
                            weapon.range,
                            weapon.reloadTime,
                            step);
}

auto generateComputerMenu(const std::string &name,
                          const float powerCost,
                          const std::optional<float> &range,
                          const std::optional<chrono::TickDuration> &duration,
                          const chrono::TickDuration &reloadTime,
                          const chrono::TimeStep &step) -> ui::UiMenuPtr
{
  auto menu = generateBlankVerticalMenu();

  const ui::MenuConfig config{.highlightable = false};
  const auto bg = ui::bgConfigFromColor(pge::colors::BLANK);

  auto label = name + " (computer)";
  auto text  = generateTextConfig(label, pge::colors::GREY, 10);
  auto prop  = std::make_unique<ui::UiTextMenu>(config, bg, text);
  menu->addMenu(std::move(prop));

  label = "Power cost: " + core::floatToStr(powerCost, 0);
  text  = generateTextConfig(label);
  prop  = std::make_unique<ui::UiTextMenu>(config, bg, text);
  menu->addMenu(std::move(prop));

  if (range)
  {
    label = "Range: " + core::floatToStr(*range, 0) + "m";
    text  = generateTextConfig(label);
    prop  = std::make_unique<ui::UiTextMenu>(config, bg, text);
    menu->addMenu(std::move(prop));
  }

  if (duration)
  {
    const auto secondsDuration = ::core::toSeconds(convertTickToDuration(*duration, step));
    label                      = "Duration: " + core::floatToStr(secondsDuration, 2) + "s";
    text                       = generateTextConfig(label);
    prop                       = std::make_unique<ui::UiTextMenu>(config, bg, text);
    menu->addMenu(std::move(prop));
  }

  const auto secondsToReload = ::core::toSeconds(convertTickToDuration(reloadTime, step));
  label                      = "Reload: " + core::floatToStr(secondsToReload, 2) + "s";
  text                       = generateTextConfig(label);
  prop                       = std::make_unique<ui::UiTextMenu>(config, bg, text);
  menu->addMenu(std::move(prop));

  return menu;
}

auto generateComputerMenu(const core::ComputerData &computer, const chrono::TimeStep &step)
  -> ui::UiMenuPtr
{
  return generateComputerMenu(computer.name,
                              computer.powerCost,
                              computer.range,
                              computer.duration,
                              computer.reloadTime,
                              step);
}

auto generateComputerMenu(const core::PlayerComputerData &computer, const chrono::TimeStep &step)
  -> ui::UiMenuPtr
{
  return generateComputerMenu(computer.name,
                              computer.powerCost,
                              computer.range,
                              computer.duration,
                              computer.reloadTime,
                              step);
}

auto generateInteractiveSection(const std::string &buttonText,
                                const ui::ClickCallback &callback,
                                const HorizontalMargin horizontalMargin,
                                const VerticalMargin verticalMargin) -> InteractiveSection
{
  InteractiveSection section{};

  auto middleSection = generateBlankVerticalMenu();
  if (verticalMargin != VerticalMargin::BOTTOM)
  {
    middleSection->addMenu(generateSpacer());
  }

  const ui::MenuConfig config{.clickCallback = callback};

  const auto bg       = ui::bgConfigFromColor(pge::colors::VERY_DARK_GREEN);
  const auto textConf = ui::textConfigFromColor(buttonText, pge::colors::WHITE, pge::colors::WHITE);
  auto button         = std::make_unique<ui::UiTextMenu>(config, bg, textConf);
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

} // namespace bsgalone::client
