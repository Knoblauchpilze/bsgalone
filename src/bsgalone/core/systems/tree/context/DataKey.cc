
#include "DataKey.hh"

namespace bsgo {

bool DataKey::changed() const
{
  return m_changed;
}

void DataKey::markAsSynced()
{
  m_changed = false;
}

} // namespace bsgo
