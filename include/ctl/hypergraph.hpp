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

   std::vector<cxptr<hyperedge<T>>> get_incident_edges() const;
   std::vector<cxptr<T>> get_adjacent_nodes() const;

   bool is_incident_to(const cxptr<hyperedge<T>>& edge);
   bool is_adjacent_to(const cxptr<T>& vertex);

   bool operator==(const T& rhs) const;
   bool operator!=(const T& rhs) const;

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
   friend class node;

   template <typename U>
   friend class hypergraph;

 public:
   hyperedge();
   hyperedge(const std::vector<std::shared_ptr<T>>& nodes);
   ~hyperedge();
   hyperedge(const hyperedge<T>& rhs) = delete;
   hyperedge operator=(const hyperedge<T>& rhs) = delete;

   void add_node(cxptr<T>& node);
   void add_node(cxptr<T>& node, size_t insertion_point);
   void remove_node(size_t at_index);

   void add_nodes(std::vector<cxptr<T>> nodes);
   void add_nodes(std::vector<cxptr<T>> nodes, size_t insertion_point);
   void remove_nodes(size_t from_index, size_t to_index);

   std::vector<cxptr<T>> get_incident_nodes() const;
   std::vector<cxptr<hyperedge<T>>> get_adjacent_edges() const;

   bool is_incident_to(const cxptr<T>& node);
   bool is_adjacent_to(const cxptr<hyperedge<T>>& edge);

   bool operator==(const hyperedge<T>& rhs) const;
   bool operator!=(const hyperedge<T>& rhs) const;

 protected:
   const std::vector<std::shared_ptr<T>>::iterator find_node(cxptr<T> node);

 private:
   std::vector<std::shared_ptr<T>> incident_nodes_;
};

template <typename T>
class hypergraph : public internal::identity
{
   template <typename U>
   friend class node;

