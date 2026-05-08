
#pragma once

#include "Account.hh"
#include "DbConnection.hh"
#include "System.hh"

namespace test {

auto insertTestAccount(bsgalone::server::DbConnection &dbConnection) -> bsgalone::server::Account;
auto insertTestSystem(bsgalone::server::DbConnection &dbConnection) -> bsgalone::server::System;

} // namespace test
