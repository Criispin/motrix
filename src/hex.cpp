// Copyright (c) 2017-2019, The Monero Project
//
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without modification, are
// permitted provided that the following conditions are met:
//
// 1. Redistributions of source code must retain the above copyright notice, this list of
//    conditions and the following disclaimer.
//
// 2. Redistributions in binary form must reproduce the above copyright notice, this list
//    of conditions and the following disclaimer in the documentation and/or other
//    materials provided with the distribution.
//
// 3. Neither the name of the copyright holder nor the names of its contributors may be
//    used to endorse or promote products derived from this software without specific
//    prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY
// EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
// MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL
// THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
// SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
// PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
// INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
// STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF
// THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

#include "hex.hpp"

#include <limits>
#include "ascii_table.hpp"

  namespace
  {
    template<typename T>
    void write_hex(T&& out, const span<const std::uint8_t> src)
    {
      static constexpr const char hex[] = u8"0123456789abcdef";
      static_assert(sizeof(hex) == 17, "bad string size");
      for (const std::uint8_t byte : src)
      {
        *out = hex[byte >> 4];
        ++out;
        *out = hex[byte & 0x0F];
        ++out;
      }
    }
  }

  void to_hex::buffer_unchecked(char* out, const span<const std::uint8_t> src) noexcept
  {
    return write_hex(out, src);
  }

  bool from_hex::to_buffer(span<std::uint8_t> out, const span<const char> src) noexcept
  {
    if (src.size() / 2 != out.size())
      return false;
    return to_buffer_unchecked(out.data(), src);
  }

  bool from_hex::to_buffer_unchecked(std::uint8_t* dst, const span<const char> s) noexcept
  {
      if (s.size() % 2 != 0)
        return false;

      const unsigned char *src = (const unsigned char *)s.data();
      for(size_t i = 0; i < s.size(); i += 2)
      {
        int tmp = *src++;
        tmp = ascii::isx[tmp];
        if (tmp == 0xff) return false;
        int t2 = *src++;
        t2 = ascii::isx[t2];
        if (t2 == 0xff) return false;
        *dst++ = (tmp << 4) | t2;
      }

      return true;
  }
