
#pragma once

#include "INode.hh"

namespace bsgo {

class ControlNode : public INode
{
  public:
  ControlNode(const std::string &name);
  ~ControlNode() override = default;

  void addChild(INodePtr node);
  void reset(DataContext &data) override;

  protected:
  std::vector<INodePtr> m_children{};
};

} // namespace bsgo
