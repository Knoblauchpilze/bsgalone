
#pragma once

#include "Account.hh"
#include "DbConnection.hh"

namespace test {

auto insertTestAccount(bsgalone::server::DbConnection &dbConnection) -> bsgalone::server::Account;

}
