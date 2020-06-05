#ifndef BASE64_HPP
#define BASE64_HPP

// This code has been modified from the original version.
//
// The original copyright notice is included below:
//   Copyright (C) 2004-2008 René Nyffenegger
//
//   This source code is provided 'as-is', without any express or implied
//   warranty. In no event will the author be held liable for any damages
//   arising from the use of this software.
//
//   Permission is granted to anyone to use this software for any purpose,
//   including commercial applications, and to alter it and redistribute it
//   freely, subject to the following restrictions:
//
//   1. The origin of this source code must not be misrepresented; you must
//      not claim that you wrote the original source code. If you use this
//      source code in a product, an acknowledgment in the product
//      documentation would be appreciated but is not required.
//
//   2. Altered source versions must be plainly marked as such, and must
//      not be misrepresented as being the original source code.
//
//   3. This notice may not be removed or altered from any source
//      distribution.
//
//   René Nyffenegger rene.nyffenegger@adp-gmbh.ch

#include <string>

static const std::string base64_chars =
             "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
             "abcdefghijklmnopqrstuvwxyz"
             "0123456789+/";

static inline bool is_base64(unsigned char c)
{
  return (std::isalnum(c) || (c == '+') || (c == '/'));
}

std::string base64_decode(std::string const& encoded_string)
{
  int size = encoded_string.size();
  int i = 0;
  int j = 0;
  int k = 0;
  unsigned char char_array_4[4], char_array_3[3];
  std::string result;

  while (size-- && (encoded_string[k] != '=')
      && is_base64(encoded_string[k]))
  {
    char_array_4[i++] = encoded_string[k++];

    if (i == 4)
    {
      for (i = 0; i < 4; i++)
        char_array_4[i] = base64_chars.find(char_array_4[i]);

      char_array_3[0] = (char_array_4[0] << 2)
        + ((char_array_4[1] & 0x30) >> 4);
      char_array_3[1] = ((char_array_4[1] & 0xf) << 4)
        + ((char_array_4[2] & 0x3c) >> 2);
      char_array_3[2] = ((char_array_4[2] & 0x3) << 6) + char_array_4[3];

      for (i = 0; i < 3; i++)
        result += char_array_3[i];

      i = 0;
    }
  }

  if (i)
  {
    for (j = i; j < 4; ++j)
      char_array_4[j] = 0;

    for (j = 0; j < 4; ++j)
      char_array_4[j] = base64_chars.find(char_array_4[j]);

    char_array_3[0] = (char_array_4[0] << 2)
      + ((char_array_4[1] & 0x30) >> 4);
    char_array_3[1] = ((char_array_4[1] & 0xf) << 4)
      + ((char_array_4[2] & 0x3c) >> 2);
    char_array_3[2] = ((char_array_4[2] & 0x3) << 6) + char_array_4[3];

    for (j = 0; j < i - 1; ++j)
      result += char_array_3[j];
  }

  return result;
}

#endif // BASE64_HPP
