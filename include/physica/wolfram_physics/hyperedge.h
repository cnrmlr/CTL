#pragma once

#include "identifiable_base.h"

#include <memory>
#include <vector>

namespace cnr
{
class Node;

class Hyperedge : public IdentifiableBase, public std::enable_shared_from_this<Hyperedge>
{
   friend class Hypergraph;

 public:
   Hyperedge();
   Hyperedge(const std::vector<std::shared_ptr<Node>>& nodes);
   ~Hyperedge();
   Hyperedge(const Hyperedge& rhs) = delete;
   Hyperedge operator=(const Hyperedge& rhs) = delete;

   const std::vector<std::weak_ptr<Node>> getNodes() const;

   bool isAdjacentTo(const std::weak_ptr<Hyperedge>& edge);
   bool isIncidentTo(const std::weak_ptr<Node>& node);

   bool operator==(const Hyperedge& rhs) const;
   bool operator!=(const Hyperedge& rhs) const;

 protected:
   const std::vector<std::shared_ptr<Node>>::iterator findNode(std::weak_ptr<Node> node);
   void removeNode(std::weak_ptr<Node> node);
   void removeFromNodeIncidenceLists();

 private:
   std::vector<std::shared_ptr<Node>> nodes_;
};
} // namespace cnr
