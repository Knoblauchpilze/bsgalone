
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

enum class CustomRenderMode
{
  PRE_RENDER,
  POST_RENDER
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

  /// @brief - Define when the custom rendering method is called. The two values
  /// correspond to either before or after the main menu rendering method. This
  /// allows inheriting classes to configure how their own rendering logic is
  /// interacting with the base rendering logic.
  CustomRenderMode customRenderMode{CustomRenderMode::POST_RENDER};
};

} // namespace pge
