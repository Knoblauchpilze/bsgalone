
#pragma once

#include "DatabaseEntityMapper.hh"
#include "Repositories.hh"
#include <core_utils/CoreObject.hh>

namespace bsgo {

class Coordinator;

class OutpostDataSource : public utils::CoreObject
{
  public:
  OutpostDataSource(const Repositories &repositories);
  ~OutpostDataSource() override = default;

  void initialize(const Uuid systemDbId,
                  Coordinator &coordinator,
                  DatabaseEntityMapper &entityMapper) const;

  private:
  Repositories m_repositories{};

  void registerOutpost(Coordinator &coordinator,
                       const Uuid outpostDbId,
                       DatabaseEntityMapper &entityMapper) const;
};

} // namespace bsgo
