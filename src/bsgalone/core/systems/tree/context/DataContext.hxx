
#pragma once

#include "DataContext.hh"

namespace bsgo {

template<typename Value>
inline void DataContext::setKey(const ContextKey key, const Value &value)
{
  m_changed           = true;
  auto [it, inserted] = m_keys.emplace(key, DataKey::fromValue(value));

  if (!inserted)
  {
    it->second.set(value);
  }
}

template<typename Value>
inline auto DataContext::tryGetValue(const ContextKey key) const -> std::optional<Value>
{
  const auto maybeKey = m_keys.find(key);
  if (maybeKey == m_keys.end())
  {
    return {};
  }

  return maybeKey->second.as<Value>();
}

} // namespace bsgo
