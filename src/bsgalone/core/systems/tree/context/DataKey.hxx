
#pragma once

#include "DataKey.hh"

namespace bsgo {

template<typename Value>
inline auto DataKey::as() const -> Value
{
  return std::get<Value>(m_key);
}

template<typename Value>
inline void DataKey::set(const Value &value)
{
  m_key     = value;
  m_changed = true;
}

template<typename Value>
inline auto DataKey::fromValue(const Value &value) -> DataKey
{
  DataKey key;
  key.m_key = value;
  return key;
}

} // namespace bsgo
