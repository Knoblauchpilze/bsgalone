
#pragma once

#include "IView.hh"
#include <memory>

namespace bsgo {

class ServerView : public IView
{
  public:
  ServerView(const CoordinatorShPtr &coordinator);
};

using ServerViewShPtr = std::shared_ptr<ServerView>;

} // namespace bsgo
