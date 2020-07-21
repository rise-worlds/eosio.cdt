//
// Created by rise on 2019/12/5.
//

#ifndef EOSIO_CDT_UINT256_T_HPP
#define EOSIO_CDT_UINT256_T_HPP
#pragma once
#include "../capi/eosio/types.h"
#include "fixed_bytes.hpp"

namespace eosio
{
   class uint256_t {

   public:
      // for idx256
      typedef uint128_t word_t;
      static constexpr uint8_t Size = 32;
      static constexpr size_t num_words() { return (Size + sizeof(word_t) - 1) / sizeof(word_t); }
      static constexpr uint8_t WIDTH = 4;

      // for idx256
      uint128_t* data() const { return (uint128_t*)uivalue; }
      uint128_t* data() { return (uint128_t*)uivalue; }
      // operator capi_name &() { return cvalue; }
      // operator const capi_name &() { return cvalue; }
      operator capi_name *() { return &cvalue; }
      operator const capi_name *() { return &cvalue; }

      constexpr uint256_t() { };
      constexpr uint256_t(uint256_t& v) { for (int i = 0; i < WIDTH; ++i) value[i] = v.value[i]; }
      constexpr uint256_t(const uint256_t& v) { for (int i = 0; i < WIDTH; ++i) value[i] = v.value[i]; }
      constexpr uint256_t(uint64_t v) { value[0] = v; }
//      constexpr uint256_t(uint128_t v) { pzValue[0] = v & 0xFFFFFFFFFFFFFFFFull; pzValue[1] = v >> 64; }
      constexpr uint256_t(uint64_t v1, uint64_t v2, uint64_t v3, uint64_t v4) { value[0] = v1; value[1] = v2; value[2] = v3; value[3] = v4; }
//      constexpr uint256_t(capi_name v) { for (int i = 0; i < WIDTH; ++i) pzValue[i] = v[i]; }
      constexpr uint256_t(const capi_name& v) { for (int i = 0; i < WIDTH; ++i) value[i] = v.data[i]; }

      constexpr uint256_t& operator=(const uint256_t& v) {
         if (&v == this) return *this;
         for (int i = 0; i < WIDTH; ++i)
            value[i] = v.value[i];
         return *this;
      }
      constexpr uint256_t& operator=(uint64_t b)
      {
         value[0] = b;
         for (int i = 1; i < WIDTH; i++)
            value[i] = 0;
         return *this;
      }
      const uint256_t operator~() const
      {
         uint256_t ret;
         for (int i = 0; i < WIDTH; i++)
            ret.value[i] = ~value[i];
         return ret;
      }
      const uint256_t operator-() const
      {
         uint256_t ret;
         for (int i = 0; i < WIDTH; i++)
            ret.value[i] = ~value[i];
         ++ret;
         return ret;
      }
      uint256_t& operator+=(const uint256_t& b)
      {
         uint64_t carry = 0;
         for (int i = 0; i < WIDTH; i++)
         {
            __uint128_t n = carry + value[i] + b.value[i];
            value[i] = n & 0xffffffffffffffffull;
            carry = n >> 64;
         }
         return *this;
      }
      uint256_t& operator-=(const uint256_t& b)
      {
         *this += -b;
         return *this;
      }

      uint256_t& operator+=(uint64_t b64)
      {
         uint256_t b;
         b = b64;
         *this += b;
         return *this;
      }
      uint256_t& operator-=(uint64_t b64)
      {
         uint256_t b;
         b = b64;
         *this += -b;
         return *this;
      }

      uint256_t& operator++()
      {
         // prefix operator
         int i = 0;
         while (i < WIDTH && ++value[i] == 0)
            i++;
         return *this;
      }

      const uint256_t operator++(int)
      {
         // postfix operator
         const uint256_t ret = *this;
         ++(*this);
         return ret;
      }

      uint256_t& operator--()
      {
         // prefix operator
         int i = 0;
         while (i < WIDTH && --value[i] == std::numeric_limits<uint64_t>::max())
            i++;
         return *this;
      }

      const uint256_t operator--(int)
      {
         // postfix operator
         const uint256_t ret = *this;
         --(*this);
         return ret;
      }

      uint256_t& operator^=(const uint256_t& b)
      {
         for (int i = 0; i < WIDTH; i++)
            value[i] ^= b.value[i];
         return *this;
      }

      constexpr uint256_t& operator&=(const uint256_t& b)
      {
         for (int i = 0; i < WIDTH; i++)
            value[i] &= b.value[i];
         return *this;
      }

      constexpr uint256_t& operator|=(const uint256_t& b)
      {
         for (int i = 0; i < WIDTH; i++)
            value[i] |= b.value[i];
         return *this;
      }

      constexpr uint256_t& operator^=(uint64_t b)
      {
         value[0] ^= b;
         return *this;
      }

      constexpr uint256_t& operator|=(uint64_t b)
      {
         value[0] |= b;
         return *this;
      }

