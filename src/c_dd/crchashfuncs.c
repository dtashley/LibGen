//$Header: svn://localhost/dtapublic/projs/emts/trunk/src/lib_c/c_datd/crchashfuncs.c 71 2016-11-05 11:07:06Z dashley $
//-------------------------------------------------------------------------------------------------
//This file is part of "David T. Ashley's Shared Source Code", a set of shared components
//integrated into many of David T. Ashley's projects.
//-------------------------------------------------------------------------------------------------
//This source code and any program in which it is compiled/used is provided under the MIT License,
//reproduced below.
//-------------------------------------------------------------------------------------------------
//Permission is hereby granted, free of charge, to any person obtaining a copy of
//this software and associated documentation files(the "Software"), to deal in the
//Software without restriction, including without limitation the rights to use,
//copy, modify, merge, publish, distribute, sublicense, and / or sell copies of the
//Software, and to permit persons to whom the Software is furnished to do so,
//subject to the following conditions :
//
//The above copyright notice and this permission notice shall be included in all
//copies or substantial portions of the Software.
//
//THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
//IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
//FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.IN NO EVENT SHALL THE
//AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
//LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
//OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
//SOFTWARE.
//-------------------------------------------------------------------------------------------------
#define MODULE_CRCHASHFUNCS

#include <assert.h>
#include <stdio.h>
#include <string.h>

#include "crchashfuncs.h"


