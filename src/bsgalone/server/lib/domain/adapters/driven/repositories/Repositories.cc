
#include "Repositories.hh"
#include "DbConnection.hh"

namespace bsgalone::server {

Repositories::Repositories()
{
  auto connection = std::make_shared<DbConnection>();
  connection->connect();

  accountRepository = std::make_shared<AccountRepository>(connection);
  playerRepository  = std::make_shared<PlayerRepository>(connection);
  systemRepository  = std::make_shared<SystemRepository>(connection);

  accountRepository->initialize();
  playerRepository->initialize();
  systemRepository->initialize();
}

} // namespace bsgalone::server
