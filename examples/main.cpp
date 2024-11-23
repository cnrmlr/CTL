#include <physica/hypergraph.h>

class Entity
{
public:
   Entity(size_t val)
       : val(val)
   {
      std::cout << "New entity: " << val << std::endl;
   }

   size_t getVal()
   {
      return val;
   }

private:
   size_t val;
};

int main()
{
   // ********************************************
   //
   // Example usage of phys::hypergraph
   //
   // ********************************************

   phys::hypergraph<Entity> entityGraph;

   auto entity = entityGraph.add_node(24); // add node
   auto lockedEntity = entity.lock(); // lock node to access data

   lockedEntity->data().getVal(); // extract data at node
   lockedEntity->uuid(); // read UUID of node
   lockedEntity->get_incident_edges();

   auto entities = entityGraph.add_nodes(3, 24);

   return 1;
}