
#pragma once

#include "IView.hh"
#include <memory>

namespace bsgo {

class PlayerView : public IView
{
  public:
  PlayerView(const CoordinatorShPtr &coordinator);
  ~PlayerView() override = default;
};

using PlayerViewShPtr = std::shared_ptr<PlayerView>;

} // namespace bsgo