      constexpr uint256_t& operator&=(uint64_t b)
      {
         value[0] &= b;
         return *this;
      }

      constexpr uint256_t& operator<<=(unsigned int shift)
      {
         uint256_t a(*this);
         for (int i = 0; i < WIDTH; i++)
            value[i] = 0;
         int k = shift / 64;
         shift = shift % 64;
         for (int i = 0; i < WIDTH; i++) {
            if (i + k + 1 < WIDTH && shift != 0)
               value[i + k + 1] |= (a.value[i] >> (64 - shift));
            if (i + k < WIDTH)
               value[i + k] |= (a.value[i] << shift);
         }
         return *this;
      }

      constexpr uint256_t& operator>>=(unsigned int shift)
      {
         uint256_t a(*this);
         for (int i = 0; i < WIDTH; i++)
            value[i] = 0;
         int k = shift / 64;
         shift = shift % 64;
         for (int i = 0; i < WIDTH; i++) {
            if (i - k - 1 >= 0 && shift != 0)
               value[i - k - 1] |= (a.value[i] << (64 - shift));
            if (i - k >= 0)
               value[i - k] |= (a.value[i] >> shift);
         }
         return *this;
      }

      operator bool() const
      {
         for (int i = 0; i < WIDTH; i++) {
            if (value[i])
               return true;
         }
         return false;
      }
//      bool operator >(uint64_t v) const
//      {
//         uint256_t b = v;
//         return *this > b;
//      }
//      uint64_t operator&(const uint256_t& v) {
//         uint256_t ret = *this;
//         ret &= uint256_t(v);
//         return ret.pzValue[0];
//      }

      friend const uint256_t operator&(const uint256_t& a, const uint256_t& b) { return uint256_t(a) &= uint256_t(b); }
      friend constexpr uint64_t operator&(const uint256_t& a, uint64_t b) {
         uint256_t ret = a;
         ret &= uint256_t(b);
         return ret.value[0];
      }
      friend constexpr bool operator>(const uint256_t &a, const uint256_t &b) {
         for (int i = 0; i < WIDTH; ++i) {
            if (a.value[i] > b.value[i])
               return true;
         }
         return false;
      }
      friend constexpr bool operator>(const uint256_t &a, uint64_t v) {
         uint256_t b = v;
         return a > b;
      }
      friend constexpr bool operator<(const uint256_t &a, const uint256_t &b) {
         for (int i = 0; i < WIDTH; ++i) {
            if (a.value[i] < b.value[i])
               return true;
         }
         return false;
      }
      friend constexpr bool operator<(const uint256_t &a, uint64_t v) {
         uint256_t b = v;
         return a < b;
      }
      friend constexpr bool operator==(const uint256_t &a, const uint256_t &b) {
         for (int i = 0; i < WIDTH; ++i) {
            if (a.value[i] != b.value[i])
               return false;
         }
         return true;
      }
      friend constexpr bool operator==(const uint256_t &a, uint64_t v) {
         uint256_t b = v;
         return a == b;
      }
      friend constexpr bool operator!=(const uint256_t &a, const uint256_t &b) {
         return !(a == b);
      }
      friend constexpr bool operator!=(const uint256_t &a, uint64_t v) {
         uint256_t b = v;
         return a != b;
      }
      friend constexpr bool operator>=(const uint256_t &a, const uint256_t &b) {
         for (int i = 0; i < WIDTH; ++i) {
            if (a.value[i] < b.value[i])
               return false;
         }
         return true;
      }
      friend constexpr bool operator<=(const uint256_t &a, const uint256_t &b) {
         for (int i = 0; i < WIDTH; ++i) {
            if (a.value[i] > b.value[i])
               return false;
         }
         return true;
      }

      union {
         uint64_t value[4] = { 0, 0, 0, 0 };
         uint128_t uivalue[2];
         capi_name cvalue;
      };

//      EOSLIB_SERIALIZE( uint256_t, (pzValue) )
      template<typename DataStream>
      friend DataStream& operator << ( DataStream& ds, const uint256_t& t ){
         ds << t.value[0]
            << t.value[1]
            << t.value[2]
            << t.value[3];
         return ds;
      }
      template<typename DataStream>
      friend DataStream& operator >> ( DataStream& ds, uint256_t& t ){
         ds >> t.value[0]
            >> t.value[1]
            >> t.value[2]
            >> t.value[3];
         return ds;
      }

      friend constexpr const uint256_t operator>>(const uint256_t& a, int32_t shift) { return uint256_t(a) >>= shift; }
      friend constexpr const uint256_t operator>>(const uint256_t& a, uint32_t shift) { return uint256_t(a) >>= shift; }
      friend constexpr const uint256_t operator<<(const uint256_t& a, int32_t shift) { return uint256_t(a) <<= shift; }
      friend constexpr const uint256_t operator<<(const uint256_t& a, uint32_t shift) { return uint256_t(a) <<= shift; }
   };
}

#endif //EOSIO_CDT_UINT256_T_HPP
