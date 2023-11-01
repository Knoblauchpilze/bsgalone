
#include "ServerView.hh"

namespace bsgo {

ServerView::ServerView(const CoordinatorShPtr &coordinator, const Repositories &repositories)
  : IView("server", coordinator, repositories)
{}

} // namespace bsgo
