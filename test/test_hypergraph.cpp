#include "physica/wolfram_physics.h"
#include <gtest/gtest.h>

namespace cnr
{
namespace test
{
class HypergraphTest : public ::testing::Test
{
 public:
   HypergraphTest() : hypergraph_(nullptr) {}

   ~HypergraphTest() {}

 protected:
   std::unique_ptr<Hypergraph> hypergraph_;

   void SetUp() override { hypergraph_ = std::make_unique<Hypergraph>(); }

   void TearDown() override { hypergraph_.reset(); }
};

TEST_F(HypergraphTest, AddNode)
{
   std::weak_ptr<Node> node = hypergraph_->addNode();

   ASSERT_TRUE(node.lock());
   ASSERT_EQ(hypergraph_->getNodes().size(), 1);
   ASSERT_EQ(hypergraph_->getNodes().at(0).lock(), node.lock());
}

TEST_F(HypergraphTest, RemoveNode)
{
   std::weak_ptr<Node> node = hypergraph_->addNode();
   hypergraph_->removeNode(node);

   ASSERT_TRUE(node.expired());
   ASSERT_TRUE(hypergraph_->getNodes().empty());
}

TEST_F(HypergraphTest, AddNodes)
{
   std::vector<std::weak_ptr<Node>> nodes = hypergraph_->addNodes(3);

   ASSERT_EQ(nodes.size(), 3);
   ASSERT_EQ(hypergraph_->getNodes().size(), 3);

   for (size_t i = 0; i < nodes.size() && i < hypergraph_->getNodes().size(); ++i)
   {
      ASSERT_TRUE(nodes.at(i).lock());
      ASSERT_EQ(nodes.at(i).lock(), hypergraph_->getNodes().at(i).lock());
   }
}

TEST_F(HypergraphTest, RemoveNodes)
{
   std::vector<std::weak_ptr<Node>> nodes = hypergraph_->addNodes(3);
   hypergraph_->removeNodes(nodes);

   ASSERT_TRUE(hypergraph_->getNodes().empty());

   for (auto& node : nodes)
   {
      ASSERT_TRUE(node.expired());
   }
}

TEST_F(HypergraphTest, AddEdge)
{
   std::vector<std::weak_ptr<Node>> nodes = hypergraph_->addNodes(3);
   std::weak_ptr<Hyperedge> edge          = hypergraph_->addEdge(nodes);

   ASSERT_TRUE(edge.lock());
   ASSERT_EQ(hypergraph_->getEdges().size(), 1);
   ASSERT_EQ(hypergraph_->getEdges().at(0).lock(), edge.lock());

   for (size_t i = 0; i < nodes.size() && i < edge.lock()->getNodes().size(); ++i)
   {
      ASSERT_EQ(nodes.at(i).lock(), edge.lock()->getNodes().at(i).lock());
   }
}

TEST_F(HypergraphTest, RemoveEdge)
{
   std::vector<std::weak_ptr<Node>> nodes = hypergraph_->addNodes(3);
   std::weak_ptr<Hyperedge> edge          = hypergraph_->addEdge(nodes);
   hypergraph_->removeEdge(edge);

   ASSERT_TRUE(edge.expired());
   ASSERT_TRUE(hypergraph_->getEdges().empty());
}

TEST_F(HypergraphTest, RemoveEdges)
{
   std::vector<std::weak_ptr<Node>> nodeSet_1 = hypergraph_->addNodes(3);
   std::vector<std::weak_ptr<Node>> nodeSet_2 = hypergraph_->addNodes(3);

   std::weak_ptr<Hyperedge> edge_1 = hypergraph_->addEdge(nodeSet_1);
   std::weak_ptr<Hyperedge> edge_2 = hypergraph_->addEdge(nodeSet_2);

   hypergraph_->removeEdges({edge_1, edge_2});

   ASSERT_TRUE(edge_1.expired());
   ASSERT_TRUE(edge_2.expired());
   ASSERT_TRUE(hypergraph_->getEdges().empty());
}
} // namespace test
} // namespace cnr
