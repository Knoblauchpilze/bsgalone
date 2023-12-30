
#include "LoginView.hh"

namespace bsgo {

LoginView::LoginView(const CoordinatorShPtr &coordinator,
                     const Repositories &repositories,
                     const IMessageQueue *messageQueue)
  : AbstractView("login", coordinator, repositories, messageQueue)
{}

auto LoginView::tryLogin(const std::string &name, const std::string &password) const
  -> std::optional<Uuid>
{
  const auto maybePlayer = m_repositories.playerRepository->findOneByName(name);
  if (!maybePlayer)
  {
    warn("No player with name \"" + name + "\"");
    return {};
  }
  if (maybePlayer->password != password)
  {
    warn("Wrong password for player \"" + name + "\"",
         "Expected " + maybePlayer->password + " but got " + password);
    return {};
  }

  return maybePlayer->id;
}

} // namespace bsgo
