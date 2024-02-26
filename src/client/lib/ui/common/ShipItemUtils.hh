
#pragma once

#include "PlayerComputerRepository.hh"
#include "PlayerWeaponRepository.hh"
#include "UiMenu.hh"

namespace pge {

auto generateWeaponMenu(const bsgo::PlayerWeapon &weapon) -> UiMenuPtr;
auto generateComputerMenu(const bsgo::PlayerComputer &computer) -> UiMenuPtr;

struct InteractiveSection
{
  UiMenuPtr menu{};
  UiMenu *button{};
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
