
#pragma once

#include <core_utils/CoreObject.hh>
#include <memory>

namespace bsgo {

class ServerView : public utils::CoreObject
{
  public:
  ServerView();
};

using ServerViewPtr = std::unique_ptr<ServerView>;

} // namespace bsgo
