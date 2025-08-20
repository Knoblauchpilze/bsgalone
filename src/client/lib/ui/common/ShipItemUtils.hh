
#pragma once

#include "ComputerData.hh"
#include "PlayerComputerData.hh"
#include "PlayerWeaponData.hh"
#include "UiMenu.hh"
#include "UiTextMenu.hh"
#include "WeaponData.hh"

namespace pge {

constexpr auto DEFAULT_MARGIN = 30;
auto generateTextConfig(const std::string &name,
                        const Color &color = colors::WHITE,
                        const int margin   = DEFAULT_MARGIN) -> TextConfig;

auto generateWeaponMenu(const bsgo::WeaponData &weapon) -> UiMenuPtr;
auto generateWeaponMenu(const bsgo::PlayerWeaponData &weapon) -> UiMenuPtr;
auto generateComputerMenu(const bsgo::ComputerData &computer) -> UiMenuPtr;
auto generateComputerMenu(const bsgo::PlayerComputerData &computer) -> UiMenuPtr;

struct InteractiveSection
{
  UiMenuPtr menu{};
  UiTextMenu *button{};
};

enum class HorizontalMargin
{
  LEFT,
  RIGHT,
  BOTH
};

enum class VerticalMargin
{
  TOP,
  BOTTOM,
  BOTH
};

auto generateInteractiveSection(const std::string &buttonText,
                                const ClickCallback &callback,
                                const HorizontalMargin horizontalMargin = HorizontalMargin::BOTH,
                                const VerticalMargin verticalMargin     = VerticalMargin::BOTH)
  -> InteractiveSection;

} // namespace pge
