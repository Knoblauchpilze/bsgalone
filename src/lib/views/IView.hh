
#pragma once

#include <memory>

namespace bsgo {

class IView
{
  public:
  virtual ~IView() = default;

  virtual void update(const float elapsedSeconds) = 0;
};

using IViewShPtr = std::shared_ptr<IView>;

} // namespace bsgo
