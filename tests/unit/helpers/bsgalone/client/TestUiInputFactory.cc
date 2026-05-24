
#include "TestUiInputFactory.hh"
#include "Controls.hh"

namespace test {

auto generateInputForClickAt(const pge::Vec2i &click) -> ui::UserInputData
{
  pge::controls::State state{};
  state.mPosX = click.x;
  state.mPosY = click.y;

  state.buttons[pge::controls::mouse::LEFT] = pge::controls::mouse::RELEASED;

  return ui::UserInputData{.controls = state};
}

} // namespace test
