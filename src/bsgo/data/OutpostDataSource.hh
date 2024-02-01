
#pragma once

#include "Repositories.hh"
#include <core_utils/CoreObject.hh>

namespace bsgo {

class Coordinator;

class OutpostDataSource : public utils::CoreObject
{
  public:
  OutpostDataSource(const Repositories &repositories, const Uuid systemDbId);
  ~OutpostDataSource() override = default;

  void initialize(Coordinator &coordinator) const;

  private:
  Uuid m_systemDbId{};
  Repositories m_repositories{};

  void registerOutpost(Coordinator &coordinator, const Uuid outpost) const;
};

} // namespace bsgo
