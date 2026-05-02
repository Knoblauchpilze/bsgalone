
#pragma once

#include "Account.hh"
#include "DbConnection.hh"
#include "System.hh"

namespace test {

auto insertTestAccount(bsgalone::core::DbConnection &dbConnection) -> bsgalone::server::Account;
auto insertTestSystem(bsgalone::core::DbConnection &dbConnection) -> bsgalone::server::System;

} // namespace test
