#pragma once

#include <memory>
#include <stdexcept>
#include <type_traits>
#include <vector>

#include <ctl/cxptr.hpp>
#include <ctl/internal/identity.hpp>
#include <ctl/internal/utility.hpp>

namespace ctl
{
template <typename T>
class node;

template <typename T>
class hyperedge;

template <typename T>
class hypergraph;

template <typename T>
class node : public internal::identity
{
   template <typename U>
   friend class hyperedge;

   template <typename U>
   friend class hypergraph;

 public:
    node();
    node(node&&) noexcept = default;
    node& operator=(node&&) noexcept = default;
    node(const node&) = delete;
    node& operator=(const node&) = delete;

   std::vector<cxptr<hyperedge<T>>> get_incident_edges();

   bool is_adjacent_to(const cxptr<node<T>>& vertex);
   bool is_incident_to(const cxptr<hyperedge<T>>& edge);

   bool operator==(const node<T>& rhs) const;
   bool operator!=(const node<T>& rhs) const;

 protected:
   void add_incident_edge(const cxptr<hyperedge<T>>& incident_edge);
   void remove_incident_edge(const cxptr<hyperedge<T>>& incident_edge);

 private:
   std::vector<std::shared_ptr<hyperedge<T>>> incident_edges_;
};

template <typename T>
class hyperedge : public internal::identity, public std::enable_shared_from_this<hyperedge<T>>
{
   template <typename U>
   friend class hypergraph;

 public:
   hyperedge();
   hyperedge(const std::vector<std::shared_ptr<T>>& nodes);
   ~hyperedge();
   hyperedge(const hyperedge<T>& rhs) = delete;
   hyperedge operator=(const hyperedge<T>& rhs) = delete;

   std::vector<std::shared_ptr<T>>::iterator add_node(const cxptr<T>& node);
   std::vector<std::shared_ptr<T>>::iterator add_nodes(const std::vector<cxptr<T>>& nodes);

   // since the same node can be added multiple times to an edge, removing a specific node
   // requires an iterator pointing to the specific node to be removed. without the iterator, 
   // the first instance of the node would be removed.
   void remove_node(typename std::vector<std::shared_ptr<T>>::iterator node_iter);

   //
   void remove_nodes(typename std::vector<std::shared_ptr<T>>::iterator node_iter, size_t count);
   
   std::vector<cxptr<T>> get_nodes() const;

   bool is_adjacent_to(const cxptr<hyperedge<T>>& edge);
   bool is_incident_to(const cxptr<T>& node);

   bool operator==(const hyperedge<T>& rhs) const;
   bool operator!=(const hyperedge<T>& rhs) const;

 protected:
   const std::vector<std::shared_ptr<T>>::iterator find_node(cxptr<T> node);

 private:
   std::vector<std::shared_ptr<T>> nodes_;
};

template <typename T>
class hypergraph : public internal::identity
{
 public:
   hypergraph();
   ~hypergraph();
   hypergraph(const hypergraph&) = delete;
   hypergraph operator=(const hypergraph) = delete;

   template <typename... Args>
   cxptr<T> add_node(Args&&... args);

   cxptr<T> add_node(T&& node);

   void remove_node(const cxptr<T>& node);

   std::vector<cxptr<T>> add_nodes(const size_t count);
   void remove_nodes(const std::vector<cxptr<T>>& nodes);

   cxptr<hyperedge<T>> add_edge(std::vector<cxptr<T>> nodes);
   void remove_edge(const cxptr<hyperedge<T>>& edge);

   std::vector<cxptr<hyperedge<T>>>
   add_edges(std::vector<std::vector<cxptr<T>>> node_sets);
   void remove_edges(const std::vector<cxptr<hyperedge<T>>>& edges);

   std::vector<cxptr<T>> get_nodes() const;
   std::vector<cxptr<hyperedge<T>>> get_edges() const;

   std::vector<std::vector<unsigned short>> get_adjacency_matrix() const;

