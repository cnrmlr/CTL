#pragma once

#include "identifiable_base.h"

#include <memory>
#include <vector>

namespace cnr
{
class Node;

template <typename T>
class Hyperedge : public IdentifiableBase, public std::enable_shared_from_this<Hyperedge<T>>
{
   friend class Hypergraph<T>;

 public:
   Hyperedge();
   Hyperedge(const std::vector<std::shared_ptr<T>>& nodes);
   ~Hyperedge();
   Hyperedge(const Hyperedge& rhs) = delete;
   Hyperedge operator=(const Hyperedge& rhs) = delete;

   const std::vector<std::weak_ptr<T>> getNodes() const;

   bool isAdjacentTo(const std::weak_ptr<Hyperedge<T>>& edge);
   bool isIncidentTo(const std::weak_ptr<T>& node);

   bool operator==(const Hyperedge& rhs) const;
   bool operator!=(const Hyperedge& rhs) const;

 protected:
   const std::vector<std::shared_ptr<T>>::iterator findNode(std::weak_ptr<T> node);
   void removeNode(std::weak_ptr<T> node);
   void removeFromNodeIncidenceLists();

 private:
   std::vector<std::shared_ptr<T>> nodes_;
};
} // namespace cnr
