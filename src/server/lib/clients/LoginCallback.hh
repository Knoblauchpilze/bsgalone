
#pragma once

#include "Uuid.hh"
#include <functional>

namespace bsgo {

/// @brief - Callback for when a player has successfully logged in.
/// @param clientId - the index of the client which just logged in: this is passed with
/// the messages received from this client.
/// @param playerDbId - the index of the player in the database associated to the client.
using PlayerLoginCallback = std::function<void(const Uuid clientId, const Uuid playerDbId)>;

} // namespace bsgo
