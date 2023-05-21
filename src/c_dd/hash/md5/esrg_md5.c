//$Header: svn://localhost/dtapublic/projs/emts/trunk/src/lib_c/c_datd/hash/md5/esrg_md5.c 242 2018-08-04 18:25:51Z dashley $
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
/* A description of the functionality of this module and the public interface
** definition is contained in the associated .H file.
*/

#define MODULE_ESRG_MD5

#include <assert.h>
#include <stddef.h>
#include <string.h>

#include "charfunc.h"
#include "esrg_md5.h"

//These are macros which are defined for efficiency.  These
//functions come from RFC 1321.
#define ESRG_MD5_FUNC_F(x,y,z) (((x) & (y)) | ((~x) &  (z)))
#define ESRG_MD5_FUNC_G(x,y,z) (((x) & (z)) | ( (y) & (~z)))
#define ESRG_MD5_FUNC_H(x,y,z) ((x) ^ (y) ^ (z))
#define ESRG_MD5_FUNC_I(x,y,z) ((y) ^ ((x) | (~z)))

//This is a left rotation macro, again for efficiency.  This
//macro rotates a 32-bit quantity x left (cyclically) by 
//n bits.
#define ESRG_MD5_FUNC_ROT_LEFT(x, n) (((x) << (n)) | ((x) >> (32-(n))))

//These macros do one operation as described in the RFC.  These allow
//the inlining of code for far more speed.
#define ESRG_MD5_FUNC_FF(a,b,c,d,x,s,ac) {                             \
           (a) += ESRG_MD5_FUNC_F((b),(c),(d)) + (x) + (unsigned)(ac); \
           (a)  = ESRG_MD5_FUNC_ROT_LEFT((a),(s));                     \
           (a) += (b);                                                 \
           }
#define ESRG_MD5_FUNC_GG(a,b,c,d,x,s,ac) {                             \
           (a) += ESRG_MD5_FUNC_G((b),(c),(d)) + (x) + (unsigned)(ac); \
           (a)  = ESRG_MD5_FUNC_ROT_LEFT((a),(s));                     \
           (a) += (b);                                                 \
           }
#define ESRG_MD5_FUNC_HH(a,b,c,d,x,s,ac) {                             \
           (a) += ESRG_MD5_FUNC_H((b),(c),(d)) + (x) + (unsigned)(ac); \
           (a)  = ESRG_MD5_FUNC_ROT_LEFT((a),(s));                     \
           (a) += (b);                                                 \
           }
#define ESRG_MD5_FUNC_II(a,b,c,d,x,s,ac) {                             \
           (a) += ESRG_MD5_FUNC_I((b),(c),(d)) + (x) + (unsigned)(ac); \
           (a)  = ESRG_MD5_FUNC_ROT_LEFT((a),(s));                     \
           (a) += (b);                                                 \
           }


//This is the padding table to append.  It is done with
//an array for quickness.
static unsigned char ESRG_MD5_pad_table[] =
   { 
   0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
   };


void ESRG_MD5_Md5StateStructOpen(struct ESRG_MD5_Md5StateStruct *arg)
   {
   assert(arg != NULL);

   memset(arg, 0, sizeof(struct ESRG_MD5_Md5StateStruct));
      //Everything to zero, processed bitcount automatically set to zero.

   arg->A = 0x67452301;  //These assignments directly from RFC.
   arg->B = 0xEFCDAB89;
   arg->C = 0x98BADCFE;
   arg->D = 0x10325476;
   }


//Copies the byte buffer to the word buffer within the state block.
//This is done in a way which hides big-endian/little-endian concerns.

static void ESRG_MD5_CopyBytesToWords(struct ESRG_MD5_Md5StateStruct *arg)
   {
   int i;

   assert(arg != NULL);

   //Copy the buffer contents into the words.  We need to be careful
   //to do this right, because of big-endian/little-endian concerns.
   for (i=0; i<16; i++)
      {
      assert((i * 4 + 3) < 64);
      arg->X[i] = (((unsigned int)(arg->buf[i*4+3])) << 24)
                  +
                  (((unsigned int)(arg->buf[i*4+2])) << 16)
                  +
                  (((unsigned int)(arg->buf[i*4+1])) <<  8)
                  +
                  (((unsigned int)(arg->buf[i*4]))   );
      }
   }


