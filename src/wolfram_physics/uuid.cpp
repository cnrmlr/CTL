#include "physica/wolfram_physics/uuid.h"

#include <iomanip>
#include <iostream>
#include <random>
#include <sstream>

namespace cnr
{
UUID::UUID() : mostSignificantBits_(0), leastSignificantBits_(0) {}

UUID::~UUID() {}

UUID::UUID(const UUID& rhs)
   : mostSignificantBits_(rhs.mostSignificantBits_),
     leastSignificantBits_(rhs.leastSignificantBits_)
{
}

UUID& UUID::operator=(const UUID& rhs)
{
   if (this != &rhs)
   {
      mostSignificantBits_  = rhs.mostSignificantBits_;
      leastSignificantBits_ = rhs.leastSignificantBits_;
   }

   return *this;
}

UUID::UUID(const size_t mostSignificantBits, const size_t leastSignificantBits)
{
   SetValues(mostSignificantBits, leastSignificantBits);
}

UUID::UUID(const std::string& stringUUID)
{
   if (!ParseStringifiedUUID(stringUUID))
   {
      throw std::invalid_argument("Invalid UUID string format");
   }
}

const UUID UUID::Generate()
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

bool UUID::operator==(const UUID& rhs) const
{
   return (mostSignificantBits_ == rhs.mostSignificantBits_) &&
          (leastSignificantBits_ == rhs.leastSignificantBits_);
}

bool UUID::operator!=(const UUID& rhs) const
{
   return (mostSignificantBits_ != rhs.mostSignificantBits_) ||
          (leastSignificantBits_ != rhs.leastSignificantBits_);
}

const std::string UUID::toString() const
{
   std::stringstream l_ss;
   l_ss << std::hex << std::setw(8) << std::setfill('0') << (mostSignificantBits_ >> 32) << '-'
        << std::setw(4) << std::setfill('0') << ((mostSignificantBits_ >> 16) & 0xFFFF) << '-'
        << std::setw(4) << std::setfill('0') << (mostSignificantBits_ & 0xFFFF) << '-'
        << std::setw(4) << std::setfill('0') << (leastSignificantBits_ >> 48) << '-'
        << std::setw(12) << std::setfill('0') << (leastSignificantBits_ & 0xFFFFFFFFFFFF);
   return l_ss.str();
}

void UUID::SetValues(const size_t a_MSB, const size_t a_LSB)
{
   mostSignificantBits_  = a_MSB;
   leastSignificantBits_ = a_LSB;
}

const size_t UUID::getMSB() const { return mostSignificantBits_; }

const size_t UUID::getLSB() const { return leastSignificantBits_; }

bool UUID::ParseStringifiedUUID(const std::string& stringUUID)
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
} // namespace cnr
