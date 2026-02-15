
#pragma once

#include <memory>

namespace pge {

class IViewListener
{
  public:
  IViewListener()          = default;
  virtual ~IViewListener() = default;

  /// @brief - Interface method allowing to be notified when a view is being updated.
  /// This method will be called whenever the view receives information that makes its
  /// internal state change.
  /// Inheriting classes should implement this method if they in turn need to update
  /// their state based on the view's state. A typical usecase is to mark some boolean
  /// `isReady` to false so that the next calls to the caller's methods will trigger
  /// a refresh.
  virtual void onViewUpdated() = 0;
};

using IViewListenerPtr = std::unique_ptr<IViewListener>;

} // namespace pge
