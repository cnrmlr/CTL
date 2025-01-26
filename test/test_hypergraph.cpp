#include <ctl/hypergraph.hpp>
#include <gtest/gtest.h>

namespace ctl::test
{
class Foo : public ctl::node<Foo>
{
 public:
   using ctl::node<Foo>::node;
   Foo(size_t value, const char* name) : value_(value), name_(name) {}
   size_t value_     = 0;
   const char* name_ = "";
};

class HypergraphTestFixture : public ::testing::Test
{
 public:
   HypergraphTestFixture() {}

   ~HypergraphTestFixture() {}

 protected:
   void SetUp() override {}

   void TearDown() override {}
};

TEST_F(HypergraphTestFixture, AddNodeToHypergraph)
{
   hypergraph<Foo> hypergraph;

   auto node = hypergraph.add_node(1, "foo");
   ASSERT_TRUE(node.is_valid());
   ASSERT_EQ(node->value_, 1);
   ASSERT_STREQ(node->name_, "foo");
   ASSERT_EQ(node->get_incident_edges().size(), 0);

   auto nodes = hypergraph.get_nodes();
   ASSERT_EQ(nodes.size(), 1);
   ASSERT_EQ(nodes[0]->value_, 1);
   ASSERT_STREQ(nodes[0]->name_, "foo");
   ASSERT_EQ(nodes[0]->get_incident_edges().size(), 0);

   ASSERT_EQ(node.get(), nodes[0].get());
}

TEST_F(HypergraphTestFixture, RemoveNodeFromHypergraph)
{
   hypergraph<Foo> hypergraph;

   auto node = hypergraph.add_node(1, "foo");
   hypergraph.remove_node(node);

   ASSERT_EQ(hypergraph.get_nodes().size(), 0);
   ASSERT_FALSE(node.is_valid());
}

TEST_F(HypergraphTestFixture, AddNodesToHypergraph)
{
   hypergraph<Foo> hypergraph;

   auto nodes = hypergraph.add_nodes(10);
   ASSERT_EQ(nodes.size(), 10);
   ASSERT_EQ(hypergraph.get_nodes().size(), 10);
}

TEST_F(HypergraphTestFixture, RemoveNodesFromHypergraph)
{
   hypergraph<Foo> hypergraph;

   auto nodes = hypergraph.add_nodes(10);
   hypergraph.remove_nodes(nodes);
   ASSERT_EQ(hypergraph.get_nodes().size(), 0);

   for (auto& node : nodes)
   {
      ASSERT_FALSE(node.is_valid());
   }
}

TEST_F(HypergraphTestFixture, AddEdgeToHypergraph)
{
   hypergraph<Foo> hypergraph;

   auto node1 = hypergraph.add_node(1, "foo");
   auto node2 = hypergraph.add_node(2, "bar");
   auto edge  = hypergraph.add_edge({node1, node2});

   ASSERT_TRUE(edge.is_valid());
   ASSERT_EQ(edge->get_incident_nodes().size(), 2);
   ASSERT_EQ(edge->get_incident_nodes()[0]->value_, 1);
   ASSERT_EQ(edge->get_incident_nodes()[1]->value_, 2);
   ASSERT_STREQ(edge->get_incident_nodes()[0]->name_, "foo");
   ASSERT_STREQ(edge->get_incident_nodes()[1]->name_, "bar");

   ASSERT_EQ(node1->get_incident_edges().size(), 1);
   ASSERT_EQ(node2->get_incident_edges().size(), 1);

   auto edges = hypergraph.get_edges();
   ASSERT_EQ(edges.size(), 1);
   ASSERT_EQ(edges[0]->get_incident_nodes().size(), 2);
   ASSERT_EQ(edges[0]->get_incident_nodes()[0]->value_, 1);
   ASSERT_EQ(edges[0]->get_incident_nodes()[1]->value_, 2);
   ASSERT_STREQ(edges[0]->get_incident_nodes()[0]->name_, "foo");
   ASSERT_STREQ(edges[0]->get_incident_nodes()[1]->name_, "bar");
}

TEST_F(HypergraphTestFixture, RemoveEdgeFromHypergraph)
{
   hypergraph<Foo> hypergraph;

   auto node1 = hypergraph.add_node(1, "foo");
   auto node2 = hypergraph.add_node(2, "bar");
   auto edge  = hypergraph.add_edge({node1, node2});
   hypergraph.remove_edge(edge);

   ASSERT_EQ(hypergraph.get_edges().size(), 0);
   ASSERT_FALSE(edge.is_valid());
   ASSERT_EQ(node1->get_incident_edges().size(), 0);
   ASSERT_EQ(node2->get_incident_edges().size(), 0);
}

TEST_F(HypergraphTestFixture, AddEdgesToHypergraph)
{
   hypergraph<Foo> hypergraph;

   auto node1 = hypergraph.add_node(1, "foo");
   auto node2 = hypergraph.add_node(2, "bar");
   auto node3 = hypergraph.add_node(3, "baz");
   auto node4 = hypergraph.add_node(4, "qux");
   auto node5 = hypergraph.add_node(5, "quux");
   auto node6 = hypergraph.add_node(6, "corge");
   auto node7 = hypergraph.add_node(7, "grault");
   auto node8 = hypergraph.add_node(8, "garply");
   auto node9 = hypergraph.add_node(9, "waldo");

   auto edges =
      hypergraph.add_edges({{node1, node2, node3}, {node4, node5, node6}, {node7, node8, node9}});

   ASSERT_EQ(edges.size(), 3);
   ASSERT_EQ(hypergraph.get_edges().size(), 3);
   ASSERT_EQ(edges[0]->get_incident_nodes().size(), 3);
   ASSERT_EQ(edges[1]->get_incident_nodes().size(), 3);
   ASSERT_EQ(edges[2]->get_incident_nodes().size(), 3);
   ASSERT_EQ(edges[0]->get_incident_nodes()[0]->value_, 1);
   ASSERT_EQ(edges[0]->get_incident_nodes()[1]->value_, 2);
   ASSERT_EQ(edges[0]->get_incident_nodes()[2]->value_, 3);
   ASSERT_EQ(edges[1]->get_incident_nodes()[0]->value_, 4);
   ASSERT_EQ(edges[1]->get_incident_nodes()[1]->value_, 5);
   ASSERT_EQ(edges[1]->get_incident_nodes()[2]->value_, 6);
   ASSERT_EQ(edges[2]->get_incident_nodes()[0]->value_, 7);
   ASSERT_EQ(edges[2]->get_incident_nodes()[1]->value_, 8);
   ASSERT_EQ(edges[2]->get_incident_nodes()[2]->value_, 9);
   ASSERT_STREQ(edges[0]->get_incident_nodes()[0]->name_, "foo");
   ASSERT_STREQ(edges[0]->get_incident_nodes()[1]->name_, "bar");
   ASSERT_STREQ(edges[0]->get_incident_nodes()[2]->name_, "baz");
   ASSERT_STREQ(edges[1]->get_incident_nodes()[0]->name_, "qux");
   ASSERT_STREQ(edges[1]->get_incident_nodes()[1]->name_, "quux");
   ASSERT_STREQ(edges[1]->get_incident_nodes()[2]->name_, "corge");
   ASSERT_STREQ(edges[2]->get_incident_nodes()[0]->name_, "grault");
   ASSERT_STREQ(edges[2]->get_incident_nodes()[1]->name_, "garply");
   ASSERT_STREQ(edges[2]->get_incident_nodes()[2]->name_, "waldo");
   ASSERT_EQ(node1->get_incident_edges().size(), 1);
   ASSERT_EQ(node2->get_incident_edges().size(), 1);
   ASSERT_EQ(node3->get_incident_edges().size(), 1);
   ASSERT_EQ(node4->get_incident_edges().size(), 1);
   ASSERT_EQ(node5->get_incident_edges().size(), 1);
   ASSERT_EQ(node6->get_incident_edges().size(), 1);
   ASSERT_EQ(node7->get_incident_edges().size(), 1);
   ASSERT_EQ(node8->get_incident_edges().size(), 1);
   ASSERT_EQ(node9->get_incident_edges().size(), 1);
}

TEST_F(HypergraphTestFixture, RemoveEdgesFromHypergraph)
{
   hypergraph<Foo> hypergraph;

   auto node1 = hypergraph.add_node(1, "foo");
   auto node2 = hypergraph.add_node(2, "bar");
   auto node3 = hypergraph.add_node(3, "baz");
   auto node4 = hypergraph.add_node(4, "qux");
   auto edge1 = hypergraph.add_edge({node1, node2});
   auto edge2 = hypergraph.add_edge({node3, node4});
   hypergraph.remove_edges({edge1, edge2});

   ASSERT_EQ(hypergraph.get_edges().size(), 0);
   ASSERT_FALSE(edge1.is_valid());
   ASSERT_FALSE(edge2.is_valid());
   ASSERT_EQ(node1->get_incident_edges().size(), 0);
   ASSERT_EQ(node2->get_incident_edges().size(), 0);
   ASSERT_EQ(node3->get_incident_edges().size(), 0);
   ASSERT_EQ(node4->get_incident_edges().size(), 0);
}

TEST_F(HypergraphTestFixture, AddNodeToEdge)
{
   hypergraph<Foo> hypergraph;

   auto node1 = hypergraph.add_node(1, "foo");
   auto node2 = hypergraph.add_node(2, "bar");
   auto edge  = hypergraph.add_edge({node1, node2});
   auto node3 = hypergraph.add_node(3, "baz");
   ASSERT_FALSE(node1->is_adjacent_to(node3));

   edge->add_node(node3);
   ASSERT_EQ(edge->get_incident_nodes().size(), 3);
   ASSERT_EQ(edge->get_incident_nodes()[2]->value_, 3);
   ASSERT_STREQ(edge->get_incident_nodes()[2]->name_, "baz");
   ASSERT_EQ(node3->get_incident_edges().size(), 1);
   ASSERT_TRUE(node3->is_adjacent_to(node1));
   ASSERT_TRUE(node3->is_adjacent_to(node2));
   ASSERT_TRUE(node1->is_incident_to(edge));
   ASSERT_EQ(hypergraph.get_nodes().size(), 3);
}

TEST_F(HypergraphTestFixture, AddNodeToEdgeAtSpecificPoint)
{
   hypergraph<Foo> hypergraph;

   auto node1 = hypergraph.add_node(1, "foo");
   auto node2 = hypergraph.add_node(2, "bar");
   auto edge  = hypergraph.add_edge({node1, node2});
   auto node3 = hypergraph.add_node(3, "baz");
   ASSERT_FALSE(node1->is_adjacent_to(node3));

   edge->add_node(node3, 1);
   ASSERT_EQ(edge->get_incident_nodes().size(), 3);
   ASSERT_EQ(edge->get_incident_nodes()[1]->value_, 3);
   ASSERT_STREQ(edge->get_incident_nodes()[1]->name_, "baz");
   ASSERT_EQ(node3->get_incident_edges().size(), 1);
   ASSERT_TRUE(node3->is_adjacent_to(node1));
   ASSERT_TRUE(node3->is_adjacent_to(node2));
   ASSERT_TRUE(node1->is_incident_to(edge));
   ASSERT_EQ(hypergraph.get_nodes().size(), 3);
}

TEST_F(HypergraphTestFixture, AddNodesToEdge)
{
   hypergraph<Foo> hypergraph;

   auto node1 = hypergraph.add_node(1, "foo");
   auto node2 = hypergraph.add_node(2, "bar");
   auto edge  = hypergraph.add_edge({node1, node2});
   auto node3 = hypergraph.add_node(3, "baz");
   auto node4 = hypergraph.add_node(4, "qux");
   ASSERT_FALSE(node1->is_adjacent_to(node3));
   ASSERT_FALSE(node2->is_adjacent_to(node4));

   edge->add_nodes({node3, node4});
   ASSERT_EQ(edge->get_incident_nodes().size(), 4);
   ASSERT_EQ(edge->get_incident_nodes()[2]->value_, 3);
   ASSERT_STREQ(edge->get_incident_nodes()[2]->name_, "baz");
   ASSERT_EQ(edge->get_incident_nodes()[3]->value_, 4);
   ASSERT_STREQ(edge->get_incident_nodes()[3]->name_, "qux");
   ASSERT_EQ(node3->get_incident_edges().size(), 1);
   ASSERT_EQ(node4->get_incident_edges().size(), 1);
   ASSERT_TRUE(node3->is_adjacent_to(node1));
   ASSERT_TRUE(node3->is_adjacent_to(node2));
   ASSERT_TRUE(node3->is_incident_to(edge));
   ASSERT_TRUE(node4->is_adjacent_to(node1));
   ASSERT_TRUE(node4->is_adjacent_to(node2));
   ASSERT_TRUE(node4->is_incident_to(edge));
   ASSERT_EQ(hypergraph.get_nodes().size(), 4);
}

TEST_F(HypergraphTestFixture, AddNodesToEdgeAtSpecificPoint)
{
   hypergraph<Foo> hypergraph;

   auto node1 = hypergraph.add_node(1, "foo");
   auto node2 = hypergraph.add_node(2, "bar");
   auto edge  = hypergraph.add_edge({node1, node2});
   auto node3 = hypergraph.add_node(3, "baz");
   auto node4 = hypergraph.add_node(4, "qux");
   ASSERT_FALSE(node1->is_adjacent_to(node3));
   ASSERT_FALSE(node2->is_adjacent_to(node4));

   edge->add_nodes({node3, node4}, 1);
   ASSERT_EQ(edge->get_incident_nodes().size(), 4);
   ASSERT_EQ(edge->get_incident_nodes()[1]->value_, 3);
   ASSERT_STREQ(edge->get_incident_nodes()[1]->name_, "baz");
   ASSERT_EQ(edge->get_incident_nodes()[2]->value_, 4);
   ASSERT_STREQ(edge->get_incident_nodes()[2]->name_, "qux");
   ASSERT_EQ(node3->get_incident_edges().size(), 1);
   ASSERT_EQ(node4->get_incident_edges().size(), 1);
   ASSERT_TRUE(node3->is_adjacent_to(node1));
   ASSERT_TRUE(node3->is_adjacent_to(node2));
   ASSERT_TRUE(node3->is_incident_to(edge));
   ASSERT_TRUE(node4->is_adjacent_to(node1));
   ASSERT_TRUE(node4->is_adjacent_to(node2));
   ASSERT_TRUE(node4->is_incident_to(edge));
   ASSERT_EQ(hypergraph.get_nodes().size(), 4);
}
} // namespace ctl::test
