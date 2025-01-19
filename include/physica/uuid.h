#pragma once

#include <string>

namespace cnr
{
class UUID
{
 public:
   UUID();
   ~UUID();
   UUID(const UUID& rhs);
   UUID& operator=(const UUID& rhs);

   UUID(const size_t mostSignificantBits, const size_t leastSignificantBits);
   UUID(const std::string& stringUUID);

   const size_t getMSB() const;
   const size_t getLSB() const;
   const std::string toString() const;

   static const UUID Generate();

   bool operator==(const UUID& rhs) const;
   bool operator!=(const UUID& rhs) const;

 private:
   size_t mostSignificantBits_;
   size_t leastSignificantBits_;

   bool ParseStringifiedUUID(const std::string& stringifiedUUID);
   void SetValues(const size_t mostSignificantBits, const size_t leastSignificantBits);
};
} // namespace cnr
