
#include "DataKey.hh"

namespace bsgalone::core {

bool DataKey::changed() const
{
  return m_changed;
}

void DataKey::markAsSynced()
{
  m_changed = false;
}

} // namespace bsgalone::core
