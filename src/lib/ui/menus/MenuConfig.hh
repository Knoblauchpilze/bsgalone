
#pragma once

#include "Action.hh"
#include "olcEngine.hh"
#include <optional>

namespace pge {

enum class MenuLayout
{
  Horizontal,
  Vertical
};

using ClickCallback     = std::function<void()>;
using HighlightCallback = std::function<void()>;
using LostFocusCallback = std::function<void()>;

struct MenuConfig
{
  olc::vi2d pos{};
  olc::vi2d dims{10, 10};

  MenuLayout layout{MenuLayout::Vertical};

  bool visible{true};
  bool highlightable{true};
  bool propagateEventsToChildren{true};
  bool expandChildrenToFit{true};

  std::optional<HighlightCallback> highlightCallback{};
  std::optional<ClickCallback> clickCallback{};
  std::optional<LostFocusCallback> lostFocusCallback{};
  std::optional<GameCallback> gameClickCallback{};
};

} // namespace pge
