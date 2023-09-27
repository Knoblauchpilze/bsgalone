
#pragma once

#include <core_utils/CoreObject.hh>
#include <memory>

namespace bsgo {

class SystemView : public utils::CoreObject
{
  public:
  SystemView();
};

using SystemViewShPtr = std::shared_ptr<SystemView>;

} // namespace bsgo
