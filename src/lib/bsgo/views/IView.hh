
#pragma once

#include <memory>

namespace bsgo {

class IView
{
  public:
  IView()          = default;
  virtual ~IView() = default;

  virtual bool isReady() const noexcept = 0;
};

using IViewShPtr = std::shared_ptr<IView>;

} // namespace bsgo
