// bdlde_crc32.cpp                                                    -*-C++-*-

// ----------------------------------------------------------------------------
//                                   NOTICE
//
// This component is not up to date with current BDE coding standards, and
// should not be used as an example for new development.
// ----------------------------------------------------------------------------

#include <bdlde_crc32.h>

#include <bsls_ident.h>
BSLS_IDENT_RCSID(bdlde_crc32_cpp,"$Id$ $CSID$")

#include <bslmf_assert.h>

///IMPLEMENTATION NOTES
///--------------------
// This implementation is based upon the CRC-32 implementation found at the
// end of RFC 1952.  The 'CRC_TABLE' below is the result of the initialization
// step found in the RFC 1952 implementation.  The values in the table are
// tested in the test driver (test case [14]).
//
// The algorithm from RFC 1952 is included below as a reference:
//..
//  // Table of CRCs of all 8-bit messages.
//  unsigned long crc_table[256];
//
//  // Flag: has the table been computed?  Initially false.
//  int crc_table_computed = 0;
//
//  // Make the table for a fast CRC.
//  void make_crc_table(void)
//  {
//      unsigned long c;
//      int n, k;
//
//      for (n = 0; n < 256; n++) {
//          c = (unsigned long) n;
//          for (k = 0; k < 8; k++) {
//              if (c & 1) {
//                  c = 0xedb88320L ^ (c >> 1);
//              } else {
//                  c = c >> 1;
//              }
//          }
//          crc_table[n] = c;
//      }
//      crc_table_computed = 1;
//  }
//
//  // Update a running crc with the bytes buf[0..len-1] and return
//  // the updated crc.  The crc should be initialized to zero.  Pre- and
//  // post-conditioning (one's complement) is performed within this
//  // function so it should not be done by the caller.  Usage example:
//  //     unsigned long crc = 0L;
//  //
//  //     while (read_buffer(buffer, length) != EOF) {
//  //         crc = update_crc(crc, buffer, length);
//  //     }
//  //     if (crc != original_crc) error();
//
//  unsigned long update_crc(unsigned long  crc,
//                           unsigned char *buf,
//                           int            len)
//  {
//      unsigned long c = crc ^ 0xffffffffL;
//      int n;
//
//      if (!crc_table_computed)
//          make_crc_table();
//      for (n = 0; n < len; n++) {
//          c = crc_table[(c ^ buf[n]) & 0xff] ^ (c >> 8);
//      }
//      return c ^ 0xffffffffL;
//  }
//
//  // Return the CRC of the bytes buf[0..len-1].
//  unsigned long crc(unsigned char *buf, int len)
//  {
//      return update_crc(0L, buf, len);
//  }
//..
//
// The main advantage of this algorithm is that the table-based lookup approach
// greatly improves the performance of the CRC calculation (see Sarwate, D.V.,
// "Computation of Cyclic Redundancy Checks via Table Look-Up", Communications
// of the ACM, 31(8), pp.  1008-1013).  Additionally, the Duff's Device-based
// implementation used in this component linearly improves the performance of
// the 'update_crc' method above; see:
//..
//  http://ravenphpscripts.com/modules.php?name=Forums&file=viewtopic&t=614
//..

#include <bsls_assert.h>
#include <bsl_ostream.h>

