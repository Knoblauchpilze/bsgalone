
#include "ServerView.hh"

namespace bsgo {

ServerView::ServerView()
  : utils::CoreObject("server")
{
  setService("view");
}

void ServerView::update(const float /*elapsedSeconds*/) {}

} // namespace bsgo
