#pragma once

#include <iomanip>
#include <iostream>
#include <memory>
#include <random>
#include <sstream>
#include <string>
#include <type_traits>
#include <vector>

namespace cnr
{

// converts a weak ptr to a shared ptr to query the given container using std::find. If the value
// was not found or if the weak ptr could not be locked conatiner.end() is returned.
template <typename T>
typename std::vector<std::shared_ptr<T>>::iterator
FindWithWeakPtr(std::vector<std::shared_ptr<T>>& sharedVector, const std::weak_ptr<T>& value)
{
   auto iter = sharedVector.end();

   if (auto sharedPtr = value.lock())
   {
      iter = std::find(sharedVector.begin(), sharedVector.end(), sharedPtr);
   }

   return iter;
}

// converts a vector of shared ptrs to a vector of weak ptrs so that a list objects can be returned
// without increasing the refernce counts of the managed objects.
template <typename T>
std::vector<std::weak_ptr<T>> MakeWeakPtrVector(const std::vector<std::shared_ptr<T>>& sharedVector)
{
   std::vector<std::weak_ptr<T>> weakVector;
   weakVector.reserve(sharedVector.size());

   for (const auto& element : sharedVector)
   {
      weakVector.emplace_back(element);
   }

   return weakVector;
}

// converts a vector of weak ptrs to a vector of shared ptrs. If one of the elements in the weak ptr
// vector could not be locked, the conversion will abort and return an empty vector.
template <typename T>
std::vector<std::shared_ptr<T>> MakeSharedPtrVector(const std::vector<std::weak_ptr<T>>& weakVector)
{
   std::vector<std::shared_ptr<T>> sharedVector;
   sharedVector.reserve(weakVector.size());

   for (auto& element : weakVector)
   {
      if (auto sharedElement = element.lock())
      {
         sharedVector.push_back(sharedElement);
      }
      else
      {
         sharedVector.clear();
         break;
      }
   }

   return sharedVector;
}

class UUID
{
 public:
   UUID() : mostSignificantBits_(0), leastSignificantBits_(0) {}
   ~UUID() {}

   UUID(const UUID& rhs)
      : mostSignificantBits_(rhs.mostSignificantBits_),
        leastSignificantBits_(rhs.leastSignificantBits_)
   {
   }

   UUID& operator=(const UUID& rhs)
   {
      if (this != &rhs)
      {
         mostSignificantBits_  = rhs.mostSignificantBits_;
         leastSignificantBits_ = rhs.leastSignificantBits_;
      }

      return *this;
   }

   UUID(const size_t mostSignificantBits, const size_t leastSignificantBits)
   {
      SetValues(mostSignificantBits, leastSignificantBits);
   }

   UUID(const std::string& stringUUID)
   {
      if (!ParseStringifiedUUID(stringUUID))
      {
         throw std::invalid_argument("Invalid UUID string format");
      }
   }

   const size_t getMSB() const { return mostSignificantBits_; }
   const size_t getLSB() const { return leastSignificantBits_; }

   const std::string toString() const
   {
      std::stringstream ss;
      ss << std::hex << std::setw(8) << std::setfill('0') << (mostSignificantBits_ >> 32) << '-'
           << std::setw(4) << std::setfill('0') << ((mostSignificantBits_ >> 16) & 0xFFFF) << '-'
           << std::setw(4) << std::setfill('0') << (mostSignificantBits_ & 0xFFFF) << '-'
           << std::setw(4) << std::setfill('0') << (leastSignificantBits_ >> 48) << '-'
           << std::setw(12) << std::setfill('0') << (leastSignificantBits_ & 0xFFFFFFFFFFFF);
      return ss.str();
   }

   static const UUID Generate()
   {
      UUID uuid;
      std::random_device randomDevice;
      std::mt19937 generator(randomDevice());
      std::uniform_int_distribution<size_t> distribution(0, UINT64_MAX);

      uuid.mostSignificantBits_  = distribution(generator);
      uuid.leastSignificantBits_ = distribution(generator);

      uuid.mostSignificantBits_ &= 0xFFFFFFFFFFFF0FFF;
      uuid.leastSignificantBits_ |= 0x0000000000004000;

      uuid.mostSignificantBits_ &= 0x3FFFFFFFFFFFFFFF;
      uuid.leastSignificantBits_ |= 0x8000000000000000;

      return uuid;
   }

   bool operator==(const UUID& rhs) const
   {
      return (mostSignificantBits_ == rhs.mostSignificantBits_) &&
             (leastSignificantBits_ == rhs.leastSignificantBits_);
   }

   bool operator!=(const UUID& rhs) const
   {
      return (mostSignificantBits_ != rhs.mostSignificantBits_) ||
             (leastSignificantBits_ != rhs.leastSignificantBits_);
   }

