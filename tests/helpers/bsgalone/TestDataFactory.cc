
#include "TestDataFactory.hh"
#include "TimeUtils.hh"

namespace test {
namespace {
auto randomInt(const int min, const int max) -> int
{
  return std::rand() % (max - min) + min;
}

auto randomFloat(const int min, const int max, const int decimals = 2) -> float
{
  const auto frac = std::rand() % static_cast<int>(std::pow(10, decimals));

  return randomInt(min, max)
         + static_cast<float>(frac) / static_cast<float>(std::pow(10, decimals));
}
} // namespace

auto generateAccount() -> bsgalone::server::Account
{
  return bsgalone::server::Account{
    // https://en.cppreference.com/w/cpp/chrono/system_clock/formatter.html
    .username = std::format("random-account-{:%F%T}", core::now()),
    .password = "secure-password",
  };
}

auto generatePlayer(const std::optional<bsgalone::core::Uuid> &maybeAccount)
  -> bsgalone::server::Player
{
  return bsgalone::server::Player{
    .account = maybeAccount,
    .name    = std::format("random-player-{:%F%T}", core::now()),
    .faction = bsgalone::core::Faction::CYLON,
    .role    = bsgalone::core::GameRole::PILOT,
  };
}

auto generateSystem(const std::optional<bsgalone::core::Uuid> &maybeId) -> bsgalone::server::System
{
  return bsgalone::server::System{
    .dbId        = maybeId.has_value() ? *maybeId : bsgalone::core::Uuid{},
    .name        = std::format("random-system-{:%F%T}", core::now()),
    .position    = Eigen::Vector3f::Random(),
    .currentTick = chrono::Tick::fromInt(randomInt(1, 78)),
    .step        = chrono::TimeStep(randomInt(3, 17), chrono::Duration::fromSeconds(2)),
  };
}

auto generateAsteroid(const bool withLoot) -> bsgalone::core::Asteroid
{
  std::optional<bsgalone::core::Loot> loot{};
  if (withLoot)
  {
    loot = bsgalone::core::Loot{.resource = bsgalone::core::Uuid{}, .amount = randomInt(1, 78)};
  }

  return bsgalone::core::Asteroid{
    .dbId      = bsgalone::core::Uuid{},
    .position  = Eigen::Vector3f::Random(),
    .radius    = randomFloat(1.0f, 2.0f, 2),
    .health    = randomFloat(1.0f, 40.0f, 1),
    .maxHealth = randomFloat(41.0f, 45.0f, 3),
    .loot      = loot,
  };
}

} // namespace test
