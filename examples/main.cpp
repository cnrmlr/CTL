#include "physica/wolfram_physics.h"

#include <cassert>
#include <iostream>

/*
 * Hypergraph Symbology
 *
 * In hypergraph theory, the following symbols and conventions are commonly used:
 *
 * V: The set of all nodes in the hypergraph.
 *    - Example: V = {v1, v2, v3, ..., vn}
 *
 * W: A subset of the node set V.
 *    - Example: V = {v1, v2, v3, ..., vn}
 *               W = {v2, v3}
 *
 * E: The set of all hyperedges in the hypergraph.
 *    - Each hyperedge is itself a subset of the node set V.
 *    - Example: E = {e1, e2, e3, ..., em} where each ei is a subset of V
 *
 * F: A subset of the edge set E.
 *    - Example:  E = {e1, e2, e3, ..., em}
 *                F = {e2, e3}
 *
 * v: A single node in the hypergraph
 *
 * e: A single edge in the hypergraph
 *
 * General Notation:
 * - Hypergraphs are often denoted as H = (V, E), where V is the set of nodes
 *   and E is the set of hyperedges.
 * - A hyperedge e in E is a set of nodes, e.g., e = {v1, v2, v3}.
 * - The incidence relation between nodes and hyperedges is fundamental in hypergraph theory.
 *
 * Usage in Code:
 * - std::vector<Node> V;          // The set of all nodes
 * - std::vector<Hyperedge> E;     // The set of all hyperedges
 * - std::vector<Node> W;          // A subset of nodes
 * - std::vector<Hyperedge> F;     // A subset of hyperedges
 * - std::weak_ptr<Node> v;        // A single node
 * - std::weak_ptr<Hyperedge> e;   // A single edge
 *
 * This symbology helps in clearly defining the structure and operations of hypergraphs
 * in both theoretical discussions and practical implementations.
 */

#include <algorithm>
#include <iostream>
#include <set>
#include <unordered_set>
#include <vector>

int main()
{
   // Each hypergraph instance should be managed by std::unique_ptr as these graphs
   // are fundamentally unique in nature and non-copyable. When a new graph is created,
   // it will have zero nodes and zero edges.

   std::unique_ptr<cnr::Hypergraph> G = std::make_unique<cnr::Hypergraph>();

   // The full sets of nodes and edges that currently exist on the hypergraph can be
   // fetched using the following functions. Note that these are sets of weak ptrs,
   // intended to not increase the reference count on objects managed by the hypergraph,
   // and may expire if operations to the hypergraph occur within the same scope that
   // these sets are declared.
   {
      std::vector<std::weak_ptr<cnr::Node>> V = G->getNodes();

      std::vector<std::weak_ptr<cnr::Hyperedge>> E = G->getEdges();
   }

   // Adding a single node to the hypergraph will return a single non-reference counted
   // pointer to the new node. The removal operation will validate the weak pointer and
   // remove it from the graph if it is not expired. The local weak ptr to the node will
   // then be expired.
   {
      std::weak_ptr<cnr::Node> v = G->addNode();

      G->removeNode(v);
   }

   // Nodes can also be added in bulk by specifying the number of nodes to add.
   // This operation will return a vector of weak ptrs to the newly created nodes.
   // We can also remove nodes in bulk by specifying a std::vector or
   // std::initializer_list of the weak node ptrs.
   {
      std::vector<std::weak_ptr<cnr::Node>> W = G->addNodes(3);

      G->removeNodes(W);
   }

   // Edges are added to the hypergraph by specifying a set of incident nodes using
   // a vector or initializer list of weak ptrs to the nodes. If an attempt is made
   // to add an expired node to an edge, that node will not be added to the edge.
   // Once an edge is created, it can be removed from the graph by passing a weak ptr to
   // the hypergraph::removeEdge operation. The remove edge operation also handles the
   // removal of the edge from the incidence lists of the nodes on the edge to ensure
   // there are no dangling strong refs and the edge can be properly removed. If a node is
   // removed from the graph while it exists on one or more edges, it is first removed
   // from all of its incident edges and then removed from the graph.
   {
      std::vector<std::weak_ptr<cnr::Node>> W = G->addNodes(3);

      std::weak_ptr<cnr::Hyperedge> e = G->addEdge(W);

      G->removeEdge(e);

      G->removeNodes(W);
   }

   // Just as nodes can be added in bulk, edges can as well. Hypergraph::addEdges
   // takes a vector of vectors where each sub-vector specifies the nodes on a specific
   // hyperedge. The operation returns a vector of weak edge ptrs so to not increase the
   // reference count on the edges that are now owned by the hypergraph. Edges can be removed
   // in bulk as well with the Hypergraph::removeEdges operation which take a vector of weak
   // edge pointers.
   {
      std::vector<std::weak_ptr<cnr::Node>> W = G->addNodes(6);

      std::vector<std::weak_ptr<cnr::Hyperedge>> F = G->addEdges(
         {{W[0], W[1]}, {W[1], W[2]}, {W[0], W[3]}, {W[1], W[2], W[3], W[4]}, {W[3], W[4], W[5]}});

      G->removeEdges(F);

      G->removeNodes(W);
   }

   {
      cnr::AbstractRelation pattern = {1, 1, 2};

      auto nodes = G->addNodes(2);

      auto edge = G->addEdge({nodes.at(0), nodes.at(0), nodes.at(1)});

      auto match = cnr::PatternMatch(pattern, G->getEdges());
   }

   {
      cnr::Rule rule = {{{1, 1}, {2, 4, 5}, {7, 5}},
                        {{3, 8}, {2, 7}, {5, 4, 1}, {4, 6}, {5, 1, 7}}};

      cnr::AbstractRelationList find    = rule.first;
      cnr::AbstractRelationList replace = rule.second;
   }

   return 1;
}
