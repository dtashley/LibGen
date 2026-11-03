#include "gtest/gtest.h"

#include "lg_cpp_dd_sha1.hpp"


class LgSha1Test : public testing::Test {
protected:
};


//Ensures that object starts initialized properly.
TEST_F(LgSha1Test, IsEmptyInitially)
{
   LgSha1 ts;
   size_t i;

   ASSERT_EQ(ts.m_state[0], 0x67452301);
   ASSERT_EQ(ts.m_state[1], 0xefcdab89);
   ASSERT_EQ(ts.m_state[2], 0x98badcfe);
   ASSERT_EQ(ts.m_state[3], 0x10325476);
   ASSERT_EQ(ts.m_state[4], 0xc3d2e1f0);

   ASSERT_EQ(ts.m_bitCount, 0);

   for (i = 0; i < 64; i++)
   {
       ASSERT_EQ(ts.m_buffer[i], 0);
   }

   ASSERT_EQ(ts.m_bufferSize, 0);

   ASSERT_FALSE(ts.m_finalized);
}


//Double-checks the hash of "abc", upper case.  This is an example from FIPS 180-1, and the expected hash is known.
TEST_F(LgSha1Test, AbcHashUppercase)
{
    LgSha1 ts;
    const char * expected_hash = "A9993E364706816ABA3E25717850C26C9CD0D89D";
    char hash_output[41] = { 0 };
    const uint8_t abc[] = { 'a', 'b', 'c' };

    ts.Add(abc, sizeof(abc));

    ts.Finalize();

    ASSERT_TRUE(ts.m_finalized);

    ts.ExtractToCstring(hash_output, true);

    ASSERT_EQ(strcmp(expected_hash, hash_output), 0);
}


//Double-checks the hash of "abc", lower case.  This is an example from FIPS 180-1, and the expected hash is known.
TEST_F(LgSha1Test, AbcHashLowercase)
{
    LgSha1 ts;
    const char* expected_hash = "a9993e364706816aba3e25717850c26c9cd0d89d";
    char hash_output[41] = { 0 };
    const uint8_t abc[] = { 'a', 'b', 'c' };

    ts.Add(abc, sizeof(abc));

    ts.Finalize();

    ASSERT_TRUE(ts.m_finalized);

    ts.ExtractToCstring(hash_output, false);

    ASSERT_EQ(strcmp(expected_hash, hash_output), 0);
}


//Double-checks hash extraction to an array of bytes.
TEST_F(LgSha1Test, AbcHashUint8Extraction)
{
   LgSha1 ts;
   const uint8_t expected_hash[] = { 0xa9, 0x99, 0x3e, 0x36, 0x47, 0x06, 0x81, 0x6a, 0xba, 0x3e, 0x25, 0x71, 0x78, 0x50, 0xc2, 0x6c, 0x9c, 0xd0, 0xd8, 0x9d, 0xd5 };
      //Final value is to be sure there is not obvious array overrun.
   uint8_t hash_output[21] = { 0 };
   const uint8_t abc[] = { 'a', 'b', 'c' };

   hash_output[20] = expected_hash[20];

   ts.Add(abc, sizeof(abc));

   ts.Finalize();

   ASSERT_TRUE(ts.m_finalized);

   ts.ExtractToByteArray(hash_output);

   ASSERT_EQ(memcmp(expected_hash, hash_output, sizeof(hash_output)), 0);
}


//Double-checks hash extraction to an array of 32-bit integers.
TEST_F(LgSha1Test, AbcHashUint32Extraction)
{
    LgSha1 ts;
    const uint32_t expected_hash[] = { 0xa9993e36, 0x4706816a, 0xba3e2571, 0x7850c26c, 0x9cd0d89d, 0xa55ad55d };
    //Final value is to be sure there is not obvious array overrun.

    uint32_t hash_output[6] = { 0 };
    const uint8_t abc[] = { 'a', 'b', 'c' };

    hash_output[5] = expected_hash[5];

    ts.Add(abc, sizeof(abc));

    ts.Finalize();

    ASSERT_TRUE(ts.m_finalized);

    ts.ExtractToUint32Array(hash_output);

    ASSERT_EQ(memcmp(expected_hash, hash_output, sizeof(hash_output)), 0);
}


//Double-checks the hash of "", lower case.
TEST_F(LgSha1Test, EmptyStringHashLowercase)
{
    LgSha1 ts;
    const char* expected_hash = "da39a3ee5e6b4b0d3255bfef95601890afd80709";
    char hash_output[41] = { 0 };

    ts.Finalize();

    ASSERT_TRUE(ts.m_finalized);

    ts.ExtractToCstring(hash_output, false);

    ASSERT_EQ(strcmp(expected_hash, hash_output), 0);
}

//Double-checks the hash of a strings out to 130 bytes.
TEST_F(LgSha1Test, ZeroTo130ByteStringHashLowercase)
{
   const struct {
       const char* input;
       const char* expected_hash;
   } test_cases[] =
   {
      { 
         //   0
         "", 
         "da39a3ee5e6b4b0d3255bfef95601890afd80709"
      },
      { 
         //   1
         "a",
         "86f7e437faa5a7fce15d1ddcb9eaeaea377667b8"
      },
      {
         "ab",
         "da23614e02469a0d7c7bd1bdab5c9c474b1904dc"
      },
      {
         "abc",
         "a9993e364706816aba3e25717850c26c9cd0d89d"
      },
      {
         "abcd",
         "81fe8bfe87576c3ecb22426f8e57847382917acf"
      },
      {
         "abcde",
         "03de6c570bfe24bfc328ccd7ca46b76eadaf4334"
      },
      {
         "abcdef",
         "1f8ac10f23c5b5bc1167bda84b833e5c057a77d2"
      },
      {
         "abcdefg",
         "2fb5e13419fc89246865e7a324f476ec624e8740"
      },
      {
         "abcdefgh",
         "425af12a0743502b322e93a015bcf868e324d56a"
      },
      { 
         "abcdefghi",
         "c63b19f1e4c8b5f76b25c49b8b87f57d8e4872a1"
      },
      {
         "abcdefghij",
         "d68c19a0a345b7eab78d5e11e991c026ec60db63"
      },
      { 
         "abcdefghijk", 
         "5dfac39f71ad4d35a153ba4fc12d943a0e178e6a" 
      },
      { 
         "abcdefghijkl", 
         "eb4608cebfcfd4df81410cbd06507ea6af978d9c" 
      },
      { 
         "abcdefghijklm", 
         "4b9892b6527214afc655b8aa52f4d203c15e7c9c" 
      },
      { 
         "abcdefghijklmn", 
         "85d7c5ff403abe72df5b8a2708821ee33cd0bcce" 
      },
    };

   size_t num_test_cases = sizeof(test_cases) / sizeof(test_cases[0]);
   size_t i;
   char hash_output[41] = { 0 };

   for (i = 0; i < num_test_cases; i++)
   {
      LgSha1 ts;

      const char* hash_input = test_cases[i].input;
      const char* expected_hash = test_cases[i].expected_hash;

      ASSERT_EQ(strlen(hash_input), i);
      ASSERT_EQ(strlen(expected_hash), 40);
      ts.Add((const uint8_t*)hash_input, strlen(hash_input));
      ts.Finalize();
      ASSERT_TRUE(ts.m_finalized);
      ts.ExtractToCstring(hash_output, false);

      //printf("Iteration %zu, Expected hash: %s, Actual hash: %s\n", i, expected_hash, hash_output);

      ASSERT_EQ(strcmp(expected_hash, hash_output), 0);
   }
}
