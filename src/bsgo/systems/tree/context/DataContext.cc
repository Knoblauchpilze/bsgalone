
#include "DataContext.hh"
#include <algorithm>

namespace bsgo {

void DataContext::clear(const ContextKey key)
{
  m_keys.erase(key);
  m_changed = true;
}

bool DataContext::changed() const
{
  return m_changed;
}

void DataContext::markAsSynced()
{
  m_changed = false;
}

} // namespace bsgo
