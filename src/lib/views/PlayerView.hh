
#pragma once

#include <core_utils/CoreObject.hh>
#include <memory>

namespace bsgo {

class PlayerView : public utils::CoreObject
{
  public:
  PlayerView();
};

using PlayerViewShPtr = std::shared_ptr<PlayerView>;

} // namespace bsgo
