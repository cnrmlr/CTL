#include <ctl/hypergraph.hpp>

class Foo : public ctl::node<Foo>
{
 public:
   using ctl::node<Foo>::node;
   Foo(size_t value, const char* name) : value_(value), name_(name) {}

 private:
   size_t value_     = 0;
   const char* name_ = "";
};

int main(int argc, char** argv)
{
   using namespace ctl;

   // Create a hypergraph with nodes of type foo
   hypergraph<Foo> graph;

   // Add a single default-constructed node to the hypergraph
   cxptr<Foo> node = graph.add_node();

   // Add a single node with custom arguments to the hypergraph
   cxptr<Foo> custom_node = graph.add_node(42, "custom node");

   // Add a single node by moving an existing object to the hypergraph
   Foo foo(42, "moved node");
   cxptr<Foo> moved_node = graph.add_node(std::move(foo));

   // Add multiple default-constructed nodes to the hypergraph
   std::vector<cxptr<Foo>> nodes = graph.add_nodes(3);

   // Add an edge to the hypergraph by specifying the nodes it connects
   cxptr<hyperedge<Foo>> edge = graph.add_edge({nodes.at(0), nodes.at(1), nodes.at(2)});

   // Add multiple edges to the hypergraph by specifying the nodes they connect
   std::vector<cxptr<hyperedge<Foo>>> edges =
      graph.add_edges({{nodes.at(0), nodes.at(1)}, {nodes.at(1), nodes.at(2)}});

   // Remove an edge from the hypergraph
   graph.remove_edge(edge);

   // Remove multiple edges from the hypergraph
   graph.remove_edges(edges);

   // Remove a single node from the hypergraph
   graph.remove_node(node);

   // Remove multiple nodes from the hypergraph
   graph.remove_nodes(nodes);

   if (node.is_valid())
   {
      // Access the managed object
      Foo& foo = node.get();

      // Access the incident edges of the node
      auto incident_edges = foo.get_incident_edges();

      // Access the managed object with a const reference
      const Foo& const_foo = node.get();

      // Get a shared_ptr if the object still exists
      auto shared_foo = node.get_shared_ptr();
   }
   else
   {
      // The node no longer exists and cannot be accessed
   }

   return 0;
}
