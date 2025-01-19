#pragma once

#include "uuid.h"

namespace cnr
{
class IdentifiableBase
{
 public:
   IdentifiableBase();
   virtual ~IdentifiableBase();
   IdentifiableBase(const IdentifiableBase& rhs) = delete;
   IdentifiableBase& operator=(const IdentifiableBase& rhs) = delete;

   bool operator==(const IdentifiableBase& rhs) const;
   bool operator!=(const IdentifiableBase& rhs) const;

   const UUID getUUID() const;

 protected:
   UUID uuid_;
};
} // namespace cnr
