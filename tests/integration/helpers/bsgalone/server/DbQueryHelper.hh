
#pragma once

#include "Account.hh"
#include "DbConnection.hh"

namespace test {

auto insertTestAccount(bsgalone::core::DbConnection &dbConnection) -> bsgalone::core::Account;

}
