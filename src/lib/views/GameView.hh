
#pragma once

#include <core_utils/CoreObject.hh>
#include <memory>

namespace bsgo {

class GameView : public utils::CoreObject
{
  public:
  GameView();
};

using GameViewShPtr = std::shared_ptr<GameView>;

} // namespace bsgo
