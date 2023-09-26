
#pragma once

#include <core_utils/CoreObject.hh>
#include <memory>

namespace bsgo {

class OutpostView : public utils::CoreObject
{
  public:
  OutpostView();
};

using OutpostViewPtr = std::unique_ptr<OutpostView>;

} // namespace bsgo
