
#pragma once

#include "Uuid.hh"

namespace std {
template<>
struct hash<bsgalone::core::Uuid>
{
  [[nodiscard]] auto operator()(const bsgalone::core::Uuid &uuid) const -> std::size_t
  {
    return std::hash<uuids::uuid>{}(uuid.m_uuid);
  }
};
} // namespace std
