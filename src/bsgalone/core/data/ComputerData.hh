
#pragma once

#include "ComputerPriceRepository.hh"
#include "ComputerRepository.hh"
#include "EntityKind.hh"
#include "TickDuration.hh"
#include "Uuid.hh"
#include <optional>
#include <unordered_map>
#include <unordered_set>

namespace bsgalone::core {

struct ComputerData
{
  Uuid dbId{};

  std::string name{};
  bool offensive{};
  float powerCost{};
  std::optional<float> range{};
  chrono::TickDuration reloadTime{};

  std::optional<chrono::TickDuration> duration{};
  std::optional<std::unordered_set<EntityKind>> allowedTargets{};
  std::optional<float> damageModifier{};

  std::unordered_map<Uuid, int> price{};

  bool operator==(const ComputerData &rhs) const;
};

auto fromDbComputer(const Computer &computer, const ComputerPriceRepository &repository)
  -> ComputerData;

auto operator<<(std::ostream &out, const ComputerData &data) -> std::ostream &;
bool operator>>(std::istream &in, ComputerData &data);

} // namespace bsgalone::core