 private:
   size_t mostSignificantBits_;
   size_t leastSignificantBits_;

   bool ParseStringifiedUUID(const std::string& stringUUID)
   {
      if (stringUUID.size() == 36 || stringUUID[8] == '-' || stringUUID[13] == '-' ||
          stringUUID[18] == '-' || stringUUID[23] == '-')
      {
         try
         {
            // Extracting the hexadecimal substrings
            std::string msbString =
               stringUUID.substr(0, 8) + stringUUID.substr(9, 4) + stringUUID.substr(14, 4);
            std::string lsbString = stringUUID.substr(19, 4) + stringUUID.substr(24);

            std::stringstream msbStream(msbString);
            std::stringstream lsbStream(lsbString);

            unsigned long long msbHex;
            unsigned long long lsbHex;

            msbStream >> std::hex >> msbHex;
            lsbStream >> std::hex >> lsbHex;

            mostSignificantBits_  = (msbHex << 32) | (msbHex >> 32);
            leastSignificantBits_ = lsbHex;

            return true;
         }
         catch (const std::exception&)
         {
            return false;
         }
      }
      else
      {
         return false;
      }
   }

   void SetValues(const size_t mostSignificantBits, const size_t leastSignificantBits)
   {
      mostSignificantBits_  = mostSignificantBits;
      leastSignificantBits_ = leastSignificantBits;
   }
};

class IdentifiableBase
{
 public:
   IdentifiableBase() : uuid_(UUID::Generate()) {}
   virtual ~IdentifiableBase() {}
   IdentifiableBase(const IdentifiableBase& rhs) = delete;
   IdentifiableBase& operator=(const IdentifiableBase& rhs) = delete;

   bool operator==(const IdentifiableBase& rhs) const { return uuid_ == rhs.getUUID(); }
   bool operator!=(const IdentifiableBase& rhs) const { return uuid_ != rhs.getUUID(); }

   const UUID getUUID() const { return uuid_; }

 protected:
   UUID uuid_;
};

template <typename NodeType>
class Node;

template <typename NodeType>
class Hyperedge;

template <typename NodeType>
class Hypergraph;

template <typename NodeType>
class Node : public IdentifiableBase
{
   template <typename U>
   friend class Hyperedge;

   template <typename U>
   friend class Hypergraph;

 public:
   Node();
   ~Node();
   Node(const Node& rhs) = delete;
   Node operator=(const Node& rhs) = delete;

   const std::vector<std::weak_ptr<Hyperedge<NodeType>>> getIncidentEdges();

   bool isAdjacentTo(const std::weak_ptr<Node<NodeType>>& vertex);
   bool isIncidentTo(const std::weak_ptr<Hyperedge<NodeType>>& edge);

   bool operator==(const Node<NodeType>& rhs) const;
   bool operator!=(const Node<NodeType>& rhs) const;

 protected:
   void addIncidentEdge(const std::weak_ptr<Hyperedge<NodeType>>& incidentEdge);
   void removeIncidentEdge(const std::weak_ptr<Hyperedge<NodeType>>& incidentEdge);

 private:
   std::vector<std::shared_ptr<Hyperedge<NodeType>>> incidentEdges_;
};

template <typename NodeType>
class Hyperedge : public IdentifiableBase, public std::enable_shared_from_this<Hyperedge<NodeType>>
{
   template <typename U>
   friend class Hypergraph;

 public:
   Hyperedge();
   Hyperedge(const std::vector<std::shared_ptr<NodeType>>& nodes);
   ~Hyperedge();
   Hyperedge(const Hyperedge<NodeType>& rhs) = delete;
   Hyperedge operator=(const Hyperedge<NodeType>& rhs) = delete;

   const std::vector<std::weak_ptr<NodeType>> getNodes() const;

   bool isAdjacentTo(const std::weak_ptr<Hyperedge<NodeType>>& edge);
   bool isIncidentTo(const std::weak_ptr<NodeType>& node);

   bool operator==(const Hyperedge<NodeType>& rhs) const;
   bool operator!=(const Hyperedge<NodeType>& rhs) const;

 protected:
   const std::vector<std::shared_ptr<NodeType>>::iterator findNode(std::weak_ptr<NodeType> node);
   void removeNode(std::weak_ptr<NodeType> node);
   void removeFromNodeIncidenceLists();

 private:
   std::vector<std::shared_ptr<NodeType>> nodes_;
};

template <typename NodeType>
class Hypergraph : public IdentifiableBase
{
 public:
   Hypergraph();
   ~Hypergraph();
   Hypergraph(const Hypergraph&) = delete;
   Hypergraph operator=(const Hypergraph) = delete;

   const std::weak_ptr<NodeType> addNode();
   void removeNode(const std::weak_ptr<NodeType>& Node);

