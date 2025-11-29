
#pragma once

#include "CoreObject.hh"
#include "DatabaseEntityMapper.hh"
#include "OutpostData.hh"
#include "Repositories.hh"
#include <optional>

namespace bsgo {

class Coordinator;

class OutpostDataSource : public core::CoreObject
{
  public:
  OutpostDataSource();
  OutpostDataSource(const Repositories &repositories);
  ~OutpostDataSource() override = default;

  void initialize(const Uuid systemDbId,
                  Coordinator &coordinator,
                  DatabaseEntityMapper &entityMapper) const;

  void registerOutpost(Coordinator &coordinator,
                       const OutpostData &data,
                       DatabaseEntityMapper &entityMapper) const;

  private:
  std::optional<Repositories> m_repositories{};

  void registerOutpost(Coordinator &coordinator,
                       const Uuid outpostDbId,
                       DatabaseEntityMapper &entityMapper) const;
};

} // namespace bsgo
