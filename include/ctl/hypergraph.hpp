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
    node(const node&) = default;
    node& operator=(const node&) = default;
    node(node&&) noexcept = default;
    node& operator=(node&&) noexcept = default;
    ~node() = default;

   std::vector<cxptr<hyperedge<T>>> get_incident_edges() const;
   std::vector<cxptr<T>> get_adjacent_nodes() const;

   bool is_incident_to(const cxptr<hyperedge<T>>& edge) const;
   bool is_adjacent_to(const cxptr<T>& vertex) const;

   bool operator==(const T& rhs) const;
   bool operator!=(const T& rhs) const;

 protected:
   //void add_incident_edge(const cxptr<hyperedge<T>>& incident_edge);
   //void remove_incident_edge(const cxptr<hyperedge<T>>& incident_edge);

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
   hyperedge(const hyperedge<T>&) = default;
   hyperedge& operator=(const hyperedge<T>&) = default;
   hyperedge(hyperedge<T>&&) = default;
   hyperedge& operator=(hyperedge<T>&&) = default;
   virtual ~hyperedge() = default;

   void add_node(cxptr<T>& node);
   void add_node(cxptr<T>& node, size_t insertion_point);
   void remove_node(size_t at_index);

   void add_nodes(std::vector<cxptr<T>> nodes);
   void add_nodes(std::vector<cxptr<T>> nodes, size_t insertion_point);
   void remove_nodes(size_t from_index, size_t to_index);

   std::vector<cxptr<T>> get_incident_nodes() const;
   std::vector<cxptr<hyperedge<T>>> get_adjacent_edges() const;

   bool is_incident_to(const cxptr<T>& node) const;
   bool is_adjacent_to(const cxptr<hyperedge<T>>& edge) const;

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
   hypergraph(const hypergraph&) = default;
   hypergraph& operator=(const hypergraph&) = default;
   hypergraph(hypergraph&&) = default;
   hypergraph& operator=(hypergraph&&) = default;
   virtual ~hypergraph() = default;

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

   for (const std::shared_ptr<hyperedge<T>>& edge : incident_edges_)
   {
      for (const std::shared_ptr<T>& other_node : edge->incident_nodes_)
      {
         if (other_node.get() != this)
         {
            adjacent_nodes.emplace_back(other_node);
         }
      }
   }

   return adjacent_nodes;
}


template <typename T>
bool node<T>::is_adjacent_to(const cxptr<T>& node) const
{
   if (node.is_valid())
   {
      const std::shared_ptr<T>& locked_node = node.lock();

      for (const std::shared_ptr<hyperedge<T>>& edge : incident_edges_)
      {
         const std::vector<std::shared_ptr<T>>& edge_nodes = edge->incident_nodes_;
         typename std::vector<std::shared_ptr<T>>::const_iterator iter = std::find(edge_nodes.begin(), edge_nodes.end(), locked_node);

         if (iter != edge_nodes.end())
         {
            return true;
         }
      }
   }

   return false;
}

template <typename T>
bool node<T>::is_incident_to(const cxptr<hyperedge<T>>& edge) const
{
   if (edge.is_valid())
   {
      return std::find(incident_edges_.begin(), incident_edges_.begin(), edge.lock()) != incident_edges_.end();
   }

   return false;
}

// template <typename T>
// void node<T>::add_incident_edge(const cxptr<hyperedge<T>>& incident_edge)
// {
//    if (incident_edge.is_valid())
//    {
//       incident_edges_.emplace_back(incident_edge.lock());
//    }
// }

// template <typename T>
// void node<T>::remove_incident_edge(const cxptr<hyperedge<T>>& incident_edge)
// {
//    if (incident_edge.is_valid())
//    {
//       std::erase(std::remove(incident_edges_.begin(), incident_edges_.end(), incident_edge.lock()));
//    }
// }

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
std::vector<cxptr<T>> hyperedge<T>::get_incident_nodes() const
{
   return internal::make_weak_ptr_vector(incident_nodes_);
}