 private:
   std::vector<std::shared_ptr<T>> nodes_;
   std::vector<std::shared_ptr<hyperedge<T>>> edges_;
};

// ----- node -----
template <typename T>
node<T>::node() : internal::identity()
{
}

template <typename T>
std::vector<cxptr<hyperedge<T>>> node<T>::get_incident_edges()
{
   return internal::make_weak_ptr_vector(incident_edges_);
}

template <typename T>
bool node<T>::is_adjacent_to(const cxptr<node>& node)
{
   for (auto& edge : incident_edges_)
   {
      if (edge->is_incident_to(node))
      {
         return true;
      }
   }

   return false;
}

template <typename T>
bool node<T>::is_incident_to(const cxptr<hyperedge<T>>& edge)
{
   return internal::find_with_weak_ptr(incident_edges_, edge) != incident_edges_.end();
}

template <typename T>
void node<T>::add_incident_edge(const cxptr<hyperedge<T>>& incident_edge)
{
   incident_edges_.emplace_back(incident_edge);
}

template <typename T>
void node<T>::remove_incident_edge(const cxptr<hyperedge<T>>& incident_edge)
{
   auto edge_iter = internal::find_with_weak_ptr(incident_edges_, incident_edge);

   if (edge_iter != incident_edges_.end())
   {
      incident_edges_.erase(edge_iter);
   }
}

template <typename T>
bool node<T>::operator==(const node<T>& rhs) const
{
   return internal::identity::operator==(rhs);
}

template <typename T>
bool node<T>::operator!=(const node<T>& rhs) const
{
   return internal::identity::operator!=(rhs);
}

// ----- hyperedge -----

template <typename T>
hyperedge<T>::hyperedge() : internal::identity(), nodes_(0)
{
}

template <typename T>
hyperedge<T>::hyperedge(const std::vector<std::shared_ptr<T>>& nodes)
   : internal::identity()
{
   nodes_.reserve(nodes.size());

   for (auto& node : nodes)
   {
      nodes_.push_back(node);
   }
}

template <typename T>
hyperedge<T>::~hyperedge()
{
}

template <typename T>
std::vector<cxptr<T>> hyperedge<T>::get_nodes() const
{
   return internal::make_weak_ptr_vector(nodes_);
}

template <typename T>
bool hyperedge<T>::is_adjacent_to(const cxptr<hyperedge<T>>& edge)
{
   for (auto& node : nodes_)
   {
      if (edge.lock() && edge.lock()->is_incident_to(node))
      {
         return true;
      }
   }

   return false;
}

template <typename T>
bool hyperedge<T>::is_incident_to(const cxptr<T>& vertex)
{
   return find_node(vertex) != nodes_.end();
}

template <typename T>
const std::vector<std::shared_ptr<T>>::iterator
hyperedge<T>::find_node(cxptr<T> node)
{
   return internal::find_with_weak_ptr(nodes_, node);
}

template <typename T>
inline std::vector<std::shared_ptr<T>>::iterator hyperedge<T>::add_node(const cxptr<T>& node)
{
   auto nodes_iter = nodes_.end();
   
   if (node.is_valid())
   {
      nodes_iter = nodes_.emplace_back(node.lock());
   }

   return nodes_iter;
}

template <typename T>
inline std::vector<std::shared_ptr<T>>::iterator
hyperedge<T>::add_nodes(const std::vector<cxptr<T>>& nodes)
{
   auto nodes_iter = nodes_.end();
   auto shared_nodes = internal::make_shared_ptr_vector(nodes);

   if (!shared_nodes.empty())
   {
      nodes_iter = nodes_.insert(nodes_.end(), shared_nodes.begin(), shared_nodes.end());
   }

   return nodes_iter;
}

template <typename T>
inline void hyperedge<T>::remove_node(typename std::vector<std::shared_ptr<T>>::iterator node_iter)
{
   if (node_iter != nodes_.end())
   {
      auto node = *node_iter;
      nodes_.erase(node_iter);

      if (find_node(node) == nodes_.end())
      {
         // it is possible to have the same node multiple times in the edge
         // so we only remove the incident edge reference from the node once
         // all instances of that node are removed from the edge.
         node->remove_incident_edge(this->shared_from_this());
      }
   }
}

template <typename T>
inline void hyperedge<T>::remove_nodes(typename std::vector<std::shared_ptr<T>>::iterator node_iter,
                                       size_t count)
{
   // Ensure we don't go beyond the end of the vector
   if (node_iter + count > nodes_.end())
   {
      count = std::distance(node_iter, nodes_.end());
   }

   // Remove the sequence of nodes at once
   std::vector<std::shared_ptr<T>> removed_nodes(node_iter, node_iter + count);
   nodes_.erase(node_iter, node_iter + count);

   // Check if all instances of each node were removed, then remove incident edge
   for (const auto& node : removed_nodes)
   {
      if (find_node(node) == nodes_.end())
      {
         node->remove_incident_edge(this->shared_from_this());
      }
   }
}

template <typename T>
bool hyperedge<T>::operator==(const hyperedge<T>& rhs) const
{
   return internal::identity::operator==(rhs);
}

template <typename T>
bool hyperedge<T>::operator!=(const hyperedge<T>& rhs) const
{
   return internal::identity::operator!=(rhs);
}

// ----- hypergraph -----

template <typename T>
hypergraph<T>::hypergraph() : internal::identity()
{
}

template <typename T>
hypergraph<T>::~hypergraph()
{
}

template <typename T>
template <typename... Args>
cxptr<T> hypergraph<T>::add_node(Args&&... args)
{
   std::shared_ptr<T> node = std::make_shared<T>(args...);
   return nodes_.emplace_back(node);
}

template <typename T>
inline cxptr<T> hypergraph<T>::add_node(T&& node)
{
   return nodes_.emplace_back(std::make_shared<T>(std::forward<T>(node)));
}

template <typename T>
void hypergraph<T>::remove_node(const cxptr<T>& node)
{
   for (auto& edge : edges_)
   {
      // remove all instances of the node from all edges it is incident to
      auto shared_nodes = internal::make_shared_ptr_vector(edge->get_nodes());
      auto node_iter = internal::find_with_weak_ptr(shared_nodes, node);

      while (node_iter != shared_nodes.end())
      {
         edge->remove_node(node_iter);
         node_iter = internal::find_with_weak_ptr(shared_nodes, node);
      }
   }

   auto node_iter = internal::find_with_weak_ptr(nodes_, node);

   if (node_iter != nodes_.end())
   {
      nodes_.erase(node_iter);
   }
}

template <typename T>
std::vector<cxptr<T>> hypergraph<T>::add_nodes(size_t count)
{
   std::vector<cxptr<T>> new_nodes;
   new_nodes.reserve(count);

   for (size_t i = 0; i < count; ++i)
   {
      new_nodes.emplace_back(add_node());
   }

   return new_nodes;
}

template <typename T>
void hypergraph<T>::remove_nodes(const std::vector<cxptr<T>>& nodes)
{
   for (auto& node : nodes)
   {
      remove_node(node);
   }
}

template <typename T>
cxptr<hyperedge<T>>
hypergraph<T>::add_edge(std::vector<cxptr<T>> nodes)
{
   std::vector<std::shared_ptr<T>> shared_nodes = internal::make_shared_ptr_vector(nodes);

   if (shared_nodes.empty())
   {
      return std::shared_ptr<hyperedge<T>>(nullptr);
   }

   std::shared_ptr<hyperedge<T>> edge = std::make_shared<hyperedge<T>>(shared_nodes);

   edges_.push_back(edge);

   for (auto& node : shared_nodes)
   {
      node->add_incident_edge(edge);
   }

   return edge;
}

template <typename T>
void hypergraph<T>::remove_edge(const cxptr<hyperedge<T>>& edge)
{
   auto edge_iter = internal::find_with_weak_ptr(edges_, edge);

   if (edge_iter != edges_.end())
   {
      for (auto& node : internal::make_shared_ptr_vector(edge_iter->get()->get_nodes()))
      {
         node->remove_incident_edge(edge);
      }

      edges_.erase(edge_iter);
   }
}

template <typename T>
std::vector<cxptr<hyperedge<T>>>
hypergraph<T>::add_edges(std::vector<std::vector<cxptr<T>>> node_sets)
{
   std::vector<cxptr<hyperedge<T>>> new_edges;
   new_edges.reserve(node_sets.size());

   for (auto& set : node_sets)
   {
      new_edges.emplace_back(add_edge(set));
   }

   return new_edges;
}

template <typename T>
void hypergraph<T>::remove_edges(const std::vector<cxptr<hyperedge<T>>>& edges)
{
   for (auto& edge : edges)
   {
      remove_edge(edge);
   }
}

template <typename T>
std::vector<cxptr<T>> hypergraph<T>::get_nodes() const
{
   return internal::make_weak_ptr_vector(nodes_);
}

template <typename T>
std::vector<cxptr<hyperedge<T>>> hypergraph<T>::get_edges() const
{
   return internal::make_weak_ptr_vector(edges_);
}

template <typename T>
std::vector<std::vector<unsigned short>> hypergraph<T>::get_adjacency_matrix() const
{
   std::vector<std::vector<unsigned short>> adjacency_matrix;
   adjacency_matrix.reserve(nodes_.size());

   for (auto& node_i : nodes_)
   {
      std::vector<unsigned short> adjacency_list;
      adjacency_list.reserve(nodes_.size());

      for (auto& node_j : nodes_)
      {
         adjacency_list.push_back(static_cast<unsigned short>(node_i->is_adjacent_to(node_j)));
      }

      adjacency_matrix.push_back(adjacency_list);
   }

   return adjacency_matrix;
}
} // namespace cnr
