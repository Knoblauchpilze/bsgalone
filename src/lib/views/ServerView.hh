
#pragma once

#include "IView.hh"
#include <core_utils/CoreObject.hh>
#include <memory>

namespace bsgo {

class ServerView : public utils::CoreObject, public IView
{
  public:
  ServerView();

  void update(const float elapsedSeconds) override;
};

using ServerViewShPtr = std::shared_ptr<ServerView>;

} // namespace bsgo
