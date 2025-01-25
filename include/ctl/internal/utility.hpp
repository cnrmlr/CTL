#pragma once

#include <algorithm>
#include <memory>
#include <vector>

#include <ctl/cxptr.hpp>

namespace ctl::internal
{
// converts a weak ptr to a shared ptr to query the given container using std::find. If the value
// was not found or if the weak ptr could not be locked conatiner.end() is returned.
template <typename T>
typename std::vector<std::shared_ptr<T>>::iterator
find_with_weak_ptr(std::vector<std::shared_ptr<T>>& shared_vector, const cxptr<T>& value)
{
   auto iter = shared_vector.end();

   if (auto shared_ptr = value.lock())
   {
      iter = std::find(shared_vector.begin(), shared_vector.end(), shared_ptr);
   }

   return iter;
}

// converts a vector of shared ptrs to a vector of weak ptrs so that a list objects can be returned
// without increasing the refernce counts of the managed objects.
template <typename T>
std::vector<cxptr<T>> make_weak_ptr_vector(const std::vector<std::shared_ptr<T>>& shared_vector)
{
   std::vector<cxptr<T>> weak_vector;
   weak_vector.reserve(shared_vector.size());

   for (const auto& element : shared_vector)
   {
      weak_vector.emplace_back(element);
   }

   return weak_vector;
}

// converts a vector of weak ptrs to a vector of shared ptrs. If one of the elements in the weak ptr
// vector could not be locked, the conversion will abort and return an empty vector.
template <typename T>
std::vector<std::shared_ptr<T>> make_shared_ptr_vector(const std::vector<cxptr<T>>& weak_vector)
{
   std::vector<std::shared_ptr<T>> shared_vector;
   shared_vector.reserve(weak_vector.size());

   for (auto& element : weak_vector)
   {
      if (auto shared_element = element.lock())
      {
         shared_vector.push_back(shared_element);
      }
      else
      {
         shared_vector.clear();
         break;
      }
   }

   return shared_vector;
}
}
