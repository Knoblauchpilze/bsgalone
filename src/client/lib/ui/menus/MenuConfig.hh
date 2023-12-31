
#pragma once

#include "UserInputData.hh"
#include "Vector2d.hh"
#include <functional>
#include <optional>

namespace pge {

enum class MenuLayout
{
  HORIZONTAL,
  VERTICAL
};

using ClickCallback     = std::function<void()>;
using HighlightCallback = std::function<void()>;
using LostFocusCallback = std::function<void()>;

struct MenuConfig
{
  Vec2i pos{};
  Vec2i dims{10, 10};

  MenuLayout layout{MenuLayout::VERTICAL};

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
