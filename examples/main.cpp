#include <hypergraph.hpp>

class Foo : public cnr::Node<Foo>
{
 public:
   Foo() : cnr::Node<Foo>() {}

   ~Foo() {}
};

int main(int argc, char** argv)
{
   // Create a hypergraph with nodes of type foo
   auto graph = std::make_unique<cnr::Hypergraph<Foo>>();

   // Add a single default-constructed node to the hypergraph
   auto node = graph->addNode();

   // Add multiple default-constructed nodes to the hypergraph
   auto nodes = graph->addNodes(3);

   // Add an edge to the hypergraph by specifying the nodes it connects
   auto edge = graph->addEdge({nodes.at(0), nodes.at(1), nodes.at(2)});

   // Add multiple edges to the hypergraph by specifying the nodes they connect
   auto edges = graph->addEdges({{nodes.at(0), nodes.at(1)}, {nodes.at(1), nodes.at(2)}});

   // Remove an edge from the hypergraph
   graph->removeEdge(edge);

   // Remove multiple edges from the hypergraph
   graph->removeEdges(edges);

   // Remove a single node from the hypergraph
   graph->removeNode(node);

   // Remove multiple nodes from the hypergraph
   graph->removeNodes(nodes);

   return 0;
}