//Does the MD-5 rounds as specified by RFC 1321.

static void ESRG_MD5_DoMd5Rounds(struct ESRG_MD5_Md5StateStruct *arg)
   {
   unsigned AA, BB, CC, DD;
         //Directly from RFC 1321.
   unsigned A, B, C, D;
         //We also want to buffer out the state variables, to eliminate
         //the risk of repeated pointer dereferences.
   unsigned X[16];
         //Buffer to avoid repeated dereferences.

   assert(arg != NULL);

   //Copy bytes into words.
   ESRG_MD5_CopyBytesToWords(arg);

   //Copy out the buffer for speed.
   X[ 0] = arg->X[ 0];
   X[ 1] = arg->X[ 1];
   X[ 2] = arg->X[ 2];
   X[ 3] = arg->X[ 3];
   X[ 4] = arg->X[ 4];
   X[ 5] = arg->X[ 5];
   X[ 6] = arg->X[ 6];
   X[ 7] = arg->X[ 7];
   X[ 8] = arg->X[ 8];
   X[ 9] = arg->X[ 9];
   X[10] = arg->X[10];
   X[11] = arg->X[11];
   X[12] = arg->X[12];
   X[13] = arg->X[13];
   X[14] = arg->X[14];
   X[15] = arg->X[15];

   //Buffer out the state for speed.
   A = arg->A;
   B = arg->B;
   C = arg->C;
   D = arg->D;

   //Make the assignments to temporary variables as described by the RFC.
   AA = A;
   BB = B;
   CC = C;
   DD = D;

   //We can now do the MD-5 rounds directly as described in the RFC.  The
   //most effective way to do this is with macros.  I tried using a tabulated
   //approach, but the speed hit was unbelievably bad.  This approach is 
   //about the best known.
   //
   //Round 1
   //
   ESRG_MD5_FUNC_FF(A,B,C,D,X[ 0], 7,0xd76aa478);  /*  1 */
   ESRG_MD5_FUNC_FF(D,A,B,C,X[ 1],12,0xe8c7b756);  /*  2 */
   ESRG_MD5_FUNC_FF(C,D,A,B,X[ 2],17,0x242070db);  /*  3 */
   ESRG_MD5_FUNC_FF(B,C,D,A,X[ 3],22,0xc1bdceee);  /*  4 */
   ESRG_MD5_FUNC_FF(A,B,C,D,X[ 4], 7,0xf57c0faf);  /*  5 */
   ESRG_MD5_FUNC_FF(D,A,B,C,X[ 5],12,0x4787c62a);  /*  6 */
   ESRG_MD5_FUNC_FF(C,D,A,B,X[ 6],17,0xa8304613);  /*  7 */
   ESRG_MD5_FUNC_FF(B,C,D,A,X[ 7],22,0xfd469501);  /*  8 */
   ESRG_MD5_FUNC_FF(A,B,C,D,X[ 8], 7,0x698098d8);  /*  9 */
   ESRG_MD5_FUNC_FF(D,A,B,C,X[ 9],12,0x8b44f7af);  /* 10 */
   ESRG_MD5_FUNC_FF(C,D,A,B,X[10],17,0xffff5bb1);  /* 11 */
   ESRG_MD5_FUNC_FF(B,C,D,A,X[11],22,0x895cd7be);  /* 12 */
   ESRG_MD5_FUNC_FF(A,B,C,D,X[12], 7,0x6b901122);  /* 13 */
   ESRG_MD5_FUNC_FF(D,A,B,C,X[13],12,0xfd987193);  /* 14 */
   ESRG_MD5_FUNC_FF(C,D,A,B,X[14],17,0xa679438e);  /* 15 */
   ESRG_MD5_FUNC_FF(B,C,D,A,X[15],22,0x49b40821);  /* 16 */
   //
   //Round 2
   //
   ESRG_MD5_FUNC_GG(A,B,C,D,X[ 1], 5,0xf61e2562);  /* 17 */
   ESRG_MD5_FUNC_GG(D,A,B,C,X[ 6], 9,0xc040b340);  /* 18 */
   ESRG_MD5_FUNC_GG(C,D,A,B,X[11],14,0x265e5a51);  /* 19 */
   ESRG_MD5_FUNC_GG(B,C,D,A,X[ 0],20,0xe9b6c7aa);  /* 20 */
   ESRG_MD5_FUNC_GG(A,B,C,D,X[ 5], 5,0xd62f105d);  /* 21 */
   ESRG_MD5_FUNC_GG(D,A,B,C,X[10], 9,0x02441453);  /* 22 */
   ESRG_MD5_FUNC_GG(C,D,A,B,X[15],14,0xd8a1e681);  /* 23 */
   ESRG_MD5_FUNC_GG(B,C,D,A,X[ 4],20,0xe7d3fbc8);  /* 24 */
   ESRG_MD5_FUNC_GG(A,B,C,D,X[ 9], 5,0x21e1cde6);  /* 25 */
   ESRG_MD5_FUNC_GG(D,A,B,C,X[14], 9,0xc33707d6);  /* 26 */
   ESRG_MD5_FUNC_GG(C,D,A,B,X[ 3],14,0xf4d50d87);  /* 27 */
   ESRG_MD5_FUNC_GG(B,C,D,A,X[ 8],20,0x455a14ed);  /* 28 */ 
   ESRG_MD5_FUNC_GG(A,B,C,D,X[13], 5,0xa9e3e905);  /* 29 */ 
   ESRG_MD5_FUNC_GG(D,A,B,C,X[ 2], 9,0xfcefa3f8);  /* 30 */ 
   ESRG_MD5_FUNC_GG(C,D,A,B,X[ 7],14,0x676f02d9);  /* 31 */ 
   ESRG_MD5_FUNC_GG(B,C,D,A,X[12],20,0x8d2a4c8a);  /* 32 */ 
   //
   //Round 3
   //
   ESRG_MD5_FUNC_HH(A,B,C,D,X[ 5], 4,0xfffa3942);  /* 33 */
   ESRG_MD5_FUNC_HH(D,A,B,C,X[ 8],11,0x8771f681);  /* 34 */
   ESRG_MD5_FUNC_HH(C,D,A,B,X[11],16,0x6d9d6122);  /* 35 */
   ESRG_MD5_FUNC_HH(B,C,D,A,X[14],23,0xfde5380c);  /* 36 */
   ESRG_MD5_FUNC_HH(A,B,C,D,X[ 1], 4,0xa4beea44);  /* 37 */
   ESRG_MD5_FUNC_HH(D,A,B,C,X[ 4],11,0x4bdecfa9);  /* 38 */
   ESRG_MD5_FUNC_HH(C,D,A,B,X[ 7],16,0xf6bb4b60);  /* 39 */
   ESRG_MD5_FUNC_HH(B,C,D,A,X[10],23,0xbebfbc70);  /* 40 */
   ESRG_MD5_FUNC_HH(A,B,C,D,X[13], 4,0x289b7ec6);  /* 41 */
   ESRG_MD5_FUNC_HH(D,A,B,C,X[ 0],11,0xeaa127fa);  /* 42 */
   ESRG_MD5_FUNC_HH(C,D,A,B,X[ 3],16,0xd4ef3085);  /* 43 */
   ESRG_MD5_FUNC_HH(B,C,D,A,X[ 6],23,0x04881d05);  /* 44 */
   ESRG_MD5_FUNC_HH(A,B,C,D,X[ 9], 4,0xd9d4d039);  /* 45 */
   ESRG_MD5_FUNC_HH(D,A,B,C,X[12],11,0xe6db99e5);  /* 46 */
   ESRG_MD5_FUNC_HH(C,D,A,B,X[15],16,0x1fa27cf8);  /* 47 */
   ESRG_MD5_FUNC_HH(B,C,D,A,X[ 2],23,0xc4ac5665);  /* 48 */
   //
   //Round 4
   //
   ESRG_MD5_FUNC_II(A,B,C,D,X[ 0], 6,0xf4292244);  /* 49 */
   ESRG_MD5_FUNC_II(D,A,B,C,X[ 7],10,0x432aff97);  /* 50 */
   ESRG_MD5_FUNC_II(C,D,A,B,X[14],15,0xab9423a7);  /* 51 */
   ESRG_MD5_FUNC_II(B,C,D,A,X[ 5],21,0xfc93a039);  /* 52 */
   ESRG_MD5_FUNC_II(A,B,C,D,X[12], 6,0x655b59c3);  /* 53 */
   ESRG_MD5_FUNC_II(D,A,B,C,X[ 3],10,0x8f0ccc92);  /* 54 */
   ESRG_MD5_FUNC_II(C,D,A,B,X[10],15,0xffeff47d);  /* 55 */
   ESRG_MD5_FUNC_II(B,C,D,A,X[ 1],21,0x85845dd1);  /* 56 */
   ESRG_MD5_FUNC_II(A,B,C,D,X[ 8], 6,0x6fa87e4f);  /* 57 */
   ESRG_MD5_FUNC_II(D,A,B,C,X[15],10,0xfe2ce6e0);  /* 58 */
   ESRG_MD5_FUNC_II(C,D,A,B,X[ 6],15,0xa3014314);  /* 59 */
   ESRG_MD5_FUNC_II(B,C,D,A,X[13],21,0x4e0811a1);  /* 60 */
   ESRG_MD5_FUNC_II(A,B,C,D,X[ 4], 6,0xf7537e82);  /* 61 */
   ESRG_MD5_FUNC_II(D,A,B,C,X[11],10,0xbd3af235);  /* 62 */
   ESRG_MD5_FUNC_II(C,D,A,B,X[ 2],15,0x2ad7d2bb);  /* 63 */
   ESRG_MD5_FUNC_II(B,C,D,A,X[ 9],21,0xeb86d391);  /* 64 */
   //End of Round 4.

   //Perform the four additions as mandated by the RFC.
   A += AA;
   B += BB;
   C += CC;
   D += DD;

   //Buffer the state vector back.
   arg->A = A;
   arg->B = B;
   arg->C = C;
   arg->D = D;
   }


