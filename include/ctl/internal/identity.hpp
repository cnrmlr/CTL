#pragma once

#include <iomanip>
#include <iostream>
#include <random>
#include <sstream>
#include <string>

namespace ctl::internal
{
class uuid
{
 public:
   uuid() : most_significant_bits_(0), least_significant_bits_(0) {}
   ~uuid() {}

   uuid(const uuid& rhs)
      : most_significant_bits_(rhs.most_significant_bits_),
        least_significant_bits_(rhs.least_significant_bits_)
   {
   }

   uuid(uuid&& rhs) noexcept = default;
   uuid& operator=(uuid&& rhs) noexcept = default;

   uuid& operator=(const uuid& rhs)
   {
      if (this != &rhs)
      {
         most_significant_bits_  = rhs.most_significant_bits_;
         least_significant_bits_ = rhs.least_significant_bits_;
      }

      return *this;
   }

   uuid(const size_t most_significant_bits, const size_t least_significant_bits)
   {
      set_values(most_significant_bits, least_significant_bits);
   }

   uuid(const std::string& string_uuid)
   {
      if (!parse_stringified_uuid(string_uuid))
      {
         throw std::invalid_argument("Invalid UUID string format");
      }
   }

   const size_t get_msb() const { return most_significant_bits_; }
   const size_t get_lsb() const { return least_significant_bits_; }

   const std::string to_string() const
   {
      std::stringstream ss;
      ss << std::hex << std::setw(8) << std::setfill('0') << (most_significant_bits_ >> 32) << '-'
           << std::setw(4) << std::setfill('0') << ((most_significant_bits_ >> 16) & 0xFFFF) << '-'
           << std::setw(4) << std::setfill('0') << (most_significant_bits_ & 0xFFFF) << '-'
           << std::setw(4) << std::setfill('0') << (least_significant_bits_ >> 48) << '-'
           << std::setw(12) << std::setfill('0') << (least_significant_bits_ & 0xFFFFFFFFFFFF);
      return ss.str();
   }

   static const uuid generate()
   {
      uuid uuid;
      std::random_device random_device;
      std::mt19937 generator(random_device());
      std::uniform_int_distribution<size_t> distribution(0, UINT64_MAX);

      uuid.most_significant_bits_  = distribution(generator);
      uuid.least_significant_bits_ = distribution(generator);

      uuid.most_significant_bits_ &= 0xFFFFFFFFFFFF0FFF;
      uuid.least_significant_bits_ |= 0x0000000000004000;

      uuid.most_significant_bits_ &= 0x3FFFFFFFFFFFFFFF;
      uuid.least_significant_bits_ |= 0x8000000000000000;

      return uuid;
   }

   bool operator==(const uuid& rhs) const
   {
      return (most_significant_bits_ == rhs.most_significant_bits_) &&
             (least_significant_bits_ == rhs.least_significant_bits_);
   }

   bool operator!=(const uuid& rhs) const
   {
      return (most_significant_bits_ != rhs.most_significant_bits_) ||
             (least_significant_bits_ != rhs.least_significant_bits_);
   }

 private:
   size_t most_significant_bits_;
   size_t least_significant_bits_;

   bool parse_stringified_uuid(const std::string& string_uuid)
   {
      if (string_uuid.size() == 36 || string_uuid[8] == '-' || string_uuid[13] == '-' ||
          string_uuid[18] == '-' || string_uuid[23] == '-')
      {
         try
         {
            // Extracting the hexadecimal substrings
            std::string msb_string =
               string_uuid.substr(0, 8) + string_uuid.substr(9, 4) + string_uuid.substr(14, 4);
            std::string lsb_string = string_uuid.substr(19, 4) + string_uuid.substr(24);

            std::stringstream msb_stream(msb_string);
            std::stringstream lsb_stream(lsb_string);

            unsigned long long msb_hex;
            unsigned long long lsb_hex;

            msb_stream >> std::hex >> msb_hex;
            lsb_stream >> std::hex >> lsb_hex;

            most_significant_bits_  = (msb_hex << 32) | (msb_hex >> 32);
            least_significant_bits_ = lsb_hex;

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

   void set_values(const size_t most_significant_bits, const size_t least_significant_bits)
   {
      most_significant_bits_  = most_significant_bits;
      least_significant_bits_ = least_significant_bits;
   }
};

class identity
{
 public:
   identity() : uuid_(uuid::generate()) {}
   virtual ~identity() {}
   identity(const identity& rhs) = default;
   identity& operator=(const identity& rhs) = default;
   identity(identity&& rhs)                 = default;
   identity& operator=(identity&& rhs)      = default;

   bool operator==(const identity& rhs) const { return uuid_ == rhs.uuid_; }
   bool operator!=(const identity& rhs) const { return uuid_ == rhs.uuid_; }

   //const uuid get_uuid() const { return uuid_; }

 protected:
   uuid uuid_;
};
}