template <typename T>
std::vector<cxptr<hyperedge<T>>> hyperedge<T>::get_adjacent_edges() const
{
   std::vector<cxptr<hyperedge<T>>> adjacent_edges;

   for (const std::shared_ptr<T>& node : incident_nodes_)
   {
      for (const std::shared_ptr<hyperedge<T>>& edge : node->incident_edges_)
      {
         if (edge.get() != this)
         {
            adjacent_edges.emplace_back(edge);
         }
      }
   }

   return adjacent_edges;
}

template <typename T>
bool hyperedge<T>::is_adjacent_to(const cxptr<hyperedge<T>>& edge) const
{
   if (edge.is_valid())
   {
      const std::vector<std::shared_ptr<T>>& other_edge_nodes = edge.get().incident_nodes_;

      for (const std::shared_ptr<T>& node : incident_nodes_)
      {
         typename std::vector<std::shared_ptr<T>>::const_iterator iter = std::find(other_edge_nodes.begin(), other_edge_nodes.end(), node);

         if (iter != other_edge_nodes.end())
         {
            return true;
         }
      }
   }

   return false;
}

template <typename T>
bool hyperedge<T>::is_incident_to(const cxptr<T>& node) const
{
   if (node.is_valid())
   {
      return std::find(incident_nodes_.begin(), incident_nodes_.end(), node.lock()) != incident_nodes_.end();
   }

   return false;
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
      std::vector<std::shared_ptr<hyperedge<T>>>& edge_list = node.get().incident_edges_;
      std::shared_ptr<hyperedge<T>> this_edge = this->shared_from_this();
      typename std::vector<std::shared_ptr<hyperedge<T>>>::iterator iter = 
         std::find(edge_list.begin(), edge_list.end(), this_edge);

      if (iter == edge_list.end())
      {
         edge_list.emplace_back(this_edge);
      }
   }
}

template <typename T>
void hyperedge<T>::add_node(cxptr<T>& node, size_t insertion_point)
{
   if (node.is_valid() && insertion_point < incident_nodes_.size())
   {
      incident_nodes_.emplace(incident_nodes_.begin() + insertion_point, node.lock());
      std::vector<std::shared_ptr<hyperedge<T>>>& edge_list = node.get().incident_edges_;
      std::shared_ptr<hyperedge<T>> this_edge = this->shared_from_this();
      typename std::vector<std::shared_ptr<hyperedge<T>>>::iterator iter = 
         std::find(edge_list.begin(), edge_list.end(), this_edge);

      if (iter == edge_list.end())
      {
         edge_list.emplace_back(this_edge);
      }
   }
}

template <typename T>
void hyperedge<T>::add_nodes(std::vector<cxptr<T>> nodes)
{
   std::vector<std::shared_ptr<T>> locked_nodes;
   locked_nodes.reserve(nodes.size());
   
   std::for_each(nodes.begin(), nodes.end(), [&locked_nodes](const cxptr<T>& node) {
      if (node.is_valid()) {
         locked_nodes.push_back(node.lock());
      }
   });

   if (locked_nodes.size() == nodes.size())
   {
      std::shared_ptr<hyperedge<T>> this_edge = this->shared_from_this();
      
      std::for_each(locked_nodes.begin(), locked_nodes.end(), [this, &this_edge](const std::shared_ptr<T>& node) {
         incident_nodes_.push_back(node);
         auto& node_edges = node->incident_edges_;
         
         if (std::find(node_edges.begin(), node_edges.end(), this_edge) == node_edges.end()) {
            node_edges.push_back(this_edge);
         }
      });
   }
}

