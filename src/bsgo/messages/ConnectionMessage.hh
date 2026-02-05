

#pragma once

#include "AbstractMessage.hh"

namespace bsgo {

class ConnectionMessage : public AbstractMessage
{
  public:
  ConnectionMessage();
  ~ConnectionMessage() override = default;

  auto serialize(std::ostream &out) const -> std::ostream & override;
  bool deserialize(std::istream &in) override;

  auto clone() const -> IMessagePtr override;
};

} // namespace bsgo