   template <typename U>
   friend class hyperedge;

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
std::vector<cxptr<hyperedge<T>>> node<T>::get_incident_edges() const
{
   return internal::make_weak_ptr_vector(incident_edges_);
}

template <typename T>
std::vector<cxptr<T>> node<T>::get_adjacent_nodes() const
{
   std::vector<cxptr<T>> adjacent_nodes;

   for (auto& edge : incident_edges_)
   {
      auto nodes = edge->get_nodes();

      for (auto& node : nodes)
      {
         if (node->get_uuid() != this->get_uuid())
         {
            adjacent_nodes.emplace_back(node);
         }
      }
   }

   return adjacent_nodes;
}

template <typename T>
bool node<T>::is_adjacent_to(const cxptr<T>& node)
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
bool node<T>::operator==(const T& rhs) const
{
   return internal::identity::operator==(rhs);
}

template <typename T>
bool node<T>::operator!=(const T& rhs) const
{
   return internal::identity::operator!=(rhs);
}

// ----- hyperedge -----

template <typename T>
hyperedge<T>::hyperedge() : internal::identity(), incident_nodes_(0)
{
}

template <typename T>
hyperedge<T>::hyperedge(const std::vector<std::shared_ptr<T>>& nodes)
   : internal::identity()
{
   incident_nodes_.reserve(nodes.size());

   for (auto& node : nodes)
   {
      incident_nodes_.push_back(node);
   }
}

template <typename T>
hyperedge<T>::~hyperedge()
{
}

template <typename T>
std::vector<cxptr<T>> hyperedge<T>::get_incident_nodes() const
{
   return internal::make_weak_ptr_vector(incident_nodes_);
}

template <typename T>
std::vector<cxptr<hyperedge<T>>> hyperedge<T>::get_adjacent_edges() const
{
   std::vector<cxptr<hyperedge<T>>> adjacent_edges;

   for (auto& node : incident_nodes_)
   {
      for (auto& edge : node->get_incident_edges())
      {
         if (edge->get_uuid() != this->get_uuid())
         {
            adjacent_edges.emplace_back(edge);
         }
      }
   }

   return adjacent_edges;
}

template <typename T>
bool hyperedge<T>::is_adjacent_to(const cxptr<hyperedge<T>>& edge)
{
   for (auto& node : incident_nodes_)
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
   return find_node(vertex) != incident_nodes_.end();
}

template <typename T>
const std::vector<std::shared_ptr<T>>::iterator
hyperedge<T>::find_node(cxptr<T> node)
{
   return internal::find_with_weak_ptr(incident_nodes_, node);
}

template <typename T>
void hyperedge<T>::add_node(cxptr<T>& node)
{
   if (node.is_valid())
   {
      incident_nodes_.emplace_back(node.lock());
      ctl::node<T>& inserted_node = node.get();
      std::shared_ptr<hyperedge<T>> this_edge = this->shared_from_this();

      if (!inserted_node.is_incident_to(this_edge))
      {
         inserted_node.add_incident_edge(this_edge);
      }
   }
}

template <typename T>
void hyperedge<T>::add_node(cxptr<T>& node, size_t insertion_point)
{
   if (node.is_valid() && insertion_point < incident_nodes_.size())
   {
      incident_nodes_.emplace(incident_nodes_.begin() + insertion_point, node.lock());
      ctl::node<T>& inserted_node = node.get();
      std::shared_ptr<hyperedge<T>> this_edge = this->shared_from_this();

      if (!inserted_node.is_incident_to(this_edge))
      {
         inserted_node.add_incident_edge(this_edge);
      }
   }
}

template <typename T>
void hyperedge<T>::add_nodes(std::vector<cxptr<T>> nodes)
{
   std::vector<std::shared_ptr<T>> locked_nodes = internal::make_shared_ptr_vector(nodes);

   if (!locked_nodes.empty())
   {
      for (auto locked_node : locked_nodes)
      {
         incident_nodes_.emplace_back(locked_node);
      }

      std::shared_ptr<hyperedge<T>> this_edge = this->shared_from_this();

      for (std::shared_ptr<T>& node : locked_nodes)
      {
         ctl::node<T>& inserted_node = *node;

         if (!inserted_node.is_incident_to(this_edge))
         {
            inserted_node.add_incident_edge(this_edge);
         }
      }
   }
}

template <typename T>
void hyperedge<T>::add_nodes(std::vector<cxptr<T>> nodes, size_t insertion_point)
{
   std::vector<std::shared_ptr<T>> locked_nodes = internal::make_shared_ptr_vector(nodes);

   if (!locked_nodes.empty() && insertion_point < incident_nodes_.size())
   {
      for (auto locked_node : locked_nodes)
      {
         incident_nodes_.emplace(incident_nodes_.begin() + insertion_point++, locked_node);
      }

      std::shared_ptr<hyperedge<T>> this_edge = this->shared_from_this();

      for (std::shared_ptr<T>& node : locked_nodes)
      {
         ctl::node<T>& inserted_node = *node;

         if (!inserted_node.is_incident_to(this_edge))
         {
            inserted_node.add_incident_edge(this_edge);
         }
      }
   }
}

template <typename T>
void hyperedge<T>::remove_node(size_t at_index)
{
   if (at_index < incident_nodes_.size())
   {
      typename std::vector<std::shared_ptr<T>>::iterator iter = incident_nodes_.begin() + at_index;
      std::shared_ptr<T> removed_node = *iter;
      incident_nodes_.erase(iter);

      if (std::find(incident_nodes_.begin(), incident_nodes_.end(), removed_node) == incident_nodes_.end())
      {
         // it is possible to have the same node multiple times in the edge
         // so we only remove the incident edge reference from the node once
         // all instances of that node are removed from the edge.
         removed_node->remove_incident_edge(this->shared_from_this());
      }
   }
}

template <typename T>
void hyperedge<T>::remove_nodes(size_t from_index, size_t to_index)
{
   if (to_index < from_index && from_index < incident_nodes_.size())
   {
      typename std::vector<std::shared_ptr<T>>::iterator from_iter = incident_nodes_.begin() + from_index;
      typename std::vector<std::shared_ptr<T>>::iterator to_iter = incident_nodes_.begin() + to_index;
      typename std::vector<std::shared_ptr<T>> removed_nodes(from_iter, to_iter);
      incident_nodes_.erase(from_iter, to_iter);

      for (std::shared_ptr<T>& removed_node : removed_nodes)
      {
         if (std::find(incident_nodes_.begin(), incident_nodes_.end(), removed_node) == incident_nodes_.end())
         {
            removed_node->remove_incident_edge(this->shared_from_this());
         }
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
cxptr<T> hypergraph<T>::add_node(T&& node)
{
   return nodes_.emplace_back(std::make_shared<T>(std::forward<T>(node)));
}

template <typename T>
void hypergraph<T>::remove_node(const cxptr<T>& node)
{
   if (node.is_valid())
   {
      for (auto& edge : edges_)
      {
         std::vector<std::shared_ptr<T>>& edge_nodes = edge->incident_nodes_;
         edge_nodes.erase(std::remove(edge_nodes.begin(), edge_nodes.end(), node.lock()));
      }

      nodes_.erase(std::remove(nodes_.begin(), nodes_.end(), node.lock()));
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
      for (auto& node : internal::make_shared_ptr_vector(edge_iter->get()->get_incident_nodes()))
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
} // namespace ctl