namespace BloombergLP {

BSLMF_ASSERT(4 == sizeof(unsigned int));

// STATIC DATA

static const unsigned int CRC_TABLE[256] = {
    0x0,        0x77073096, 0xee0e612c, 0x990951ba, 0x76dc419,
    0x706af48f, 0xe963a535, 0x9e6495a3, 0xedb8832,  0x79dcb8a4,
    0xe0d5e91e, 0x97d2d988, 0x9b64c2b,  0x7eb17cbd, 0xe7b82d07,
    0x90bf1d91, 0x1db71064, 0x6ab020f2, 0xf3b97148, 0x84be41de,
    0x1adad47d, 0x6ddde4eb, 0xf4d4b551, 0x83d385c7, 0x136c9856,
    0x646ba8c0, 0xfd62f97a, 0x8a65c9ec, 0x14015c4f, 0x63066cd9,
    0xfa0f3d63, 0x8d080df5, 0x3b6e20c8, 0x4c69105e, 0xd56041e4,
    0xa2677172, 0x3c03e4d1, 0x4b04d447, 0xd20d85fd, 0xa50ab56b,
    0x35b5a8fa, 0x42b2986c, 0xdbbbc9d6, 0xacbcf940, 0x32d86ce3,
    0x45df5c75, 0xdcd60dcf, 0xabd13d59, 0x26d930ac, 0x51de003a,
    0xc8d75180, 0xbfd06116, 0x21b4f4b5, 0x56b3c423, 0xcfba9599,
    0xb8bda50f, 0x2802b89e, 0x5f058808, 0xc60cd9b2, 0xb10be924,
    0x2f6f7c87, 0x58684c11, 0xc1611dab, 0xb6662d3d, 0x76dc4190,
    0x1db7106,  0x98d220bc, 0xefd5102a, 0x71b18589, 0x6b6b51f,
    0x9fbfe4a5, 0xe8b8d433, 0x7807c9a2, 0xf00f934,  0x9609a88e,
    0xe10e9818, 0x7f6a0dbb, 0x86d3d2d,  0x91646c97, 0xe6635c01,
    0x6b6b51f4, 0x1c6c6162, 0x856530d8, 0xf262004e, 0x6c0695ed,
    0x1b01a57b, 0x8208f4c1, 0xf50fc457, 0x65b0d9c6, 0x12b7e950,
    0x8bbeb8ea, 0xfcb9887c, 0x62dd1ddf, 0x15da2d49, 0x8cd37cf3,
    0xfbd44c65, 0x4db26158, 0x3ab551ce, 0xa3bc0074, 0xd4bb30e2,
    0x4adfa541, 0x3dd895d7, 0xa4d1c46d, 0xd3d6f4fb, 0x4369e96a,
    0x346ed9fc, 0xad678846, 0xda60b8d0, 0x44042d73, 0x33031de5,
    0xaa0a4c5f, 0xdd0d7cc9, 0x5005713c, 0x270241aa, 0xbe0b1010,
    0xc90c2086, 0x5768b525, 0x206f85b3, 0xb966d409, 0xce61e49f,
    0x5edef90e, 0x29d9c998, 0xb0d09822, 0xc7d7a8b4, 0x59b33d17,
    0x2eb40d81, 0xb7bd5c3b, 0xc0ba6cad, 0xedb88320, 0x9abfb3b6,
    0x3b6e20c,  0x74b1d29a, 0xead54739, 0x9dd277af, 0x4db2615,
    0x73dc1683, 0xe3630b12, 0x94643b84, 0xd6d6a3e,  0x7a6a5aa8,
    0xe40ecf0b, 0x9309ff9d, 0xa00ae27,  0x7d079eb1, 0xf00f9344,
    0x8708a3d2, 0x1e01f268, 0x6906c2fe, 0xf762575d, 0x806567cb,
    0x196c3671, 0x6e6b06e7, 0xfed41b76, 0x89d32be0, 0x10da7a5a,
    0x67dd4acc, 0xf9b9df6f, 0x8ebeeff9, 0x17b7be43, 0x60b08ed5,
    0xd6d6a3e8, 0xa1d1937e, 0x38d8c2c4, 0x4fdff252, 0xd1bb67f1,
    0xa6bc5767, 0x3fb506dd, 0x48b2364b, 0xd80d2bda, 0xaf0a1b4c,
    0x36034af6, 0x41047a60, 0xdf60efc3, 0xa867df55, 0x316e8eef,
    0x4669be79, 0xcb61b38c, 0xbc66831a, 0x256fd2a0, 0x5268e236,
    0xcc0c7795, 0xbb0b4703, 0x220216b9, 0x5505262f, 0xc5ba3bbe,
    0xb2bd0b28, 0x2bb45a92, 0x5cb36a04, 0xc2d7ffa7, 0xb5d0cf31,
    0x2cd99e8b, 0x5bdeae1d, 0x9b64c2b0, 0xec63f226, 0x756aa39c,
    0x26d930a,  0x9c0906a9, 0xeb0e363f, 0x72076785, 0x5005713,
    0x95bf4a82, 0xe2b87a14, 0x7bb12bae, 0xcb61b38,  0x92d28e9b,
    0xe5d5be0d, 0x7cdcefb7, 0xbdbdf21,  0x86d3d2d4, 0xf1d4e242,
    0x68ddb3f8, 0x1fda836e, 0x81be16cd, 0xf6b9265b, 0x6fb077e1,
    0x18b74777, 0x88085ae6, 0xff0f6a70, 0x66063bca, 0x11010b5c,
    0x8f659eff, 0xf862ae69, 0x616bffd3, 0x166ccf45, 0xa00ae278,
    0xd70dd2ee, 0x4e048354, 0x3903b3c2, 0xa7672661, 0xd06016f7,
    0x4969474d, 0x3e6e77db, 0xaed16a4a, 0xd9d65adc, 0x40df0b66,
    0x37d83bf0, 0xa9bcae53, 0xdebb9ec5, 0x47b2cf7f, 0x30b5ffe9,
    0xbdbdf21c, 0xcabac28a, 0x53b39330, 0x24b4a3a6, 0xbad03605,
    0xcdd70693, 0x54de5729, 0x23d967bf, 0xb3667a2e, 0xc4614ab8,
    0x5d681b02, 0x2a6f2b94, 0xb40bbe37, 0xc30c8ea1, 0x5a05df1b,
    0x2d02ef8d
};

namespace bdlde {
                                // -----------
                                // class Crc32
                                // -----------

// MANIPULATORS
void Crc32::update(const void *data, bsl::size_t length)
{
    BSLS_ASSERT(data || !length);

    // The following is a Duff's Device-based implementation of a common
    // algorithm (see end of RFC 1952).

    register const unsigned char *d = (const unsigned char *)data;
    register unsigned int tmp = d_crc;

    switch (length % 4) {
      case 3: tmp = CRC_TABLE[(tmp ^ *d++) & 0xff] ^ (tmp >> 8);
      case 2: tmp = CRC_TABLE[(tmp ^ *d++) & 0xff] ^ (tmp >> 8);
      case 1: tmp = CRC_TABLE[(tmp ^ *d++) & 0xff] ^ (tmp >> 8);
      default: ;
    }

    bsl::size_t n = length / 4;
    while (n) {
        tmp = CRC_TABLE[(tmp ^ *d++) & 0xff] ^ (tmp >> 8);
        tmp = CRC_TABLE[(tmp ^ *d++) & 0xff] ^ (tmp >> 8);
        tmp = CRC_TABLE[(tmp ^ *d++) & 0xff] ^ (tmp >> 8);
        tmp = CRC_TABLE[(tmp ^ *d++) & 0xff] ^ (tmp >> 8);
        --n;
    }

    d_crc = tmp;
}

// ACCESSORS
bsl::ostream& Crc32::print(bsl::ostream& stream) const
{
    const char *hex = "0123456789abcdef";
    unsigned int crc = checksum();

    // Fill an array with the output and then stream it out.

    char array[2 + 8 + 1];  // 2 for "0x"; 8 for the CRC; 1 for '\0'

    array[0] = '0';
    array[1] = 'x';

    array[2] = hex[ crc >> 28       ];
    array[3] = hex[(crc >> 24) & 0xf];
    array[4] = hex[(crc >> 20) & 0xf];
    array[5] = hex[(crc >> 16) & 0xf];
    array[6] = hex[(crc >> 12) & 0xf];
    array[7] = hex[(crc >>  8) & 0xf];
    array[8] = hex[(crc >>  4) & 0xf];
    array[9] = hex[ crc        & 0xf];

    array[10] = '\0';

    return stream << array;
}

}  // close package namespace
}  // close enterprise namespace

// ----------------------------------------------------------------------------
// Copyright 2015 Bloomberg Finance L.P.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
// ----------------------------- END-OF-FILE ----------------------------------