//This is the data table used to generate CRC-32 values.  The code to
//generate these values isn't reproduced here (it was discarded after
//the values were generated), but this code originally came from a
//web page by Richard A. Ellingson.  I do not at this time understand
//the math behind the generation or usage of this table.
static const unsigned CRCHASHFUNCS_crc32tab[256] =
   {
   /* [000] */  0x00000000,
   /* [001] */  0x77073096,
   /* [002] */  0xEE0E612C,
   /* [003] */  0x990951BA,
   /* [004] */  0x076DC419,
   /* [005] */  0x706AF48F,
   /* [006] */  0xE963A535,
   /* [007] */  0x9E6495A3,
   /* [008] */  0x0EDB8832,
   /* [009] */  0x79DCB8A4,
   /* [010] */  0xE0D5E91E,
   /* [011] */  0x97D2D988,
   /* [012] */  0x09B64C2B,
   /* [013] */  0x7EB17CBD,
   /* [014] */  0xE7B82D07,
   /* [015] */  0x90BF1D91,
   /* [016] */  0x1DB71064,
   /* [017] */  0x6AB020F2,
   /* [018] */  0xF3B97148,
   /* [019] */  0x84BE41DE,
   /* [020] */  0x1ADAD47D,
   /* [021] */  0x6DDDE4EB,
   /* [022] */  0xF4D4B551,
   /* [023] */  0x83D385C7,
   /* [024] */  0x136C9856,
   /* [025] */  0x646BA8C0,
   /* [026] */  0xFD62F97A,
   /* [027] */  0x8A65C9EC,
   /* [028] */  0x14015C4F,
   /* [029] */  0x63066CD9,
   /* [030] */  0xFA0F3D63,
   /* [031] */  0x8D080DF5,
   /* [032] */  0x3B6E20C8,
   /* [033] */  0x4C69105E,
   /* [034] */  0xD56041E4,
   /* [035] */  0xA2677172,
   /* [036] */  0x3C03E4D1,
   /* [037] */  0x4B04D447,
   /* [038] */  0xD20D85FD,
   /* [039] */  0xA50AB56B,
   /* [040] */  0x35B5A8FA,
   /* [041] */  0x42B2986C,
   /* [042] */  0xDBBBC9D6,
   /* [043] */  0xACBCF940,
   /* [044] */  0x32D86CE3,
   /* [045] */  0x45DF5C75,
   /* [046] */  0xDCD60DCF,
   /* [047] */  0xABD13D59,
   /* [048] */  0x26D930AC,
   /* [049] */  0x51DE003A,
   /* [050] */  0xC8D75180,
   /* [051] */  0xBFD06116,
   /* [052] */  0x21B4F4B5,
   /* [053] */  0x56B3C423,
   /* [054] */  0xCFBA9599,
   /* [055] */  0xB8BDA50F,
   /* [056] */  0x2802B89E,
   /* [057] */  0x5F058808,
   /* [058] */  0xC60CD9B2,
   /* [059] */  0xB10BE924,
   /* [060] */  0x2F6F7C87,
   /* [061] */  0x58684C11,
   /* [062] */  0xC1611DAB,
   /* [063] */  0xB6662D3D,
   /* [064] */  0x76DC4190,
   /* [065] */  0x01DB7106,
   /* [066] */  0x98D220BC,
   /* [067] */  0xEFD5102A,
   /* [068] */  0x71B18589,
   /* [069] */  0x06B6B51F,
   /* [070] */  0x9FBFE4A5,
   /* [071] */  0xE8B8D433,
   /* [072] */  0x7807C9A2,
   /* [073] */  0x0F00F934,
   /* [074] */  0x9609A88E,
   /* [075] */  0xE10E9818,
   /* [076] */  0x7F6A0DBB,
   /* [077] */  0x086D3D2D,
   /* [078] */  0x91646C97,
   /* [079] */  0xE6635C01,
   /* [080] */  0x6B6B51F4,
   /* [081] */  0x1C6C6162,
   /* [082] */  0x856530D8,
   /* [083] */  0xF262004E,
   /* [084] */  0x6C0695ED,
   /* [085] */  0x1B01A57B,
   /* [086] */  0x8208F4C1,
   /* [087] */  0xF50FC457,
   /* [088] */  0x65B0D9C6,
   /* [089] */  0x12B7E950,
   /* [090] */  0x8BBEB8EA,
   /* [091] */  0xFCB9887C,
   /* [092] */  0x62DD1DDF,
   /* [093] */  0x15DA2D49,
   /* [094] */  0x8CD37CF3,
   /* [095] */  0xFBD44C65,
   /* [096] */  0x4DB26158,
   /* [097] */  0x3AB551CE,
   /* [098] */  0xA3BC0074,
   /* [099] */  0xD4BB30E2,
   /* [100] */  0x4ADFA541,
   /* [101] */  0x3DD895D7,
   /* [102] */  0xA4D1C46D,
   /* [103] */  0xD3D6F4FB,
   /* [104] */  0x4369E96A,
   /* [105] */  0x346ED9FC,
   /* [106] */  0xAD678846,
   /* [107] */  0xDA60B8D0,
   /* [108] */  0x44042D73,
   /* [109] */  0x33031DE5,
   /* [110] */  0xAA0A4C5F,
   /* [111] */  0xDD0D7CC9,
   /* [112] */  0x5005713C,
   /* [113] */  0x270241AA,
   /* [114] */  0xBE0B1010,
   /* [115] */  0xC90C2086,
   /* [116] */  0x5768B525,
   /* [117] */  0x206F85B3,
   /* [118] */  0xB966D409,
   /* [119] */  0xCE61E49F,
   /* [120] */  0x5EDEF90E,
   /* [121] */  0x29D9C998,
   /* [122] */  0xB0D09822,
   /* [123] */  0xC7D7A8B4,
   /* [124] */  0x59B33D17,
   /* [125] */  0x2EB40D81,
   /* [126] */  0xB7BD5C3B,
   /* [127] */  0xC0BA6CAD,
   /* [128] */  0xEDB88320,
   /* [129] */  0x9ABFB3B6,
   /* [130] */  0x03B6E20C,
   /* [131] */  0x74B1D29A,
   /* [132] */  0xEAD54739,
   /* [133] */  0x9DD277AF,
   /* [134] */  0x04DB2615,
   /* [135] */  0x73DC1683,
   /* [136] */  0xE3630B12,
   /* [137] */  0x94643B84,
   /* [138] */  0x0D6D6A3E,
   /* [139] */  0x7A6A5AA8,
   /* [140] */  0xE40ECF0B,
   /* [141] */  0x9309FF9D,
   /* [142] */  0x0A00AE27,
   /* [143] */  0x7D079EB1,
   /* [144] */  0xF00F9344,
   /* [145] */  0x8708A3D2,
   /* [146] */  0x1E01F268,
   /* [147] */  0x6906C2FE,
   /* [148] */  0xF762575D,
   /* [149] */  0x806567CB,
   /* [150] */  0x196C3671,
   /* [151] */  0x6E6B06E7,
   /* [152] */  0xFED41B76,
   /* [153] */  0x89D32BE0,
   /* [154] */  0x10DA7A5A,
   /* [155] */  0x67DD4ACC,
   /* [156] */  0xF9B9DF6F,
   /* [157] */  0x8EBEEFF9,
   /* [158] */  0x17B7BE43,
   /* [159] */  0x60B08ED5,
   /* [160] */  0xD6D6A3E8,
   /* [161] */  0xA1D1937E,
   /* [162] */  0x38D8C2C4,
   /* [163] */  0x4FDFF252,
   /* [164] */  0xD1BB67F1,
   /* [165] */  0xA6BC5767,
   /* [166] */  0x3FB506DD,
   /* [167] */  0x48B2364B,
   /* [168] */  0xD80D2BDA,
   /* [169] */  0xAF0A1B4C,
   /* [170] */  0x36034AF6,
   /* [171] */  0x41047A60,
   /* [172] */  0xDF60EFC3,
   /* [173] */  0xA867DF55,
   /* [174] */  0x316E8EEF,
   /* [175] */  0x4669BE79,
   /* [176] */  0xCB61B38C,
   /* [177] */  0xBC66831A,
   /* [178] */  0x256FD2A0,
   /* [179] */  0x5268E236,
   /* [180] */  0xCC0C7795,
   /* [181] */  0xBB0B4703,
   /* [182] */  0x220216B9,
   /* [183] */  0x5505262F,
   /* [184] */  0xC5BA3BBE,
   /* [185] */  0xB2BD0B28,
   /* [186] */  0x2BB45A92,
   /* [187] */  0x5CB36A04,
   /* [188] */  0xC2D7FFA7,
   /* [189] */  0xB5D0CF31,
   /* [190] */  0x2CD99E8B,
   /* [191] */  0x5BDEAE1D,
   /* [192] */  0x9B64C2B0,
   /* [193] */  0xEC63F226,
   /* [194] */  0x756AA39C,
   /* [195] */  0x026D930A,
   /* [196] */  0x9C0906A9,
   /* [197] */  0xEB0E363F,
   /* [198] */  0x72076785,
   /* [199] */  0x05005713,
   /* [200] */  0x95BF4A82,
   /* [201] */  0xE2B87A14,
   /* [202] */  0x7BB12BAE,
   /* [203] */  0x0CB61B38,
   /* [204] */  0x92D28E9B,
   /* [205] */  0xE5D5BE0D,
   /* [206] */  0x7CDCEFB7,
   /* [207] */  0x0BDBDF21,
   /* [208] */  0x86D3D2D4,
   /* [209] */  0xF1D4E242,
   /* [210] */  0x68DDB3F8,
   /* [211] */  0x1FDA836E,
   /* [212] */  0x81BE16CD,
   /* [213] */  0xF6B9265B,
   /* [214] */  0x6FB077E1,
   /* [215] */  0x18B74777,
   /* [216] */  0x88085AE6,
   /* [217] */  0xFF0F6A70,
   /* [218] */  0x66063BCA,
   /* [219] */  0x11010B5C,
   /* [220] */  0x8F659EFF,
   /* [221] */  0xF862AE69,
   /* [222] */  0x616BFFD3,
   /* [223] */  0x166CCF45,
   /* [224] */  0xA00AE278,
   /* [225] */  0xD70DD2EE,
   /* [226] */  0x4E048354,
   /* [227] */  0x3903B3C2,
   /* [228] */  0xA7672661,
   /* [229] */  0xD06016F7,
   /* [230] */  0x4969474D,
   /* [231] */  0x3E6E77DB,
   /* [232] */  0xAED16A4A,
   /* [233] */  0xD9D65ADC,
   /* [234] */  0x40DF0B66,
   /* [235] */  0x37D83BF0,
   /* [236] */  0xA9BCAE53,
   /* [237] */  0xDEBB9EC5,
   /* [238] */  0x47B2CF7F,
   /* [239] */  0x30B5FFE9,
   /* [240] */  0xBDBDF21C,
   /* [241] */  0xCABAC28A,
   /* [242] */  0x53B39330,
   /* [243] */  0x24B4A3A6,
   /* [244] */  0xBAD03605,
   /* [245] */  0xCDD70693,
   /* [246] */  0x54DE5729,
   /* [247] */  0x23D967BF,
   /* [248] */  0xB3667A2E,
   /* [249] */  0xC4614AB8,
   /* [250] */  0x5D681B02,
   /* [251] */  0x2A6F2B94,
   /* [252] */  0xB40BBE37,
   /* [253] */  0xC30C8EA1,
   /* [254] */  0x5A05DF1B,
   /* [255] */  0x2D02EF8D
   };