void ESRG_MD5_Md5StateStructAddData(struct ESRG_MD5_Md5StateStruct *arg,
                                    void *pointer_in,
                                    unsigned  len)
   {
   unsigned int   low_32;
   unsigned int   byte_offset;
   unsigned char *data;

   assert(arg        != NULL);
   assert(pointer_in != NULL);

   data = (unsigned char *)pointer_in;
      //It is easier to do it this way, rather than cast all the time.

   low_32 = (unsigned int)arg->bit_count;
      //Copy off the least significant bits.  Easier to do once.

   byte_offset = low_32 >> 3;
      //This gives our byte offset, up to 500+Mb or so.

   while(len--)
      {
      //We process rounds AFTER a byte is added to the buffer.  So
      //it is always safe to add a byte first.
      arg->buf[byte_offset & 0x3F] = *data;

      //Nothing to do unless this was the final byte of the buffer.
      if ((byte_offset & 0x3F) == 63)
         {
         ESRG_MD5_DoMd5Rounds(arg);
         }

      //Increment.
      data++;
      byte_offset++;
      arg->bit_count += 8;
      }
   }


void ESRG_MD5_Md5StateStructClose(struct ESRG_MD5_Md5StateStruct  *state,
                                  struct ESRG_MD5_Md5ResultStruct *result)
   {
   unsigned int   low_32, high_32, high_32_copy, low_32_copy;
   unsigned int   byte_offset;
   unsigned int   buffer_offset;
   unsigned char  length_buf[8];
   //int            i;

   assert(state  != NULL);
   assert(result != NULL);

   //Obtain easier-to-use indices.  These provide a snapshot of the
   //length before padding is done.
   low_32        =  (unsigned int)state->bit_count;
   high_32       =  (unsigned int)(state->bit_count >> 32);
   byte_offset   =  low_32 >> 3;
   buffer_offset =  byte_offset & 0x3F;

   //We need to pad the buffer out to 8 bytes short of a multiple,
   //per RFC 1321.
   ESRG_MD5_Md5StateStructAddData(state, 
                                  ESRG_MD5_pad_table,
                                  (buffer_offset==56) ? (64) : ((56 - buffer_offset) & 0x3F));

   //At this point we are fully prepped to stuff in the length in bits.
   //Prepare the length in a buffer.
   high_32_copy = high_32;
   low_32_copy  = low_32;
   length_buf[0] = (unsigned char)(low_32_copy);
   length_buf[1] = (unsigned char)(low_32_copy >>   8);
   length_buf[2] = (unsigned char)(low_32_copy >>  16);
   length_buf[3] = (unsigned char)(low_32_copy >>  24);
   length_buf[4] = (unsigned char)(high_32_copy);
   length_buf[5] = (unsigned char)(high_32_copy >>  8);
   length_buf[6] = (unsigned char)(high_32_copy >> 16);
   length_buf[7] = (unsigned char)(high_32_copy >> 24);

   //Tack on the length.  This is guaranteed to generate end up with
   //the last thing being done the compute plus the index being zero.
   //
   ESRG_MD5_Md5StateStructAddData(state, 
                                  length_buf,
                                  8);

   //Be absolutely sure we are rolled over to zero.
   assert((((int)state->bit_count) & 0x1FF) == 0);

   //Zero out the return state, just to be sure.
   memset(result, 0, sizeof(struct ESRG_MD5_Md5ResultStruct));

   //Give caller the binary version.
   result->md5_words[0] = state->A;
   result->md5_words[1] = state->B;
   result->md5_words[2] = state->C;
   result->md5_words[3] = state->D;

   //Convert to string for caller.
   CHARFUNC_int_to_lc_hex_rev(state->A, result->md5_chars +  0);
   CHARFUNC_int_to_lc_hex_rev(state->B, result->md5_chars +  8);
   CHARFUNC_int_to_lc_hex_rev(state->C, result->md5_chars + 16);
   CHARFUNC_int_to_lc_hex_rev(state->D, result->md5_chars + 24);

   //Because of the way the CHARFUNC_int_to_lc_hex_rev() function
   //works, it produces the mirror image of the sequence of nibbles.
   //This is not quite what we want.  What we want (least significant
   //byte first, but within each byte most significant nibble first)
   //from each integer is this:
   //
   // n1 n0 n3 n2 n5 n4 n7 n6
   //
   //but what we get from that function is this:
   //
   // n0 n1 n2 n3 n4 n5 n6 n6,
   //
   //so we have to swap nibbles in each byte.
   //
   {
   int i;
   char temp;

   for (i=0; i<16; i++)
      {
      temp                     = result->md5_chars[i*2];
      result->md5_chars[i*2]   = result->md5_chars[i*2+1];
      result->md5_chars[i*2+1] = temp;
      }
   }

   result->md5_chars[32] = 0;  //Terminator.

   //Destroy the state, which may contain sensitive information.
   //This idea came from Rivest's sample code.
   memset(state, 0, sizeof(struct ESRG_MD5_Md5StateStruct));
   }


//Returns version control string for file.
//
const char *ESRG_MD5_cvcinfo(void)
   {   
   return ("$Header: svn://localhost/dtapublic/projs/emts/trunk/src/lib_c/c_datd/hash/md5/esrg_md5.c 242 2018-08-04 18:25:51Z dashley $");
   }


//Returns version control string for associated .H file.
//
const char *ESRG_MD5_hvcinfo(void)
   {   
   return (ESRG_MD5_H_VERSION);
   }

//End of esrg_md5.c.
