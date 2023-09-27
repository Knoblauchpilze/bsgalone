
#pragma once

#include <core_utils/CoreObject.hh>
#include <memory>

namespace bsgo {

class ServerView : public utils::CoreObject
{
  public:
  ServerView();
};

using ServerViewShPtr = std::shared_ptr<ServerView>;

} // namespace bsgo