//It is known that the XOR of all of the table values above is 0,
//and that the sum is 0xFFFFFF80.  This is used to check the table
//for corruption on demand.
#define CRCHASHFUNCS_CRC32TABLE_XOR    (0x00000000)
#define CRCHASHFUNCS_CRC32TABLE_SUM    (0xFFFFFF80)


//Rotates an unsigned int left with wraparound.
static unsigned CRCHASHFUNCS_Crc32RotateLeftWithWrap(unsigned arg)
   {
   if (arg & 0x80000000)
      {
      arg <<= 1;
      arg |=  (unsigned)1;
      }
   else
      {
      arg <<= 1;
      arg &=  0xFFFFFFFE;
      }
    
   return(arg);
   }


//Appears to swap the bits in a data value, bit 0 for bit 7, bit 1
//for bit 6, etc. Obtained from the Internet (Richard A. Ellingson).
static unsigned CRCHASHFUNCS_Crc32Reflect(unsigned  ref,
                                          unsigned char ch)
   {
   unsigned i;
   unsigned value;

   value = 0;

   for (i=1; i<((unsigned)(ch+1)); i++)
      {
      if (ref & 1)
         {
         value |= (1 << (ch-i));
         }
      ref >>= 1;
      }

   return(value);
   }


//Returns !=0 if the internal table is alright.
int CRCHASHFUNCS_Crc32TableCheck(void)
   {
   int i;
   unsigned xor_val;
   unsigned sum_val;

   xor_val = sum_val = 0;

   for (i=0; i<256; i++)
      {
      xor_val ^= CRCHASHFUNCS_crc32tab[i];
      sum_val += CRCHASHFUNCS_crc32tab[i];
      }

   return((xor_val == CRCHASHFUNCS_CRC32TABLE_XOR) && 
          (sum_val == CRCHASHFUNCS_CRC32TABLE_SUM));
   }