   const std::vector<std::weak_ptr<NodeType>> addNodes(const size_t count);
   void removeNodes(const std::vector<std::weak_ptr<NodeType>>& nodes);

   const std::weak_ptr<Hyperedge<NodeType>> addEdge(std::vector<std::weak_ptr<NodeType>> nodes);
   void removeEdge(const std::weak_ptr<Hyperedge<NodeType>>& edge);

   const std::vector<std::weak_ptr<Hyperedge<NodeType>>>
   addEdges(std::vector<std::vector<std::weak_ptr<NodeType>>> nodeSets);
   void removeEdges(const std::vector<std::weak_ptr<Hyperedge<NodeType>>>& edges);

   const std::vector<std::weak_ptr<NodeType>> getNodes() const;
   const std::vector<std::weak_ptr<Hyperedge<NodeType>>> getEdges() const;

   std::vector<std::vector<unsigned short>> getAdajencyMatrix() const;

 private:
   std::vector<std::shared_ptr<NodeType>> nodes_;
   std::vector<std::shared_ptr<Hyperedge<NodeType>>> edges_;
};

template <typename NodeType>
Node<NodeType>::Node() : IdentifiableBase()
{
}

template <typename NodeType>
Node<NodeType>::~Node()
{
}

template <typename NodeType>
const std::vector<std::weak_ptr<Hyperedge<NodeType>>> Node<NodeType>::getIncidentEdges()
{
   return MakeWeakPtrVector(incidentEdges_);
}

template <typename NodeType>
bool Node<NodeType>::isAdjacentTo(const std::weak_ptr<Node>& node)
{
   for (auto& edge : incidentEdges_)
   {
      if (edge->isIncidentTo(node))
      {
         return true;
      }
   }

   return false;
}

template <typename NodeType>
bool Node<NodeType>::isIncidentTo(const std::weak_ptr<Hyperedge<NodeType>>& edge)
{
   return FindWithWeakPtr(incidentEdges_, edge) != incidentEdges_.end();
}

template <typename NodeType>
void Node<NodeType>::addIncidentEdge(const std::weak_ptr<Hyperedge<NodeType>>& incidentEdge)
{
   incidentEdges_.emplace_back(incidentEdge);
}

template <typename NodeType>
void Node<NodeType>::removeIncidentEdge(const std::weak_ptr<Hyperedge<NodeType>>& incidentEdge)
{
   auto edgeIter = FindWithWeakPtr(incidentEdges_, incidentEdge);

   if (edgeIter != incidentEdges_.end())
   {
      incidentEdges_.erase(edgeIter);
   }
}

template <typename NodeType>
bool Node<NodeType>::operator==(const Node<NodeType>& rhs) const
{
   return IdentifiableBase::operator==(rhs);
}

template <typename NodeType>
bool Node<NodeType>::operator!=(const Node<NodeType>& rhs) const
{
   return IdentifiableBase::operator!=(rhs);
}

// ----- Hyperedge -----

template <typename NodeType>
Hyperedge<NodeType>::Hyperedge() : IdentifiableBase(), nodes_(0)
{
}

template <typename NodeType>
Hyperedge<NodeType>::Hyperedge(const std::vector<std::shared_ptr<NodeType>>& nodes)
   : IdentifiableBase()
{
   nodes_.reserve(nodes.size());

   for (auto& node : nodes)
   {
      nodes_.push_back(node);
   }
}

template <typename NodeType>
Hyperedge<NodeType>::~Hyperedge()
{
}

template <typename NodeType>
const std::vector<std::weak_ptr<NodeType>> Hyperedge<NodeType>::getNodes() const
{
   return MakeWeakPtrVector(nodes_);
}

template <typename NodeType>
bool Hyperedge<NodeType>::isAdjacentTo(const std::weak_ptr<Hyperedge<NodeType>>& edge)
{
   for (auto& node : nodes_)
   {
      if (edge.lock() && edge.lock()->isIncidentTo(node))
      {
         return true;
      }
   }

   return false;
}

template <typename NodeType>
bool Hyperedge<NodeType>::isIncidentTo(const std::weak_ptr<NodeType>& vertex)
{
   return findNode(vertex) != nodes_.end();
}

template <typename NodeType>
const std::vector<std::shared_ptr<NodeType>>::iterator
Hyperedge<NodeType>::findNode(std::weak_ptr<NodeType> node)
{
   return FindWithWeakPtr(nodes_, node);
}

template <typename NodeType>
void Hyperedge<NodeType>::removeNode(std::weak_ptr<NodeType> node)
{
   auto nodeIter = findNode(node);

   if (nodeIter != nodes_.end())
   {
      nodes_.erase(findNode(node));
   }
}

template <typename NodeType>
void Hyperedge<NodeType>::removeFromNodeIncidenceLists()
{
   for (auto& node : nodes_)
   {
      node->removeIncidentEdge(shared_from_this());
   }
}

template <typename NodeType>
bool Hyperedge<NodeType>::operator==(const Hyperedge<NodeType>& rhs) const
{
   return IdentifiableBase::operator==(rhs);
}

template <typename NodeType>
bool Hyperedge<NodeType>::operator!=(const Hyperedge<NodeType>& rhs) const
{
   return IdentifiableBase::operator!=(rhs);
}

// ----- Hypergraph -----

template <typename NodeType>
Hypergraph<NodeType>::Hypergraph() : IdentifiableBase()
{
}

template <typename NodeType>
Hypergraph<NodeType>::~Hypergraph()
{
}

template <typename NodeType>
const std::weak_ptr<NodeType> Hypergraph<NodeType>::addNode()
{
   std::shared_ptr<NodeType> node = std::make_shared<NodeType>();
   return nodes_.emplace_back(node);
}

template <typename NodeType>
void Hypergraph<NodeType>::removeNode(const std::weak_ptr<NodeType>& node)
{
   for (auto& edge : edges_)
   {
      edge->removeNode(node);
   }

   auto nodeIter = FindWithWeakPtr(nodes_, node);

   if (nodeIter != nodes_.end())
   {
      nodes_.erase(nodeIter);
   }
}

template <typename NodeType>
const std::vector<std::weak_ptr<NodeType>> Hypergraph<NodeType>::addNodes(size_t count)
{
   std::vector<std::weak_ptr<NodeType>> newNodes;
   newNodes.reserve(count);

   for (size_t i = 0; i < count; ++i)
   {
      newNodes.emplace_back(addNode());
   }

   return newNodes;
}

template <typename NodeType>
void Hypergraph<NodeType>::removeNodes(const std::vector<std::weak_ptr<NodeType>>& nodes)
{
   for (auto& node : nodes)
   {
      removeNode(node);
   }
}

template <typename NodeType>
const std::weak_ptr<Hyperedge<NodeType>>
Hypergraph<NodeType>::addEdge(std::vector<std::weak_ptr<NodeType>> nodes)
{
   std::vector<std::shared_ptr<NodeType>> sharedNodes = MakeSharedPtrVector(nodes);

   if (sharedNodes.empty())
   {
      return std::shared_ptr<Hyperedge<NodeType>>(nullptr);
   }

   std::shared_ptr<Hyperedge<NodeType>> edge = std::make_shared<Hyperedge<NodeType>>(sharedNodes);

   edges_.push_back(edge);

   for (auto& node : sharedNodes)
   {
      node->addIncidentEdge(edge);
   }

   return edge;
}

template <typename NodeType>
void Hypergraph<NodeType>::removeEdge(const std::weak_ptr<Hyperedge<NodeType>>& edge)
{
   auto edgeIter = FindWithWeakPtr(edges_, edge);

   if (edgeIter != edges_.end())
   {
      edgeIter->get()->removeFromNodeIncidenceLists();
      edges_.erase(edgeIter);
   }
}

template <typename NodeType>
const std::vector<std::weak_ptr<Hyperedge<NodeType>>>
Hypergraph<NodeType>::addEdges(std::vector<std::vector<std::weak_ptr<NodeType>>> nodeSets)
{
   std::vector<std::weak_ptr<Hyperedge<NodeType>>> newEdges;
   newEdges.reserve(nodeSets.size());

   for (auto& set : nodeSets)
   {
      newEdges.emplace_back(addEdge(set));
   }

   return newEdges;
}

template <typename NodeType>
void Hypergraph<NodeType>::removeEdges(const std::vector<std::weak_ptr<Hyperedge<NodeType>>>& edges)
{
   for (auto& edge : edges)
   {
      removeEdge(edge);
   }
}

template <typename NodeType>
const std::vector<std::weak_ptr<NodeType>> Hypergraph<NodeType>::getNodes() const
{
   return MakeWeakPtrVector(nodes_);
}

template <typename NodeType>
const std::vector<std::weak_ptr<Hyperedge<NodeType>>> Hypergraph<NodeType>::getEdges() const
{
   return MakeWeakPtrVector(edges_);
}

template <typename NodeType>
std::vector<std::vector<unsigned short>> Hypergraph<NodeType>::getAdajencyMatrix() const
{
   std::vector<std::vector<unsigned short>> adjacencyMatrix;
   adjacencyMatrix.reserve(nodes_.size());

   for (auto& node_i : nodes_)
   {
      std::vector<unsigned short> adjacencyList;
      adjacencyList.reserve(nodes_.size());

      for (auto& node_j : nodes_)
      {
         adjacencyList.push_back(static_cast<unsigned short>(node_i->isAdjacentTo(node_j)));
      }

      adjacencyMatrix.push_back(adjacencyList);
   }

   return adjacencyMatrix;
}
} // namespace cnr
