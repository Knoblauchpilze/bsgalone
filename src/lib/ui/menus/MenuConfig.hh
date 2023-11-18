
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

struct MenuConfig
{
  olc::vi2d pos{};
  olc::vi2d dims{10, 10};

  MenuLayout layout{MenuLayout::Vertical};

  bool visible{true};
  bool highligtable{true};
  bool propagateEventsToChildren{true};

  std::optional<HighlightCallback> highlightCallback{};
  std::optional<ClickCallback> clickCallback{};
  std::optional<GameCallback> gameClickCallBack{};
};

} // namespace pge