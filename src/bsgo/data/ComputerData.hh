
#pragma once

#include "ComputerPriceRepository.hh"
#include "ComputerRepository.hh"
#include "EntityKind.hh"
#include "TickDuration.hh"
#include "Uuid.hh"
#include <optional>
#include <unordered_map>
#include <unordered_set>

namespace bsgo {

struct ComputerData
{
  Uuid dbId{};

  std::string name{};
  bool offensive{};
  float powerCost{};
  std::optional<float> range{};
  chrono::TickDuration reloadTime{};

  std::optional<chrono::TickDuration> duration{};
  std::optional<std::unordered_set<bsgalone::core::EntityKind>> allowedTargets{};
  std::optional<float> damageModifier{};

  std::unordered_map<Uuid, int> price{};

  bool operator==(const ComputerData &rhs) const;

  auto serialize(std::ostream &out) const -> std::ostream &;
  bool deserialize(std::istream &in);
};

auto fromDbComputer(const Computer &computer, const ComputerPriceRepository &repository)
  -> ComputerData;

} // namespace bsgo
