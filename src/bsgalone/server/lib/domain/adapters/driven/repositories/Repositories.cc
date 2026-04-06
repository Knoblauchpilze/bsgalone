
#include "Repositories.hh"
#include "DbConnection.hh"

namespace bsgalone::server {

Repositories::Repositories()
{
  auto connection = std::make_shared<DbConnection>();
  connection->connect();

  accountRepository = std::make_shared<AccountRepository>(connection);
  playerRepository  = std::make_shared<PlayerRepository>(connection);

  accountRepository->initialize();
  playerRepository->initialize();
}

} // namespace bsgalone::server