template <typename T>
void hyperedge<T>::add_nodes(std::vector<cxptr<T>> nodes, size_t insertion_point)
{
   if (insertion_point > incident_nodes_.size()) {
      return;
   }

   std::vector<std::shared_ptr<T>> locked_nodes;
   locked_nodes.reserve(nodes.size());
   
   std::for_each(nodes.begin(), nodes.end(), [&locked_nodes](const cxptr<T>& node) {
      if (node.is_valid()) {
         locked_nodes.push_back(node.lock());
      }
   });

   if (locked_nodes.size() == nodes.size())
   {
      incident_nodes_.insert(incident_nodes_.begin() + insertion_point, 
                           locked_nodes.begin(), 
                           locked_nodes.end());

      std::shared_ptr<hyperedge<T>> this_edge = this->shared_from_this();
      std::for_each(locked_nodes.begin(), locked_nodes.end(), [&this_edge](const std::shared_ptr<T>& node) {
         auto& node_edges = node->incident_edges_;
         if (std::find(node_edges.begin(), node_edges.end(), this_edge) == node_edges.end()) {
            node_edges.push_back(this_edge);
         }
      });
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
         removed_node->incident_edges_.erase(this->shared_from_this());
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
            removed_node->incident_edges_.erase(this->shared_from_this());
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
   std::vector<std::shared_ptr<T>> locked_nodes;
   locked_nodes.reserve(nodes.size());

   std::copy_if(nodes.begin(), nodes.end(), std::back_inserter(locked_nodes),
      [](const cxptr<T>& node) { return node.is_valid(); });
   
   if (locked_nodes.size() != nodes.size())
   {
      return std::shared_ptr<hyperedge<T>>(nullptr);
   }

   std::shared_ptr<hyperedge<T>> edge = edges_.emplace_back(std::make_shared<hyperedge<T>>(locked_nodes));

   std::for_each(locked_nodes.begin(), locked_nodes.end(), [&edge](std::shared_ptr<T>& node)
   {
      node->incident_edges_.emplace_back(edge);
   });

   return edge;
}

template <typename T>
void hypergraph<T>::remove_edge(const cxptr<hyperedge<T>>& edge)
{
   if (edge.is_valid())
   {
      typename std::vector<std::shared_ptr<hyperedge<T>>>::iterator iter 
         = std::find(edges_.begin(), edges_.end(), edge.lock());

      if (iter != edges_.end())
      {
         std::shared_ptr<hyperedge<T>>& removed_edge = *iter;
         std::vector<std::shared_ptr<T>>& incident_nodes = removed_edge->incident_nodes_;

         std::for_each(incident_nodes.begin(), incident_nodes.end(), [&removed_edge](std::shared_ptr<T>& node)
         {
            std::vector<std::shared_ptr<hyperedge<T>>>& incident_edges = node->incident_edges_;

            incident_edges.erase(std::remove_if(incident_edges.begin(), incident_edges.end(), 
               [&removed_edge](std::shared_ptr<hyperedge<T>>& incident_edge)
            {
               return incident_edge == removed_edge;
            }));
         });
      }

      edges_.erase(iter);
   }
}

template <typename T>
std::vector<cxptr<hyperedge<T>>>
hypergraph<T>::add_edges(std::vector<std::vector<cxptr<T>>> node_sets)
{
   std::vector<std::vector<std::shared_ptr<T>>> locked_node_sets;
   locked_node_sets.reserve(node_sets.size());

   std::for_each(node_sets.begin(), node_sets.end(), [&locked_node_sets](std::vector<cxptr<T>>& nodes)
   {
      std::vector<std::shared_ptr<T>> locked_nodes;
      locked_nodes.reserve(nodes.size());

      std::copy_if(nodes.begin(), nodes.end(), std::back_inserter(locked_nodes),
         [](const cxptr<T>& node) { return node.is_valid(); });

      if (locked_nodes.size() == nodes.size())
      {
         locked_node_sets.emplace_back(locked_nodes);
      }
   });

   if (locked_node_sets.size() == node_sets.size())
   {
      size_t original_size = edges_.size();

      std::for_each(locked_node_sets.begin(), locked_node_sets.end(), [this](std::vector<std::shared_ptr<T>>& node_set)
      {
         std::shared_ptr<hyperedge<T>> edge = edges_.emplace_back(std::make_shared<hyperedge<T>>(node_set));

         std::for_each(node_set.begin(), node_set.end(), [&edge](std::shared_ptr<T>& node)
         {
            node->incident_edges_.emplace_back(edge);
         });
      });

      // Create a vector of the newly added edges
      std::vector<cxptr<hyperedge<T>>> new_edges;
      new_edges.reserve(edges_.size() - original_size);
      for (size_t i = original_size; i < edges_.size(); ++i) {
         new_edges.emplace_back(edges_[i]);
      }
      return new_edges;
   }

   return std::vector<cxptr<hyperedge<T>>>();
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
