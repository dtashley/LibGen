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

//Double-checks the hash of a strings out to 130 bytes.  Also double-checks the following behavior:
//  a) The AddOne method.
//  b) The IsFinalized method.
//  c) The Clear method.
//  d) Overruns SHA1 objects.
//  e) Overruns of arrays used to extract the SHA1 hash.
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
          //   2
          "ab",
          "da23614e02469a0d7c7bd1bdab5c9c474b1904dc"
       },
       {
          //   3
          "abc",
          "a9993e364706816aba3e25717850c26c9cd0d89d"
       },
       {
          //   4
          "abcd",
          "81fe8bfe87576c3ecb22426f8e57847382917acf"
       },
       {
          //   5
          "abcde",
          "03de6c570bfe24bfc328ccd7ca46b76eadaf4334"
       },
       {
          //   6
          "abcdef",
          "1f8ac10f23c5b5bc1167bda84b833e5c057a77d2"
       },
       {
          //   7
          "abcdefg",
          "2fb5e13419fc89246865e7a324f476ec624e8740"
       },
       {
          //   8
          "abcdefgh",
          "425af12a0743502b322e93a015bcf868e324d56a"
       },
       { 
          //   9
          "abcdefghi",
          "c63b19f1e4c8b5f76b25c49b8b87f57d8e4872a1"
       },
       {
          //  10
          "abcdefghij",
          "d68c19a0a345b7eab78d5e11e991c026ec60db63"
       },
       { 
          //  11
          "abcdefghijk", 
          "5dfac39f71ad4d35a153ba4fc12d943a0e178e6a" 
       },
       { 
          //  12
          "abcdefghijkl", 
          "eb4608cebfcfd4df81410cbd06507ea6af978d9c" 
       },
       { 
          //  13
          "abcdefghijklm", 
          "4b9892b6527214afc655b8aa52f4d203c15e7c9c" 
       },
       { 
          //  14
          "abcdefghijklmn", 
          "85d7c5ff403abe72df5b8a2708821ee33cd0bcce" 
       },
       {
          //  15
          "abcdefghijklmno",
          "2938dcc2e3aa77987c7e5d4a0f26966706d06782"
       },
       {
          //  16
          "abcdefghijklmnop",
          "14f3995288acd189e6e50a7af47ee7099aa682b9"
       },
       {
          //  17
          "abcdefghijklmnopq",
          "d83d621fcd2d4d2985547043a7a5fd4d0a090a54"
       },
       {
          //  18
          "abcdefghijklmnopqr",
          "e381fe42c57e48a082178641effd1cb90771021a"
       },
       {
          //  19
          "abcdefghijklmnopqrs",
          "893e69ff0109f3459c4243013b3de8b12b41a30e"
       },
       {
          //  20
          "abcdefghijklmnopqrst",
          "14a23ad70f2a5dd725575de6c43e1cdd8b15e3e5"
       },
       {
          //  21
          "abcdefghijklmnopqrstu",
          "eca986b95d587f34d71ca7752a4e0010bfb5c6f4"
       },
       {
          //  22
          "abcdefghijklmnopqrstuv",
          "c436f0485bf34625ff7764ead634bccd875b514d"
       },
       {
          //  23
          "abcdefghijklmnopqrstuvw",
          "f41a73ec7a087859800f5c4cbf71e94487346b6c"
       },
       {
          //  24
          "abcdefghijklmnopqrstuvwx",
          "d717e22e1659305fad6ef08864923db64aba9c08"
       },
       {
          //  25
          "abcdefghijklmnopqrstuvwxy",
          "97b2cc05c4620ebf4ce1330623b950a4a171f7e2"
       },
       {
          //  26
          "abcdefghijklmnopqrstuvwxyz",
          "32d10c7b8cf96570ca04ce37f2a19d84240d3a89"
       },
       {
          //  27
          "abcdefghijklmnopqrstuvwxyz0",
          "1f06dac650d75bea9c5fc39501cbee940a9840b6"
       },
       {
          //  28
          "abcdefghijklmnopqrstuvwxyz01",
          "cee916fb7b45ac3f1eb0ec5fd2e89590e91c5340"
       },
       {
          //  29
          "abcdefghijklmnopqrstuvwxyz012",
          "16ef74baddb4e6df5093d195e8cc2a4c6a13f0df"
       },
       {
          //  30
          "abcdefghijklmnopqrstuvwxyz0123",
          "c2d4d1a28f23979da6a624eaa341f09cacfd120a"
       },
       {
          //  31
          "abcdefghijklmnopqrstuvwxyz01234",
          "5bfeaef7ffac97436977763e1c4d109bcdddc098"
       },
       {
          //  32
          "abcdefghijklmnopqrstuvwxyz012345",
          "a3eb15172cc7e6090a2eb32e6dc8c3bd30c39a02"
       },
       {
          //  33
          "abcdefghijklmnopqrstuvwxyz0123456",
          "6827cf9752e8a116ed5ced73befafc0f4b8e162a"
       },
       {
          //  34
          "abcdefghijklmnopqrstuvwxyz01234567",
          "ab70fab3b46c53185ff80b0814cbba65df0be2b9"
       },
       {
          //  35
          "abcdefghijklmnopqrstuvwxyz012345678",
          "ba084b76039a36e2b8937db5b0ce13af7283b327"
       },
       {
          //  36
          "abcdefghijklmnopqrstuvwxyz0123456789",
          "d2985049a677bbc4b4e8dea3b89c4820e5668e3a"
       },
       {
          //  37
          "abcdefghijklmnopqrstuvwxyz0123456789A",
          "db52a54bf70f70eb1199825f55fdab77724b7b62"
       },
       {
          //  38
          "abcdefghijklmnopqrstuvwxyz0123456789AB",
          "efd372f4d69baef6c0b98c30d9661e94cfda0786"
       },
       {
          //  39
          "abcdefghijklmnopqrstuvwxyz0123456789ABC",
          "ea833b6cb07765232dd7a357bfc7670e59f37884"
       },
       {
          //  40
          "abcdefghijklmnopqrstuvwxyz0123456789ABCD",
          "e5e3689be0074bdaa62087570615090a03b38b7c"
       },
       {
          //  41
          "abcdefghijklmnopqrstuvwxyz0123456789ABCDE",
          "1457c246201d6726e1bde5a8f443674dfd0843fe"
       },
       {
          //  42
          "abcdefghijklmnopqrstuvwxyz0123456789ABCDEF",
          "010f948abfa803a331126d12ee5f09db581618fc"
       },
       {
          //  43
          "abcdefghijklmnopqrstuvwxyz0123456789ABCDEFG",
          "1d9ba1363b8457e1f3f376adcd893750c272c77b"
       },
       {
          //  44
          "abcdefghijklmnopqrstuvwxyz0123456789ABCDEFGH",
          "3d24199f6a51c42590ef18c724af98c11a9054d4"
       },
       {
          //  45
          "abcdefghijklmnopqrstuvwxyz0123456789ABCDEFGHI",
          "11cc02945a5b65d84e1575415f3252eba314d87e"
       },
       {
          //  46
          "abcdefghijklmnopqrstuvwxyz0123456789ABCDEFGHIJ",
          "eb8b34e72146f04339e68c380de92333202b7b95"
       },
       {
          //  47
          "abcdefghijklmnopqrstuvwxyz0123456789ABCDEFGHIJK",
          "8aa1ef2317a24cd997b7b1d1688f360691994890"
       },
       {
          //  48
          "abcdefghijklmnopqrstuvwxyz0123456789ABCDEFGHIJKL",
          "470244a6b03026ededc7ba9bbb1351869de55a5f"
       },
       {
          //  49
          "abcdefghijklmnopqrstuvwxyz0123456789ABCDEFGHIJKLM",
          "ad1254d7ed069700e41fa92e20fd248e0b665c2a"
       },
       {
          //  50
          "abcdefghijklmnopqrstuvwxyz0123456789ABCDEFGHIJKLMN",
          "e73e21df183113f1b807deb5fba92cf9a9f89d38"
       },
       {
          //  51
          "abcdefghijklmnopqrstuvwxyz0123456789ABCDEFGHIJKLMNO",
          "7b6004d4fd4fbf220d788987be23b9b248bd3d3a"
       },
       {
          //  52
          "abcdefghijklmnopqrstuvwxyz0123456789ABCDEFGHIJKLMNOP",
          "eec0d7acb76fb2069b7fbfb8c975bd47966bd3d0"
       },
       {
          //  53
          "abcdefghijklmnopqrstuvwxyz0123456789ABCDEFGHIJKLMNOPQ",
          "d740c0d50cfb183622fda22109ebc5850ea458c6"
       },
       {
          //  54
          "abcdefghijklmnopqrstuvwxyz0123456789ABCDEFGHIJKLMNOPQR",
          "42c8197689df02366f11956cb42a5166c1619aa1"
       },
       {
          //  55
          "abcdefghijklmnopqrstuvwxyz0123456789ABCDEFGHIJKLMNOPQRS",
          "b720993221bd1cc79959e921e9857b851d2982e0"
       },
       {
          //  56
          "abcdefghijklmnopqrstuvwxyz0123456789ABCDEFGHIJKLMNOPQRST",
          "9d3f49d84e67b34664458fdca6e74b9386c654ff"
       },
       {
          //  57
          "abcdefghijklmnopqrstuvwxyz0123456789ABCDEFGHIJKLMNOPQRSTU",
          "a14f45b52008e0be3afb6da23eb79eb9d1347173"
       },
       {
          //  58
          "abcdefghijklmnopqrstuvwxyz0123456789ABCDEFGHIJKLMNOPQRSTUV",
          "276d10cd1af3ee46edf8085c753716d7dc30d98b"
       },
       {
          //  59
          "abcdefghijklmnopqrstuvwxyz0123456789ABCDEFGHIJKLMNOPQRSTUVW",
          "212ccbdb03a93b8a5f1a2a98eb9e279e1641bf14"
       },
       {
          //  60
          "abcdefghijklmnopqrstuvwxyz0123456789ABCDEFGHIJKLMNOPQRSTUVWX",
          "29104e033f181e01628a5c6492af3a72601fbe0a"
       },
       {
          //  61
          "abcdefghijklmnopqrstuvwxyz0123456789ABCDEFGHIJKLMNOPQRSTUVWXY",
          "aab3e756c3301ec5d8bdfe1d0af3c49bc9e6151d"
       },
       {
          //  62
          "abcdefghijklmnopqrstuvwxyz0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ",
          "e9f9fa76318da606b3da2fabdd076bdbfa36fb0c"
       },
       {
          //  63
          "abcdefghijklmnopqrstuvwxyz0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ-",
          "4b4dbd23e529ab855f741c9d53a74d041c948b55"
       },
       {
          //  64
          "abcdefghijklmnopqrstuvwxyz0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ-:",
          "adab849782d8691f675b224aaa8c051407d83ebf"
       },
       {
          //  65
          "abcdefghijklmnopqrstuvwxyz0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ-:.",
          "ec218f6fc583607e957a249cf455f3ea72d791fb"
       },
       {
          //  66
          "abcdefghijklmnopqrstuvwxyz0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ-:.$",
          "9886f8ede342bf46ad71b018d7f0a0fb9e762580"
       },
       {
          //  67
          "abcdefghijklmnopqrstuvwxyz0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ-:.$+",
          "735d40535b997d720b5fbd4199b03033cd3f572a"
       },
       {
          //  68
          "abcdefghijklmnopqrstuvwxyz0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ-:.$+a",
          "b87159f615151db295b3f33d426504c4e369caeb"
       },
       {
          //  69
          "abcdefghijklmnopqrstuvwxyz0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ-:.$+ab",
          "8e0b45424b3d9ae55c130dd6d970b1e0299dc93b"
       },
       {
          //  70
          "abcdefghijklmnopqrstuvwxyz0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ-:.$+abc",
          "9cb1b58e73508bc6279125717e1fe2d10e084c89"
       },
       {
          //  71
          "abcdefghijklmnopqrstuvwxyz0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ-:.$+abcd",
          "e92bf35cc5246530089386b0fc79ccc6d2382575"
       },
       {
          //  72
          "abcdefghijklmnopqrstuvwxyz0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ-:.$+abcde",
          "7cd0ead871ed2e23cb4b90baa016f29a64f9a6ba"
       },
       {
          //  73
          "abcdefghijklmnopqrstuvwxyz0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ-:.$+abcdef",
          "5e059830e79bac1ae631f9d63c0a4af223265e74"
       },
       {
          //  74
          "abcdefghijklmnopqrstuvwxyz0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ-:.$+abcdefg",
          "19952d6839fd56cbbaede81057de9b9b519e2419"
       },
       {
          //  75
          "abcdefghijklmnopqrstuvwxyz0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ-:.$+abcdefgh",
          "95c7f34945c597b8eea18ae061709a6513912950"
       },
       {
          //  76
          "abcdefghijklmnopqrstuvwxyz0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ-:.$+abcdefghi",
          "04b8bff0d6768cc8de16647540b33273d4650a86"
       },
       {
          //  77
          "abcdefghijklmnopqrstuvwxyz0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ-:.$+abcdefghij",
          "05d54d829f65132bcf67943945b9e2ccde79e498"
       },
       {
          //  78
          "abcdefghijklmnopqrstuvwxyz0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ-:.$+abcdefghijk",
          "a9f6bc09748d3d5072b763c8751f93d07d075991"
       },
       {
          //  79
          "abcdefghijklmnopqrstuvwxyz0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ-:.$+abcdefghijkl",
          "c62cf4c7e00427cf0a492baf55bcbd16296e167f"
       },
       {
          //  80
          "abcdefghijklmnopqrstuvwxyz0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ-:.$+abcdefghijklm",
          "ec1f8ba079853f93542133fc0f0105273cb89943"
       },
       {
          //  81
          "abcdefghijklmnopqrstuvwxyz0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ-:.$+abcdefghijklmn",
          "25a0287ec1c8e3ded54da1ca0c022ed19d71d3ba"
       },
       {
          //  82
          "abcdefghijklmnopqrstuvwxyz0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ-:.$+abcdefghijklmno",
          "35d33fde82bd602ca6d75a0d6667c96bd55e655e"
       },
       {
          //  83
          "abcdefghijklmnopqrstuvwxyz0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ-:.$+abcdefghijklmnop",
          "a36d79eb7b16a1aebba7ee4212ec8facf121652d"
       },
       {
          //  84
          "abcdefghijklmnopqrstuvwxyz0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ-:.$+abcdefghijklmnopq",
          "1688e41d40264f65f6d1f039de513b4da9c1ad1f"
       },
       {
          //  85
          "abcdefghijklmnopqrstuvwxyz0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ-:.$+abcdefghijklmnopqr",
          "08be0eef59cd41f68ed29a08aff9b991189a7368"
       },
       {
          //  86
          "abcdefghijklmnopqrstuvwxyz0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ-:.$+abcdefghijklmnopqrs",
          "b6e51874839c49672dd00dbd3b53b0f1d14dbb24"
       },
       {
          //  87
          "abcdefghijklmnopqrstuvwxyz0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ-:.$+abcdefghijklmnopqrst",
          "f496c647c23a9f56855cd50526677e0f15c39d22"
       },
       {
          //  88
          "abcdefghijklmnopqrstuvwxyz0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ-:.$+abcdefghijklmnopqrstu",
          "c80b5a499a703bc9dcb9958b9bb9260fc3d49e3f"
       },
       {
          //  89
          "abcdefghijklmnopqrstuvwxyz0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ-:.$+abcdefghijklmnopqrstuv",
          "010f2b1fa3eef3b887fc1b1ecd362b3baaa4acc9"
       },
       {
          //  90
          "abcdefghijklmnopqrstuvwxyz0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ-:.$+abcdefghijklmnopqrstuvw",
          "c57a44b1740c767fdd8ad2f9947bb04e4b6d2354"
       },
       {
          //  91
          "abcdefghijklmnopqrstuvwxyz0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ-:.$+abcdefghijklmnopqrstuvwx",
          "03305a3b1b722f8c8699fee5fbec5536e0bb20aa"
       },
       {
          //  92
          "abcdefghijklmnopqrstuvwxyz0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ-:.$+abcdefghijklmnopqrstuvwxy",
          "9453d5a8dc584828c95a1e2d931e3d475a30a4a9"
       },
       {
          //  93
          "abcdefghijklmnopqrstuvwxyz0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ-:.$+abcdefghijklmnopqrstuvwxyz",
          "b26b0d37f575f86e1314eed36dcb77dc58636c3a"
       },
       {
          //  94
          "abcdefghijklmnopqrstuvwxyz0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ-:.$+abcdefghijklmnopqrstuvwxyz0",
          "ca3110edb2f5cc9da8dd334e9bff85873bc39ee8"
       },
       {
          //  95
          "abcdefghijklmnopqrstuvwxyz0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ-:.$+abcdefghijklmnopqrstuvwxyz01",
          "bdc76cabc079e2fd295b75d041a35902c6df4498"
       },
       {
          //  96
          "abcdefghijklmnopqrstuvwxyz0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ-:.$+abcdefghijklmnopqrstuvwxyz012",
          "ec9441698ae3db21cf0c5ed9df697043857f1f9e"
       },
       {
          //  97
          "abcdefghijklmnopqrstuvwxyz0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ-:.$+abcdefghijklmnopqrstuvwxyz0123",
          "f6f2af6fe6cee3bc2c01dde7060a63dcbae3a6b1"
       },
       {
          //  98
          "abcdefghijklmnopqrstuvwxyz0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ-:.$+abcdefghijklmnopqrstuvwxyz01234",
          "aa173225afdcfa5387607c73a80e02a9735a4080"
       },
       {
          //  99
          "abcdefghijklmnopqrstuvwxyz0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ-:.$+abcdefghijklmnopqrstuvwxyz012345",
          "99bebad05324456b6738ec12eca2e2c126a53ea7"
       },
       {
          // 100
          "abcdefghijklmnopqrstuvwxyz0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ-:.$+abcdefghijklmnopqrstuvwxyz0123456",
          "0c2c40f22c6562ba02ec8da412da100d0fea76ba"
       },
       {
          // 101
          "abcdefghijklmnopqrstuvwxyz0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ-:.$+abcdefghijklmnopqrstuvwxyz01234567",
          "032ea8350c8a9468f172a991f47eaa5972116b8f"
       },
       {
          // 102
          "abcdefghijklmnopqrstuvwxyz0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ-:.$+abcdefghijklmnopqrstuvwxyz012345678",
          "345c6455647145685af14f280ad8497608331f45"
       },
       {
          // 103
          "abcdefghijklmnopqrstuvwxyz0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ-:.$+abcdefghijklmnopqrstuvwxyz0123456789",
          "70ee5475509542f906126ae84444169672d7cf05"
       },
       {
          // 104
          "abcdefghijklmnopqrstuvwxyz0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ-:.$+abcdefghijklmnopqrstuvwxyz0123456789A",
          "d6db5e8a558f789e6f5b780fa889efe3416262b9"
       },
       {
          // 105
          "abcdefghijklmnopqrstuvwxyz0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ-:.$+abcdefghijklmnopqrstuvwxyz0123456789AB",
          "fd78142abf8f5d305d348814129a7f6afec234e1"
       },
       {
          // 106
          "abcdefghijklmnopqrstuvwxyz0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ-:.$+abcdefghijklmnopqrstuvwxyz0123456789ABC",
          "d7ebe92a1aee89b71bff5a6a62a517335f65afbd"
       },
       {
          // 107
          "abcdefghijklmnopqrstuvwxyz0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ-:.$+abcdefghijklmnopqrstuvwxyz0123456789ABCD",
          "e6a35266f3da502039c1c725e7f3f098226ccc11"
       },
       {
          // 108
          "abcdefghijklmnopqrstuvwxyz0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ-:.$+abcdefghijklmnopqrstuvwxyz0123456789ABCDE",
          "fb1e958fd4041e6c2946d7257ad6bfbe329cf319"
       },
       {
          // 109
          "abcdefghijklmnopqrstuvwxyz0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ-:.$+abcdefghijklmnopqrstuvwxyz0123456789ABCDEF",
          "f63355109b5578a1092f0106727dbde33b4c0b13"
       },
       {
          // 110
          "abcdefghijklmnopqrstuvwxyz0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ-:.$+abcdefghijklmnopqrstuvwxyz0123456789ABCDEFG",
          "79166deb4f3e652dd434220e6bf67ad7c9d917d5"
       },
       {
          // 111
          "abcdefghijklmnopqrstuvwxyz0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ-:.$+abcdefghijklmnopqrstuvwxyz0123456789ABCDEFGH",
          "711d6a8cc00de3d751c174bd695ec9184a48e731"
       },
       {
          // 112
          "abcdefghijklmnopqrstuvwxyz0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ-:.$+abcdefghijklmnopqrstuvwxyz0123456789ABCDEFGHI",
          "221fff4ba5de3d678269990d7cf844ebeae24ac0"
       },
       {
          // 113
          "abcdefghijklmnopqrstuvwxyz0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ-:.$+abcdefghijklmnopqrstuvwxyz0123456789ABCDEFGHIJ",
          "daf2bc68fdfb9310be85025b8f0917b6081888ac"
       },
       {
          // 114
          "abcdefghijklmnopqrstuvwxyz0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ-:.$+abcdefghijklmnopqrstuvwxyz0123456789ABCDEFGHIJK",
          "39c75378fcd5bf2c157519e8fc89bd67f67a6218"
       },
       {
          // 115
          "abcdefghijklmnopqrstuvwxyz0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ-:.$+abcdefghijklmnopqrstuvwxyz0123456789ABCDEFGHIJKL",
          "8f821165f060dfe976661af52d60e1e7fc5cad07"
       },
       {
          // 116
          "abcdefghijklmnopqrstuvwxyz0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ-:.$+abcdefghijklmnopqrstuvwxyz0123456789ABCDEFGHIJKLM",
          "6db3ba00981e7d4a99e0dff4dd70b5b57f44b9c8"
       },
       {
          // 117
          "abcdefghijklmnopqrstuvwxyz0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ-:.$+abcdefghijklmnopqrstuvwxyz0123456789ABCDEFGHIJKLMN",
          "4c06d2e71e4486ed2b144adc406376a5a6e0e99f"
       },
       {
          // 118
          "abcdefghijklmnopqrstuvwxyz0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ-:.$+abcdefghijklmnopqrstuvwxyz0123456789ABCDEFGHIJKLMNO",
          "7ba3ea243611cf675e3dda771e497f6de3d27bb0"
       },
       {
          // 119
          "abcdefghijklmnopqrstuvwxyz0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ-:.$+abcdefghijklmnopqrstuvwxyz0123456789ABCDEFGHIJKLMNOP",
          "08241fd8cb572f371ad94b332b3c09cd1945a4c3"
       },
       {
          // 120
          "abcdefghijklmnopqrstuvwxyz0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ-:.$+abcdefghijklmnopqrstuvwxyz0123456789ABCDEFGHIJKLMNOPQ",
          "3c77dd79be47f726ffad1ad42a65ee817febb424"
       },
       {
          // 121
          "abcdefghijklmnopqrstuvwxyz0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ-:.$+abcdefghijklmnopqrstuvwxyz0123456789ABCDEFGHIJKLMNOPQR",
          "6081bdf6eb4bcac9e6bb039876f588842fd41002"
       },
       {
          // 122
          "abcdefghijklmnopqrstuvwxyz0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ-:.$+abcdefghijklmnopqrstuvwxyz0123456789ABCDEFGHIJKLMNOPQRS",
          "e6e096f698073193b6bd0b8e1665b7ba64a11b11"
       },
       {
          // 123
          "abcdefghijklmnopqrstuvwxyz0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ-:.$+abcdefghijklmnopqrstuvwxyz0123456789ABCDEFGHIJKLMNOPQRST",
          "9332f76ec33bca499b8e0aa0f579bb13445b1a43"
       },
       {
          // 124
          "abcdefghijklmnopqrstuvwxyz0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ-:.$+abcdefghijklmnopqrstuvwxyz0123456789ABCDEFGHIJKLMNOPQRSTU",
          "ea41b7919a7e5fc325778504371d2c3bd00aba6f"
       },
       {
          // 125
          "abcdefghijklmnopqrstuvwxyz0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ-:.$+abcdefghijklmnopqrstuvwxyz0123456789ABCDEFGHIJKLMNOPQRSTUV",
          "addc249764e3eae3d30244716da53905daee7fc2"
       },
       {
          // 126
          "abcdefghijklmnopqrstuvwxyz0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ-:.$+abcdefghijklmnopqrstuvwxyz0123456789ABCDEFGHIJKLMNOPQRSTUVW",
          "1a734bff5def4fd0b15b4425527994a240283a0e"
       },
       {
          // 127
          "abcdefghijklmnopqrstuvwxyz0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ-:.$+abcdefghijklmnopqrstuvwxyz0123456789ABCDEFGHIJKLMNOPQRSTUVWX",
          "3abc3e853a54900c043d2bda2945460af9e7a4d6"
       },
       {
          // 128
          "abcdefghijklmnopqrstuvwxyz0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ-:.$+abcdefghijklmnopqrstuvwxyz0123456789ABCDEFGHIJKLMNOPQRSTUVWXY",
          "c64e3ee2fc70e364a5b8c5d11488b125977e8129"
       },
       {
          // 129
          "abcdefghijklmnopqrstuvwxyz0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ-:.$+abcdefghijklmnopqrstuvwxyz0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ",
          "d30aaca25ad6be601b3b063f7f9c9fc338874108"
       },
       {
          // 130
          "abcdefghijklmnopqrstuvwxyz0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ-:.$+abcdefghijklmnopqrstuvwxyz0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ-",
          "68f434f75775f6fe636c53f6d1d159901fd38ecc"
       },
    };

   size_t num_test_cases = sizeof(test_cases) / sizeof(test_cases[0]);
   size_t i, j;
   char hash_output[41] = { 0 };

   //Double check the test vectors.
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

   //Double check the Clear method.
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

      ts.Clear();
      ts.Add((const uint8_t*)hash_input, strlen(hash_input));
      ts.Finalize();
      ASSERT_TRUE(ts.m_finalized);
      ts.ExtractToCstring(hash_output, false);
      ASSERT_EQ(strcmp(expected_hash, hash_output), 0);

      ts.Clear();
      ts.Add((const uint8_t*)hash_input, strlen(hash_input));
      ASSERT_FALSE(ts.m_finalized);

      ts.Clear();
      ASSERT_FALSE(ts.m_finalized);
      ts.Add((const uint8_t*)hash_input, strlen(hash_input));
      ts.Finalize();
      ASSERT_TRUE(ts.m_finalized);
      ts.ExtractToCstring(hash_output, false);
      ASSERT_EQ(strcmp(expected_hash, hash_output), 0);
   }

   //Double check the AddOne method.
   for (i = 0; i < num_test_cases; i++)
   {
      LgSha1 ts;

      const char* hash_input = test_cases[i].input;
      const char* expected_hash = test_cases[i].expected_hash;

      ASSERT_EQ(strlen(hash_input), i);
      ASSERT_EQ(strlen(expected_hash), 40);

      for (j = 0; j < strlen(hash_input); j++)
      {
         ts.AddOne((uint8_t)hash_input[j]);
      }
      ts.Finalize();
      ASSERT_TRUE(ts.m_finalized);
      ts.ExtractToCstring(hash_output, false);

      //printf("Iteration %zu, Expected hash: %s, Actual hash: %s\n", i, expected_hash, hash_output);

      ASSERT_EQ(strcmp(expected_hash, hash_output), 0);
   }

   //Check for overruns of the SHA1 objects.
   {
      LgSha1 ts[3]; //Each element should be initialized by constructor.
      char hash_output[43] = { 0 };
      uint8_t hash_output_bytes[22] = { 0 };
      uint32_t hash_output_words[7] = { 0 };

      //Load up the arrays with markers at both ends so it
      //can be checked if there were overruns.
      hash_output[0] = (char)0xA5;
      hash_output[42] = (char)0xA5;
      hash_output_bytes[0] = 0xA5;
      hash_output_bytes[21] = 0xA5;
      hash_output_words[0] = 0xA5A5A5A5;
      hash_output_words[6] = 0xA5A5A5A5;

      for (i = 0; i < num_test_cases; i++)
      {
         const char* hash_input = test_cases[i].input;
         const char* expected_hash = test_cases[i].expected_hash;

         ts[1].Add((const uint8_t*)hash_input, strlen(hash_input));
      }

      ts[1].Finalize();

      //Extract using pointers one past the start, so we can look
      //for writes that should not have occurred.
      ts[1].ExtractToCstring(hash_output+1);
      ts[1].ExtractToByteArray(hash_output_bytes + 1);
      ts[1].ExtractToUint32Array(hash_output_words + 1);

      //Check for overruns in the buffers used for copying the hash out.
      ASSERT_EQ(hash_output[0], (char)0xA5);
      ASSERT_EQ(hash_output[42], (char)0xA5);
      ASSERT_EQ(hash_output_bytes[0], 0xA5);
      ASSERT_EQ(hash_output_bytes[21], 0xA5);
      ASSERT_EQ(hash_output_words[0], 0xA5A5A5A5);
      ASSERT_EQ(hash_output_words[6], 0xA5A5A5A5);

      //Check for object overruns.
      ASSERT_EQ(ts[0].m_bufferSize, 0);
      ASSERT_EQ(ts[2].m_bufferSize, 0);
      ASSERT_EQ(ts[0].m_bitCount, 0);
      ASSERT_EQ(ts[2].m_bitCount, 0);
      ASSERT_FALSE(ts[0].m_finalized);
      ASSERT_FALSE(ts[2].m_finalized);
      ASSERT_EQ(ts[0].m_state[0], 0x67452301);
      ASSERT_EQ(ts[2].m_state[0], 0x67452301);
      ASSERT_EQ(ts[0].m_state[1], 0xefcdab89);
      ASSERT_EQ(ts[2].m_state[1], 0xefcdab89);
      ASSERT_EQ(ts[0].m_state[2], 0x98badcfe);
      ASSERT_EQ(ts[2].m_state[2], 0x98badcfe);
      ASSERT_EQ(ts[0].m_state[3], 0x10325476);
      ASSERT_EQ(ts[2].m_state[3], 0x10325476);
      ASSERT_EQ(ts[0].m_state[4], 0xc3d2e1f0);
      ASSERT_EQ(ts[2].m_state[4], 0xc3d2e1f0);

      for (i = 0; i < 64; i++)
      {
         ASSERT_EQ(ts[0].m_buffer[i], 0);
         ASSERT_EQ(ts[2].m_buffer[i], 0);
      }
   }
}
