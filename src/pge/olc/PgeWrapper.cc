
#include "PgeWrapper.hh"

namespace pge {

PgeWrapper::PgeWrapper(const WrapperCallbacks &config)
  : olc::PixelGameEngine()
  , m_createCallback(config.createCallback)
  , m_updateCallback(config.updateCallback)
  , m_destroyCallback(config.destroyCallback)
{}

bool PgeWrapper::OnUserCreate()
{
  if (m_createCallback)
  {
    return (*m_createCallback)();
  }
  return true;
}

bool PgeWrapper::OnUserUpdate(float fElapsedTime)
{
  if (m_updateCallback)
  {
    return (*m_updateCallback)(fElapsedTime);
  }
  return true;
}

bool PgeWrapper::OnUserDestroy()
{
  if (m_destroyCallback)
  {
    return (*m_destroyCallback)();
  }
  return true;
}

} // namespace pge
