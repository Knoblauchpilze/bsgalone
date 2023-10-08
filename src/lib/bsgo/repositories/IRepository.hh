
#pragma once

#include <core_utils/CoreObject.hh>

namespace bsgo {

class IRepository : public utils::CoreObject
{
  public:
  IRepository(const std::string &name);
  ~IRepository() override = default;
};

} // namespace bsgo
