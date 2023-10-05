
#pragma once

#include "IView.hh"
#include <core_utils/CoreObject.hh>
#include <memory>

namespace bsgo {

class PlayerView : public utils::CoreObject, public IView
{
  public:
  PlayerView();
  ~PlayerView() override = default;

  void update(const float elapsedSeconds) override;
};

using PlayerViewShPtr = std::shared_ptr<PlayerView>;

} // namespace bsgo
