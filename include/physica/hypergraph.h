#pragma once

#include "identifiable_base.h"

#include <memory>
#include <vector>

namespace cnr
{
class Hyperedge;
class Node;

class Hypergraph : public IdentifiableBase
{
 public:
   Hypergraph();
   ~Hypergraph();
   Hypergraph(const Hypergraph&) = delete;
   Hypergraph operator=(const Hypergraph) = delete;

   const std::weak_ptr<Node> addNode();
   void removeNode(const std::weak_ptr<Node>& Node);

   const std::vector<std::weak_ptr<Node>> addNodes(const size_t count);
   void removeNodes(const std::vector<std::weak_ptr<Node>>& nodes);

   const std::weak_ptr<Hyperedge> addEdge(std::vector<std::weak_ptr<Node>> nodes);
   void removeEdge(const std::weak_ptr<Hyperedge>& edge);

   const std::vector<std::weak_ptr<Hyperedge>>
   addEdges(std::vector<std::vector<std::weak_ptr<Node>>> nodeSets);
   void removeEdges(const std::vector<std::weak_ptr<Hyperedge>>& edges);

   const std::vector<std::weak_ptr<Node>> getNodes() const;
   const std::vector<std::weak_ptr<Hyperedge>> getEdges() const;

   std::vector<std::vector<unsigned short>> getAdajencyMatrix() const;

 private:
   std::vector<std::shared_ptr<Node>> nodes_;
   std::vector<std::shared_ptr<Hyperedge>> edges_;
};
} // namespace cnr
