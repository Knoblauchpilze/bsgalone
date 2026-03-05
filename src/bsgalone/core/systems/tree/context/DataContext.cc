
#include "DataContext.hh"
#include <algorithm>

namespace bsgo {

auto DataContext::tryGetKey(const ContextKey key) const -> std::optional<DataKey>
{
  const auto maybeKey = m_keys.find(key);
  if (maybeKey == m_keys.end())
  {
    return {};
  }

  return maybeKey->second;
}

void DataContext::clear(const ContextKey key)
{
  m_keys.erase(key);
  m_changed = true;
}

bool DataContext::changed() const
{
  // https://stackoverflow.com/questions/17306715/lambda-signature-for-each-unordered-map
  return m_changed || std::any_of(m_keys.begin(), m_keys.end(), [](const KeyMap::value_type &key) {
           return key.second.changed();
         });
}

void DataContext::markAsSynced()
{
  m_changed = false;
  for (auto &key : m_keys)
  {
    key.second.markAsSynced();
  }
}

} // namespace bsgo
