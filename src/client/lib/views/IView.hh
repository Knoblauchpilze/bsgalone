
#pragma once

#include "IViewListener.hh"
#include <memory>

namespace pge {

class IView
{
  public:
  IView()          = default;
  virtual ~IView() = default;

  virtual bool isReady() const noexcept = 0;
  virtual void reset()                  = 0;

  /// @brief - registers a new listener for this view which will be notified of updates.
  /// Note: this method is NOT required to be thread-safe
  /// @param listener - the listener to register
  virtual void addListener(IViewListenerPtr listener) = 0;
};

using IViewShPtr = std::shared_ptr<IView>;

} // namespace pge
