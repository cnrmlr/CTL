#pragma once

#include "identifiable_base.h"

#include <memory>
#include <vector>

namespace cnr
{
template <typename T>
class Hyperedge : public IdentifiableBase, public std::enable_shared_from_this<Hyperedge<T>>
{
   friend class Hypergraph<T>;

 public:
   Hyperedge() : IdentifiableBase(), nodes_(0) {}

   Hyperedge(const std::vector<std::shared_ptr<Node>>& nodes) : IdentifiableBase()
   {
      nodes_.reserve(nodes.size());

      for (auto& node : nodes)
      {
         nodes_.push_back(node);
      }
   }

   ~Hyperedge() {}

   const std::vector<std::weak_ptr<Node>> getNodes() const { return MakeWeakPtrVector(nodes_); }

   bool isAdjacentTo(const std::weak_ptr<Hyperedge>& edge)
   {
      for (auto& node : nodes_)
      {
         if (edge.lock() && edge.lock()->isIncidentTo(node))
         {
            return true;
         }
      }

      return false;
   }

   bool isIncidentTo(const std::weak_ptr<Node>& vertex) { return findNode(vertex) != nodes_.end(); }

   bool operator==(const Hyperedge& rhs) const { return IdentifiableBase::operator==(rhs); }

   bool operator!=(const Hyperedge& rhs) const { return IdentifiableBase::operator!=(rhs); }

 protected:
   const std::vector<std::shared_ptr<Node>>::iterator findNode(std::weak_ptr<Node> node)
   {
      return FindWithWeakPtr(nodes_, node);
   }

   void removeNode(std::weak_ptr<Node> node)
   {
      auto nodeIter = findNode(node);

      if (nodeIter != nodes_.end())
      {
         nodes_.erase(findNode(node));
      }
   }

   void removeFromNodeIncidenceLists()
   {
      for (auto& node : nodes_)
      {
         node->removeIncidentEdge(shared_from_this());
      }
   }

 private:
   std::vector<std::shared_ptr<T>> nodes_;
};

template <typename T>
class Hypergraph : public IdentifiableBase
{
 public:
   Hypergraph() : IdentifiableBase() {}

   ~Hypergraph() {}

   const std::weak_ptr<Node> addNode()
   {
      std::shared_ptr<Node> node = std::make_shared<Node>();
      return nodes_.emplace_back(node);
   }

   void removeNode(const std::weak_ptr<Node>& node)
   {
      // remove the Node from the edges it exists on
      for (auto& edge : edges_)
      {
         edge->removeNode(node);
      }

      // remove the Node from the graph
      auto nodeIter = FindWithWeakPtr(nodes_, node);

      if (nodeIter != nodes_.end())
      {
         nodes_.erase(nodeIter);
      }
   }

   const std::vector<std::weak_ptr<Node>> addNodes(size_t count)
   {
      std::vector<std::weak_ptr<Node>> newNodes;
      newNodes.reserve(count);

      for (size_t i = 0; i < count; ++i)
      {
         newNodes.emplace_back(addNode());
      }

      return newNodes;
   }

   void removeNodes(const std::vector<std::weak_ptr<Node>>& nodes)
   {
      for (auto& node : nodes)
      {
         removeNode(node);
      }
   }

   const std::weak_ptr<Hyperedge> addEdge(std::vector<std::weak_ptr<Node>> nodes)
   {
      std::vector<std::shared_ptr<Node>> sharedNodes = MakeSharedPtrVector(nodes);

      // return nullptr if there was an issue doing the conversion
      if (sharedNodes.empty())
      {
         return std::shared_ptr<Hyperedge>(nullptr);
      }

      // add the edge to the graph and give the incident vertices a ref to the new edge
      std::shared_ptr<Hyperedge> edge = std::make_shared<Hyperedge>(sharedNodes);

      edges_.push_back(edge);

      for (auto& node : sharedNodes)
      {
         node->addIncidentEdge(edge);
      }

      return edge;
   }

   void removeEdge(const std::weak_ptr<Hyperedge>& edge)
   {
      auto edgeIter = FindWithWeakPtr(edges_, edge);

      if (edgeIter != edges_.end())
      {
         edgeIter->get()->removeFromNodeIncidenceLists();
         edges_.erase(edgeIter);
      }
   }

   const std::vector<std::weak_ptr<Hyperedge>>
   addEdges(std::vector<std::vector<std::weak_ptr<Node>>> NodeSets)
   {
      std::vector<std::weak_ptr<Hyperedge>> newEdges;
      newEdges.reserve(NodeSets.size());

      for (auto& set : NodeSets)
      {
         newEdges.emplace_back(addEdge(set));
      }

      return newEdges;
   }

   void removeEdges(const std::vector<std::weak_ptr<Hyperedge>>& edges)
   {
      for (auto& edge : edges)
      {
         removeEdge(edge);
      }
   }

   const std::vector<std::weak_ptr<Node>> getNodes() const { return MakeWeakPtrVector(nodes_); }

   const std::vector<std::weak_ptr<Hyperedge>> getEdges() const
   {
      return MakeWeakPtrVector(edges_);
   }

   std::vector<std::vector<unsigned short>> getAdajencyMatrix() const
   {
      std::vector<std::vector<unsigned short>> adjacencyMatrix;
      adjacencyMatrix.reserve(nodes_.size());

      for (auto& node_i : nodes_)
      {
         std::vector<unsigned short> adjacencyList;
         adjacencyList.reserve(nodes_.size());

         for (auto& node_j : nodes_)
         {
            adjacencyList.push_back(static_cast<unsigned short>(node_i->isAdjacentTo(node_j)));
         }

         adjacencyMatrix.push_back(adjacencyList);
      }

      return adjacencyMatrix;
   }

 private:
   std::vector<std::shared_ptr<T>> nodes_;
   std::vector<std::shared_ptr<Hyperedge>> edges_;
};
} // namespace cnr
