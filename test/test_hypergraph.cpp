#include <ctl/hypergraph.hpp>
#include <gtest/gtest.h>

namespace ctl::test
{
class Foo : public ctl::node<Foo>
{
 public:
   using ctl::node<Foo>::node;
   Foo(size_t value, const char* name) : value_(value), name_(name) {}

 private:
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

TEST_F(HypergraphTestFixture, AddNode) {}

TEST_F(HypergraphTestFixture, RemoveNode) {}

TEST_F(HypergraphTestFixture, AddNodes) {}

TEST_F(HypergraphTestFixture, RemoveNodes) {}

TEST_F(HypergraphTestFixture, AddEdge) {}

TEST_F(HypergraphTestFixture, RemoveEdge) {}

TEST_F(HypergraphTestFixture, RemoveEdges) {}
} // namespace ctl::test
