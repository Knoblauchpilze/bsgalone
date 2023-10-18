
#include "ServerView.hh"

namespace bsgo {

ServerView::ServerView(const CoordinatorShPtr &coordinator)
  : IView("server", coordinator)
{}

} // namespace bsgo
