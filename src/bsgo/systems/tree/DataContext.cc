
#include "DataContext.hh"

namespace bsgo {

DataContext::DataContext() {}

DataContext::DataContext(const int targetIndex)
  : m_targetIndex(targetIndex)
{}

auto DataContext::tryGetTargetIndex() const -> std::optional<int>
{
  return m_targetIndex;
}

void DataContext::setTargetIndex(const int targetIndex)
{
  if (!m_targetIndex || *m_targetIndex != targetIndex)
  {
    m_changed     = true;
    m_targetIndex = targetIndex;
  }
}

void DataContext::clearTargetIndex()
{
  m_targetIndex.reset();
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