//Initializes a CRC state struct.  This will be added to, the CRC will be 
//extracted, and the data structure will finally be closed.
void CRCHASHFUNCS_Crc32StateStructOpen(struct CRCHASHFUNCS_Crc32StateStruct *arg)
   {
   arg->crc = 0xFFFFFFFF;
   }


//Adds data to the data structure.
void CRCHASHFUNCS_Crc32StateStructAddData(struct CRCHASHFUNCS_Crc32StateStruct *arg,
                                          void *data,
                                          unsigned len)
   {     
   assert(arg != NULL);
   assert(data != NULL);

   while (len--)
      {
      arg->crc = (arg->crc >> 8) ^ CRCHASHFUNCS_crc32tab[((arg->crc & 0xFF) ^ *((unsigned char *)data)) & 0xFF];
      data = (unsigned char *)data + 1;
      }
   }


unsigned CRCHASHFUNCS_Crc32Extract(struct CRCHASHFUNCS_Crc32StateStruct *arg)
   {
   assert(arg != NULL);

   return(arg->crc ^ 0xFFFFFFFF);
   }


void CRCHASHFUNCS_Crc32StateStructClose(struct CRCHASHFUNCS_Crc32StateStruct *arg)
   {
   assert(arg != NULL);

   //Do nothing at this time.
   }


//Returns version control string for file.
//
const char *CRCHASHFUNCS_cvcinfo(void)
   {   
   return ("$Header: svn://localhost/dtapublic/projs/emts/trunk/src/lib_c/c_datd/crchashfuncs.c 71 2016-11-05 11:07:06Z dashley $");
   }


//Returns version control string for associated .H file.
//
const char *CRCHASHFUNCS_hvcinfo(void)
   {   
   return (CRCHASHFUNCS_H_VERSION);
   }

//End of crchashfuncs.c.
