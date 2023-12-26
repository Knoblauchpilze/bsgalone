
#pragma once

#include "AbstractMessage.hh"
#include "SystemType.hh"

namespace bsgo {

class SystemMessage : public AbstractMessage
{
  public:
  SystemMessage(const SystemType &systemType);
  ~SystemMessage() override = default;

  auto systemType() const -> SystemType;

  private:
  SystemType m_systemType{};
};

} // namespace bsgo
