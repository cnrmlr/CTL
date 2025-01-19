#include "physica/hypergraph.h"
#include "physica/common_utility.h"
#include "physica/hyperedge.h"
#include "physica/node.h"

namespace cnr
{
Hypergraph::Hypergraph() : IdentifiableBase() {}

Hypergraph::~Hypergraph() {}

const std::weak_ptr<Node> Hypergraph::addNode()
{
   std::shared_ptr<Node> node = std::make_shared<Node>();
   return nodes_.emplace_back(node);
}

void Hypergraph::removeNode(const std::weak_ptr<Node>& node)
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

const std::vector<std::weak_ptr<Node>> Hypergraph::addNodes(size_t count)
{
   std::vector<std::weak_ptr<Node>> newNodes;
   newNodes.reserve(count);

   for (size_t i = 0; i < count; ++i)
   {
      newNodes.emplace_back(addNode());
   }

   return newNodes;
}

void Hypergraph::removeNodes(const std::vector<std::weak_ptr<Node>>& nodes)
{
   for (auto& node : nodes)
   {
      removeNode(node);
   }
}

const std::weak_ptr<Hyperedge> Hypergraph::addEdge(std::vector<std::weak_ptr<Node>> nodes)
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

void Hypergraph::removeEdge(const std::weak_ptr<Hyperedge>& edge)
{
   auto edgeIter = FindWithWeakPtr(edges_, edge);

   if (edgeIter != edges_.end())
   {
      edgeIter->get()->removeFromNodeIncidenceLists();
      edges_.erase(edgeIter);
   }
}

const std::vector<std::weak_ptr<Hyperedge>>
Hypergraph::addEdges(std::vector<std::vector<std::weak_ptr<Node>>> NodeSets)
{
   std::vector<std::weak_ptr<Hyperedge>> newEdges;
   newEdges.reserve(NodeSets.size());

   for (auto& set : NodeSets)
   {
      newEdges.emplace_back(addEdge(set));
   }

   return newEdges;
}

void Hypergraph::removeEdges(const std::vector<std::weak_ptr<Hyperedge>>& edges)
{
   for (auto& edge : edges)
   {
      removeEdge(edge);
   }
}

const std::vector<std::weak_ptr<Node>> Hypergraph::getNodes() const
{
   return MakeWeakPtrVector(nodes_);
}

const std::vector<std::weak_ptr<Hyperedge>> Hypergraph::getEdges() const
{
   return MakeWeakPtrVector(edges_);
}

std::vector<std::vector<unsigned short>> Hypergraph::getAdajencyMatrix() const
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
} // namespace cnr
