
#pragma once

#include "Controls.hh"
#include <functional>

// TODO: This is only needed for the `setScreen` method in the interface.
// Once the screen change is event driven, it should be removed.
namespace bsgalone::client {
enum class Screen;
}

namespace ui {

// TODO: This interface could be removed when the UI is properly event driven.
// Instead of publishing actions, the menus could publish events.
// The interface shows that it's not a lot to replace, it is probably enough
// to have a `ScreenChangeCommand`.
class IScreenChanger
{
  public:
  virtual ~IScreenChanger() = default;

  virtual auto getScreen() const noexcept -> bsgalone::client::Screen = 0;
  virtual void setScreen(const bsgalone::client::Screen screen)       = 0;
  virtual void terminate() noexcept                                    = 0;
};

using GameCallback = std::function<void(IScreenChanger &)>;

struct UserInputData
{
  pge::controls::State controls{};
  std::vector<GameCallback> actions{};
};

} // namespace ui
