
#pragma once

#include "Account.hh"
#include "DbConnection.hh"
#include "Faction.hh"
#include "System.hh"
#include <optional>

namespace test {

auto insertTestAccount(bsgalone::server::DbConnection &dbConnection) -> bsgalone::server::Account;
auto insertTestSystem(bsgalone::server::DbConnection &dbConnection) -> bsgalone::server::System;

/// @brief - Inserts a test player for a player. The ship will be automatically picked as the base
/// ship for each faction. There's no verification that the faction provided in input matches the
/// actual faction of the player: the caller should be careful about this.
/// The ship will be inserted either as active or not (depending on the value of the input bool)
/// and the ship will be registered in the provided system when not empty.
/// @param dbConnectin - the database connection to use to perform the SQL queries
/// @param playerDbId - the identifier of the player the ship belongs to: if it does not exist the
/// insertion of the ship will raise an error
/// @param faction - the faction of the ship to create. Should match the faction of the player
/// @param active - whether or not the ship should be active
/// @param maybeSystem - when provided, the system in which the ship currently is
void insertTestShip(bsgalone::server::DbConnection &dbConnection,
                    const bsgalone::core::Uuid playerDbId,
                    const bsgalone::core::Faction faction,
                    const bool active,
                    const std::optional<bsgalone::core::Uuid> &maybeSystem);

} // namespace test
