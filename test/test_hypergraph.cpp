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
   HypergraphTestFixture() : hypergraph_(nullptr) {}

   ~HypergraphTestFixture() {}

 protected:
   std::unique_ptr<hypergraph<Foo>> hypergraph_;

   void SetUp() override { hypergraph_ = std::make_unique<hypergraph<Foo>>(); }

   void TearDown() override { hypergraph_.reset(); }
};

TEST_F(HypergraphTestFixture, AddNodeToHypergraph)
{
   auto node = hypergraph_->add_node(1, "foo");
   ASSERT_TRUE(node.is_valid());
   ASSERT_EQ(node->value_, 1);
   ASSERT_STREQ(node->name_, "foo");
   ASSERT_EQ(node->get_incident_edges().size(), 0);

   auto nodes = hypergraph_->get_nodes();
   ASSERT_EQ(nodes.size(), 1);
   ASSERT_EQ(nodes[0]->value_, 1);
   ASSERT_STREQ(nodes[0]->name_, "foo");
   ASSERT_EQ(nodes[0]->get_incident_edges().size(), 0);

   ASSERT_EQ(node.get(), nodes[0].get());
}

TEST_F(HypergraphTestFixture, RemoveNodeFromHypergraph)
{
   auto node = hypergraph_->add_node(1, "foo");
   hypergraph_->remove_node(node);

   ASSERT_EQ(hypergraph_->get_nodes().size(), 0);
   ASSERT_FALSE(node.is_valid());
}

TEST_F(HypergraphTestFixture, AddNodesToHypergraph)
{
   auto nodes = hypergraph_->add_nodes(10);
   ASSERT_EQ(nodes.size(), 10);
   ASSERT_EQ(hypergraph_->get_nodes().size(), 10);
}

TEST_F(HypergraphTestFixture, RemoveNodesFromHypergraph)
{
   auto nodes = hypergraph_->add_nodes(10);
   hypergraph_->remove_nodes(nodes);
   ASSERT_EQ(hypergraph_->get_nodes().size(), 0);

   for (auto& node : nodes)
   {
      ASSERT_FALSE(node.is_valid());
   }
}

TEST_F(HypergraphTestFixture, AddEdgeToHypergraph)
{
   auto node1 = hypergraph_->add_node(1, "foo");
   auto node2 = hypergraph_->add_node(2, "bar");
   auto edge  = hypergraph_->add_edge({node1, node2});

   ASSERT_TRUE(edge.is_valid());
   ASSERT_EQ(edge->get_incident_nodes().size(), 2);
   ASSERT_EQ(edge->get_incident_nodes()[0]->value_, 1);
   ASSERT_EQ(edge->get_incident_nodes()[1]->value_, 2);
   ASSERT_STREQ(edge->get_incident_nodes()[0]->name_, "foo");
   ASSERT_STREQ(edge->get_incident_nodes()[1]->name_, "bar");

   ASSERT_EQ(node1->get_incident_edges().size(), 1);
   ASSERT_EQ(node2->get_incident_edges().size(), 1);

   auto edges = hypergraph_->get_edges();
   ASSERT_EQ(edges.size(), 1);
   ASSERT_EQ(edges[0]->get_incident_nodes().size(), 2);
   ASSERT_EQ(edges[0]->get_incident_nodes()[0]->value_, 1);
   ASSERT_EQ(edges[0]->get_incident_nodes()[1]->value_, 2);
   ASSERT_STREQ(edges[0]->get_incident_nodes()[0]->name_, "foo");
   ASSERT_STREQ(edges[0]->get_incident_nodes()[1]->name_, "bar");
}

TEST_F(HypergraphTestFixture, RemoveEdgeFromHypergraph)
{
   auto node1 = hypergraph_->add_node(1, "foo");
   auto node2 = hypergraph_->add_node(2, "bar");
   auto edge  = hypergraph_->add_edge({node1, node2});
   hypergraph_->remove_edge(edge);

   ASSERT_EQ(hypergraph_->get_edges().size(), 0);
   ASSERT_FALSE(edge.is_valid());
   ASSERT_EQ(node1->get_incident_edges().size(), 0);
   ASSERT_EQ(node2->get_incident_edges().size(), 0);
}

TEST_F(HypergraphTestFixture, AddEdgesToHypergraph)
{
   auto node1 = hypergraph_->add_node(1, "foo");
   auto node2 = hypergraph_->add_node(2, "bar");
   auto node3 = hypergraph_->add_node(3, "baz");
   auto node4 = hypergraph_->add_node(4, "qux");
   auto node5 = hypergraph_->add_node(5, "quux");
   auto node6 = hypergraph_->add_node(6, "corge");
   auto node7 = hypergraph_->add_node(7, "grault");
   auto node8 = hypergraph_->add_node(8, "garply");
   auto node9 = hypergraph_->add_node(9, "waldo");

   auto edges =
      hypergraph_->add_edges({{node1, node2, node3}, {node4, node5, node6}, {node7, node8, node9}});

   ASSERT_EQ(edges.size(), 3);
   ASSERT_EQ(hypergraph_->get_edges().size(), 3);
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
   auto node1 = hypergraph_->add_node(1, "foo");
   auto node2 = hypergraph_->add_node(2, "bar");
   auto node3 = hypergraph_->add_node(3, "baz");
   auto node4 = hypergraph_->add_node(4, "qux");
   auto edge1 = hypergraph_->add_edge({node1, node2});
   auto edge2 = hypergraph_->add_edge({node3, node4});
   hypergraph_->remove_edges({edge1, edge2});

   ASSERT_EQ(hypergraph_->get_edges().size(), 0);
   ASSERT_FALSE(edge1.is_valid());
   ASSERT_FALSE(edge2.is_valid());
   ASSERT_EQ(node1->get_incident_edges().size(), 0);
   ASSERT_EQ(node2->get_incident_edges().size(), 0);
   ASSERT_EQ(node3->get_incident_edges().size(), 0);
   ASSERT_EQ(node4->get_incident_edges().size(), 0);
}

TEST_F(HypergraphTestFixture, AddNodeToEdge)
{
   auto node1 = hypergraph_->add_node(1, "foo");
   auto node2 = hypergraph_->add_node(2, "bar");
   auto edge  = hypergraph_->add_edge({node1, node2});
   auto node3 = hypergraph_->add_node(3, "baz");

   edge->add_node(node3, edge->get_incident_nodes().begin() + 1);
   ASSERT_EQ(edge->get_incident_nodes().size(), 3);
   ASSERT_EQ(edge->get_incident_nodes()[2]->value_, 3);
   ASSERT_STREQ(edge->get_incident_nodes()[2]->name_, "baz");
   ASSERT_EQ(node3->get_incident_edges().size(), 1);
}
} // namespace ctl::test
