
#pragma once

#include "ComputerRepository.hh"
#include "PlayerComputerRepository.hh"
#include "PlayerWeaponRepository.hh"
#include "UiMenu.hh"
#include "UiTextMenu.hh"
#include "WeaponRepository.hh"

namespace pge {

auto generateWeaponMenu(const bsgo::Weapon &weapon) -> UiMenuPtr;
auto generateWeaponMenu(const bsgo::PlayerWeapon &weapon) -> UiMenuPtr;
auto generateComputerMenu(const bsgo::Computer &computer) -> UiMenuPtr;
auto generateComputerMenu(const bsgo::PlayerComputer &computer) -> UiMenuPtr;

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
