
#pragma once

#include "UserInputData.hh"
#include "Vector2d.hh"

namespace test {

auto generateInputForClickAt(const pge::Vec2i &click) -> ui::UserInputData;

}
