
#include "ServerView.hh"

namespace bsgo {

ServerView::ServerView()
  : utils::CoreObject("server")
{
  setService("view");
}

} // namespace bsgo
