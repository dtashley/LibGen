#include "gtest/gtest.h"

#include "lg_cpp_dd_buf_uint8.hpp"


class LgBufUint8Test : public testing::Test {
protected:
};


//Ensures that object starts initialized properly.
TEST_F(LgBufUint8Test, IsEmptyInitially)
{
   LgBufUint8 ts;

   ASSERT_EQ(ts.m_n_allocd, 0);
   ASSERT_EQ(ts.m_n_used,   0);
   ASSERT_EQ(ts.m_errs,     0);
   ASSERT_EQ(ts.m_bufptr,   nullptr);
}


//Ensures two empty objects compare equal.
TEST_F(LgBufUint8Test, EmptyObjectsCompareEqual)
{
   LgBufUint8 ts1;
   LgBufUint8 ts2;

   ASSERT_EQ(ts1.m_n_allocd, 0);
   ASSERT_EQ(ts1.m_n_used, 0);
   ASSERT_EQ(ts1.m_errs, 0);
   ASSERT_EQ(ts1.m_bufptr, nullptr);

   ASSERT_EQ(ts2.m_n_allocd, 0);
   ASSERT_EQ(ts2.m_n_used, 0);
   ASSERT_EQ(ts2.m_errs, 0);
   ASSERT_EQ(ts2.m_bufptr, nullptr);

   ASSERT_EQ(ts1.Cmp(ts2), 0);
   ASSERT_EQ(ts2.Cmp(ts1), 0);

   ASSERT_FALSE(ts1 <  ts2);
   ASSERT_TRUE (ts1 <= ts2);
   ASSERT_TRUE (ts1 == ts2);
   ASSERT_FALSE(ts1 != ts2);
   ASSERT_TRUE (ts1 >= ts2);
   ASSERT_FALSE(ts1 >  ts2);

   ASSERT_FALSE(ts2 <  ts1);
   ASSERT_TRUE (ts2 <= ts1);
   ASSERT_TRUE (ts2 == ts1);
   ASSERT_FALSE(ts2 != ts1);
   ASSERT_TRUE (ts2 >= ts1);
   ASSERT_FALSE(ts2 >  ts1);
}


//Ensures empty object tests less than populated object.
TEST_F(LgBufUint8Test, EmptyObjectTestLessThanPopulated)
{
   LgBufUint8 ts1;
   LgBufUint8 ts2;
   unsigned i;

   for (i = 0; i < 10000; i++)
   {
      ts1.AppendOne(i);
      ASSERT_FALSE(ts1 <  ts2);
      ASSERT_FALSE(ts1 <= ts2);
      ASSERT_FALSE(ts1 == ts2);
      ASSERT_TRUE (ts1 != ts2);
      ASSERT_TRUE (ts1 >= ts2);
      ASSERT_TRUE (ts1 > ts2);
   }
}


//Ensures same-sized objects compared based on content.
TEST_F(LgBufUint8Test, SameSizeObjectsComparedOnContent)
{
   LgBufUint8 ts1;
   LgBufUint8 ts2;
   unsigned i;

   for (i = 0; i < 10000; i++)
   {
      ts1.AppendOne(i);
      ts2.AppendOne(i);
   }

   ASSERT_TRUE(ts1 == ts2);

   ts1[500] = 255;

   ASSERT_FALSE(ts1 == ts2);
   ASSERT_TRUE(ts1 > ts2);

   ts1[500] = 0;

   ASSERT_FALSE(ts1 == ts2);
   ASSERT_FALSE(ts1 > ts2);

   ts1[500] = (uint8_t)500;

   ASSERT_TRUE(ts1 == ts2);

   ts1[9999] = 255;

   ASSERT_FALSE(ts1 == ts2);
   ASSERT_TRUE(ts1 > ts2);
}

//Checks the error getting and clearing functions.
TEST_F(LgBufUint8Test, ErrGetAndClear)
{
   LgBufUint8 ts1;
   unsigned result;

   //Attempt to read a file that presumably does not exist.
   ts1.Fread("xxqqzz98_5576.noexist");

   result = ts1.ErrsGet();

   ASSERT_EQ(result, LG_CPP_DD_BUF_UINT8_ERR_FOPEN);

   ts1.ErrsClear();

   result = ts1.ErrsGet();

   ASSERT_EQ(result, 0);
}


//Checks file reading and writing, but happy path only.
TEST_F(LgBufUint8Test, FreadFwriteHappyPathOnly)
{
   const char* s = "0123456789abcdefghijklmnopqrstuvwxyz-+/*ABCDEFGHIJKLMNOPQRSTUVWXYZ";
   LgBufUint8 ts1, ts2;
   const size_t slen = strlen(s);
   unsigned prng = 0;
   bool bresult = false;
   size_t iter;
   //unsigned result;

   //Form a buffer with known reproducible contents.
   for (iter = 0; iter < 5000; iter++)
   {
      ts1.AppendOne(s[prng]);
      prng = (prng + 61) % slen;
   }
   
   //Dump to a file.
   bresult = ts1.Fwrite("ut_temp_FreadFwriteHappyPathOnly.txt");
   ASSERT_TRUE(bresult);

   //Read from the file to the other variable.
   bresult = ts2.Fread("ut_temp_FreadFwriteHappyPathOnly.txt");
   ASSERT_TRUE(bresult);

   ASSERT_TRUE(ts1 == ts2);
}

//Checks clearing.
TEST_F(LgBufUint8Test, ClearTest)
{
   size_t iter;
   LgBufUint8 ts1;

   //Form a buffer with known reproducible contents.
   for (iter = 0; iter < 5000; iter++)
   {
      ts1.AppendOne((uint8_t)iter);
   }

   ASSERT_EQ(ts1.m_n_used, 5000);
   ASSERT_TRUE(ts1.m_n_allocd >= 5000);

   ts1.Clear();

   ASSERT_EQ(ts1.m_n_used, 0);
   ASSERT_TRUE(ts1.m_n_allocd >= 5000);
}


//Checks trimming.
TEST_F(LgBufUint8Test, TrimTest)
{
   size_t iter, n_used_snapshot, n_allocd_snapshot;
   LgBufUint8 ts1;

   //Fill the thing with 100,000 uint8_t's.
   for (iter = 0; iter < 100000; iter++)
   {
      ts1.AppendOne((uint8_t)iter+17);
   }

   //std::cout << ts1.m_n_used << "\n";

   //Capture the number used and number allocated for further comparison.
   n_used_snapshot = ts1.m_n_used;
   n_allocd_snapshot = ts1.m_n_allocd;

   //Remove a multiple of 256 near 30000.  This ensures that the "+17" relationship holds.
   for (iter = 0; iter < (30000 & 0xFF00u); iter++)
   {
      ts1.DeleteOne(0);
   }

   //In order to pass, the number used has to have decreased as expected.
   ASSERT_TRUE(ts1.m_n_used < n_used_snapshot);

   //In order to pass, the number allocd has to be unchanged.
   ASSERT_EQ(ts1.m_n_allocd, n_allocd_snapshot);

   //std::cout << ts1.m_n_used << "\n";

   //However, we can be slightly more particular.  The number used should have decreased
   //by a very particular amount.
   ASSERT_EQ(ts1.m_n_used, 100000 - (30000 & 0xFF00u));

   //Be sure the +17 relationship still holds.
   for (iter = 0; iter < ts1.Nelem(); iter++)
   {
      ASSERT_EQ(ts1[iter], (iter + 17) & 0xFF);
   }

   //Do the trim.
   ts1.Trim();

   //The amount allocated must less.
   ASSERT_TRUE(ts1.m_n_allocd < n_allocd_snapshot);

   //The number used has to be effectively unchanged.
   ASSERT_EQ(ts1.m_n_used, 100000 - (30000 & 0xFF00u));

   //ts1.DebugWrite(std::cout, "");
}


//Checks clearing and deallocating.
TEST_F(LgBufUint8Test, ClearAndDeallocateTest)
{
   size_t iter;
   LgBufUint8 ts1;

   //Form a buffer with known reproducible contents.
   for (iter = 0; iter < 5000; iter++)
   {
      ts1.AppendOne((uint8_t)iter);
   }

   ASSERT_EQ(ts1.m_n_used, 5000);
   ASSERT_TRUE(ts1.m_n_allocd >= 5000);

   ts1.ClearAndDeallocate();

   ASSERT_EQ(ts1.m_n_used, 0);
   ASSERT_EQ(ts1.m_n_allocd, 0);
   ASSERT_EQ(ts1.m_errs, 0);
   ASSERT_EQ(ts1.m_bufptr, nullptr);
}


//Checks reading one element by value.
TEST_F(LgBufUint8Test, ReadOneByValTest)
{
   size_t iter;
   LgBufUint8 ts1;

   //Form a buffer with known reproducible contents.
   for (iter = 0; iter < 5000; iter++)
   {
      ts1.AppendOne((uint8_t)(iter + 7));
   }

   //Check the read.
   for (iter = 0; iter < 5000; iter++)
   {
      ASSERT_EQ(ts1.ReadOneByVal(iter), (uint8_t)(iter + 7));
   }

   //Check out of range indices.
   ASSERT_EQ(ts1.ReadOneByVal(5000), 0);
   ASSERT_EQ(ts1.ReadOneByVal(5001), 0);
   ASSERT_EQ(ts1.ReadOneByVal(31254319), 0);
}


//Checks reading one element.
TEST_F(LgBufUint8Test, ReadOneTest)
{
   size_t iter;
   LgBufUint8 ts1;
   uint8_t output;

   //Form a buffer with known reproducible contents.
   for (iter = 0; iter < 5000; iter++)
   {
      ts1.AppendOne((uint8_t)(iter + 11));
   }

   //Check reading.
   for (iter = 0; iter < 5000; iter++)
   {
      //Try with a null pointer.  Should be no exception.
      ts1.ReadOne(iter, nullptr);

      //Try a proper read.
      output = 0;
      ts1.ReadOne(iter, &output);
      ASSERT_EQ(output, (uint8_t)(iter + 11));
   }

   //Try four reads out of range.
   output = 1;
   ts1.ReadOne(5000, &output);
   ASSERT_EQ(output, 0);
   output = 1;
   ts1.ReadOne(5001, &output);
   ASSERT_EQ(output, 0);
   output = 1;
   ts1.ReadOne(6000, &output);
   ASSERT_EQ(output, 0);
   output = 1;
   ts1.ReadOne(3128819, &output);
   ASSERT_EQ(output, 0);
}


//Checks read path with read count of zero.
TEST_F(LgBufUint8Test, ReadReadCountZero)
{
   size_t iter;
   LgBufUint8 ts1;
   size_t rv;
   uint8_t buf[25];

   //Form a buffer with known reproducible contents.
   for (iter = 0; iter < 5000; iter++)
   {
      ts1.AppendOne((uint8_t)(iter + 23));
   }

   //Read count of zero, non-null pointer.
   memset(buf, 0xD5, sizeof(buf));
   rv = ts1.Read(5, buf, 0);
   ASSERT_EQ(rv, 0);
   ASSERT_EQ(buf[0], 0xD5);

   //Read count of zero, null pointer.
   rv = ts1.Read(5, nullptr, 0);
   ASSERT_EQ(rv, 0);
}


//Checks path with index out of bounds, not null pointer.
TEST_F(LgBufUint8Test, ReadIndexOobNotNullPtr)
{
   size_t iter;
   LgBufUint8 ts1;
   size_t rv;
   uint8_t buf[25];

   //Form a buffer with known reproducible contents.
   for (iter = 0; iter < 5000; iter++)
   {
      ts1.AppendOne((uint8_t)(iter + 23));
   }

   //Read count of 20, non-null pointer.
   memset(buf, 0xD5, sizeof(buf));
   rv = ts1.Read(5000, buf, 20);
   ASSERT_EQ(rv, 0);
   for (iter = 0; iter < 20; iter++)
   {
      ASSERT_EQ(buf[iter], 0);
   }
   for (iter = 20; iter < sizeof(buf); iter++)
   {
      ASSERT_EQ(buf[iter], 0xD5);
   }
}


//Checks fully contained path, null pointer.
TEST_F(LgBufUint8Test, ReadIndexFullyContainedNullPtr)
{
   size_t iter;
   LgBufUint8 ts1;
   size_t rv;
   uint8_t buf[25];

   //Form a buffer with known reproducible contents.
   for (iter = 0; iter < 5000; iter++)
   {
      ts1.AppendOne((uint8_t)(iter + 23));
   }

   //Read count of 20, null pointer.
   memset(buf, 0xD5, sizeof(buf));
   rv = ts1.Read(1, nullptr, 20);
   ASSERT_EQ(rv, 0);
}


//Checks fully contained path, non-null pointer.
TEST_F(LgBufUint8Test, ReadIndexFullyContainedNonNullPtr)
{
   size_t iter;
   LgBufUint8 ts1;
   size_t rv;
   uint8_t buf[25];

   //Form a buffer with known reproducible contents.
   for (iter = 0; iter < 5000; iter++)
   {
      ts1.AppendOne((uint8_t)(iter + 23));
   }

   //Read count of 20, non-null pointer.
   memset(buf, 0xD5, sizeof(buf));
   rv = ts1.Read(1, buf, 20);
   ASSERT_EQ(rv, 20);
   for (iter = 0; iter < 20; iter++)
   {
      ASSERT_EQ(buf[iter], ts1.m_bufptr[iter+1]);
   }
   for (iter = 20; iter < sizeof(buf); iter++)
   {
      ASSERT_EQ(buf[iter], 0xD5);
   }
}


//Checks partially contained path, non-null pointer.
TEST_F(LgBufUint8Test, ReadIndexPartiallyContainedNonNullPtr)
{
   size_t iter;
   LgBufUint8 ts1;
   size_t rv;
   uint8_t buf[25];

   //Form a buffer with known reproducible contents.
   for (iter = 0; iter < 5000; iter++)
   {
      ts1.AppendOne((uint8_t)(iter + 23));
   }

   //Read count of 20, non-null pointer.
   memset(buf, 0xD5, sizeof(buf));
   rv = ts1.Read(4995, buf, 20);
   ASSERT_EQ(rv, 5);
   for (iter = 0; iter < 5; iter++)
   {
      ASSERT_EQ(buf[iter], ts1.m_bufptr[iter + 4995]);
   }
   for (iter = 5; iter < 20; iter++)
   {
      ASSERT_EQ(buf[iter], 0);
   }
   for (iter = 20; iter < sizeof(buf); iter++)
   {
      ASSERT_EQ(buf[iter], 0xD5);
   }
}


//Checks assign one with empty object and subscript 0.  If something goes wrong with the
//initial test in the function, the nullptr reference should generate an exception and
//a detectable test failure.
TEST_F(LgBufUint8Test, AssignOneEmptyObjSubs0)
{
   LgBufUint8 ts1;

   ts1.AssignOne(0, 0xC9);

   //Double-check that values are as expected.
   ASSERT_EQ(ts1.m_n_used, 0);
   ASSERT_EQ(ts1.m_errs, 0);
   ASSERT_EQ(ts1.m_n_allocd, 0);
   ASSERT_EQ(ts1.m_bufptr, nullptr);
}


//Checks assign one with object of one element and subscript 0.
TEST_F(LgBufUint8Test, AssignOneObj1Subs0)
{
   LgBufUint8 ts1;

   ts1.AppendOne(87);
   ASSERT_EQ(ts1.m_bufptr[0], 87);

   ts1.AssignOne(0, 44);

   //Double-check that values are as expected.
   ASSERT_EQ(ts1.m_n_used, 1);
   ASSERT_EQ(ts1.m_errs, 0);
   ASSERT_TRUE(ts1.m_n_allocd >= ts1.m_n_used);
   ASSERT_TRUE(ts1.m_bufptr != nullptr);
   ASSERT_EQ(ts1.m_bufptr[0], 44);
}


//Checks assign one with object of one element and subscript 1.
TEST_F(LgBufUint8Test, AssignOneObj1Subs1)
{
   LgBufUint8 ts1;

   ts1.AppendOne(87);
   ASSERT_EQ(ts1.m_bufptr[0], 87);

   ts1.AssignOne(1, 44);

   //Double-check that values are as expected.
   ASSERT_EQ(ts1.m_n_used, 1);
   ASSERT_EQ(ts1.m_errs, 0);
   ASSERT_TRUE(ts1.m_n_allocd >= ts1.m_n_used);
   ASSERT_TRUE(ts1.m_bufptr != nullptr);
   ASSERT_EQ(ts1.m_bufptr[0], 87);
}


//Checks assign with empty object and subscript 0, nullptr, n=0.
TEST_F(LgBufUint8Test, AssignObjEmptySubs0NpN0)
{
   LgBufUint8 ts1;

   ts1.Assign(0, nullptr, 0);

   //Double-check that values are as expected.
   ASSERT_EQ(ts1.m_n_used, 0);
   ASSERT_EQ(ts1.m_errs, 0);
   ASSERT_EQ(ts1.m_n_allocd, 0);
   ASSERT_EQ(ts1.m_bufptr, nullptr);
}


//Checks assign with empty object and subscript 0, nullptr, n=1.
TEST_F(LgBufUint8Test, AssignObjEmptySubs0NpN1)
{
   LgBufUint8 ts1;

   ts1.Assign(0, nullptr, 1);

   //Double-check that values are as expected.
   ASSERT_EQ(ts1.m_n_used, 0);
   ASSERT_EQ(ts1.m_errs, 0);
   ASSERT_EQ(ts1.m_n_allocd, 0);
   ASSERT_EQ(ts1.m_bufptr, nullptr);
}


//Checks assign with empty object and subscript 0, nullptr, n=10.
TEST_F(LgBufUint8Test, AssignObjEmptySubs0NpN10)
{
   LgBufUint8 ts1;

   ts1.Assign(0, nullptr, 10);

   //Double-check that values are as expected.
   ASSERT_EQ(ts1.m_n_used, 0);
   ASSERT_EQ(ts1.m_errs, 0);
   ASSERT_EQ(ts1.m_n_allocd, 0);
   ASSERT_EQ(ts1.m_bufptr, nullptr);
}


//Checks assign with empty object and subscript 0, non-nullptr, n=10.
TEST_F(LgBufUint8Test, AssignObjEmptySubs0NnpN10)
{
   size_t iter;
   LgBufUint8 ts1;
   uint8_t buf[10];

   //Block fill the buffer with 0xA9 and down, so we can recognize the pattern.
   for (iter = 0; iter < 10; iter++)
   {
      buf[iter] = ((uint8_t)(0xA9 + iter));
   }

   ts1.Assign(0, buf, 10);

   //Double-check that values are as expected.
   ASSERT_EQ(ts1.m_n_used, 0);
   ASSERT_EQ(ts1.m_errs, 0);
   ASSERT_EQ(ts1.m_n_allocd, 0);
   ASSERT_EQ(ts1.m_bufptr, nullptr);
}


//Checks ordinary assign case, populated object, subscript 0, non-nullptr, n=10, no overrrun.
TEST_F(LgBufUint8Test, AssignObjPopSubs0NnpN10)
{
   size_t iter;
   LgBufUint8 ts1;
   uint8_t buf[10];

   //Block fill the buffer with 0xA9 and down, so we can recognize the pattern.
   for (iter = 0; iter < 10; iter++)
   {
      buf[iter] = ((uint8_t)(0xA9 + iter));
   }

   //Form a buffer with known reproducible contents.
   for (iter = 0; iter < 5000; iter++)
   {
      ts1.AppendOne((uint8_t)(iter + 29));
   }

   //Assign at the location.
   ts1.Assign(0, buf, 10);

   //Double-check that values are as expected.
   ASSERT_EQ(ts1.m_n_used, 5000);
   ASSERT_EQ(ts1.m_errs, 0);
   ASSERT_TRUE(ts1.m_n_allocd >= ts1.m_n_used);
   ASSERT_TRUE(ts1.m_bufptr != nullptr);

   for (iter = 0; iter < 10; iter++)
   {
      ASSERT_EQ(ts1.m_bufptr[iter], buf[iter]);
   }
   for (iter = 10; iter < 5000; iter++)
   {
      ASSERT_EQ(ts1.m_bufptr[iter], (uint8_t)(iter + 29));
   }
}


//Checks ordinary assign case, populated object, subscript 21, non-nullptr, n=10, no overrrun.
TEST_F(LgBufUint8Test, AssignObjPopSubs21NnpN10)
{
   size_t iter;
   LgBufUint8 ts1;
   uint8_t buf[10];

   //Block fill the buffer with 0xA9 and down, so we can recognize the pattern.
   for (iter = 0; iter < 10; iter++)
   {
      buf[iter] = ((uint8_t)(0xA9 + iter));
   }

   //Form a buffer with known reproducible contents.
   for (iter = 0; iter < 5000; iter++)
   {
      ts1.AppendOne((uint8_t)(iter + 29));
   }

   //Assign at the location.
   ts1.Assign(21, buf, 10);

   //Double-check that values are as expected.
   ASSERT_EQ(ts1.m_n_used, 5000);
   ASSERT_EQ(ts1.m_errs, 0);
   ASSERT_TRUE(ts1.m_n_allocd >= ts1.m_n_used);
   ASSERT_TRUE(ts1.m_bufptr != nullptr);

   for (iter = 0; iter < 21; iter++)
   {
      ASSERT_EQ(ts1.m_bufptr[iter], (uint8_t)(iter + 29));
   }
   for (; iter < (21 + 10); iter++)
   {
      ASSERT_EQ(ts1.m_bufptr[iter], buf[iter - 21]);
   }
   for (; iter < 5000; iter++)
   {
      ASSERT_EQ(ts1.m_bufptr[iter], (uint8_t)(iter + 29));
   }
}


//Checks ordinary assign case, populated object, subscript 4989, non-nullptr, n=10, no overrrun.
//This puts the assign block one away from the end of the buffer.
TEST_F(LgBufUint8Test, AssignObjPopSubs4989NnpN10)
{
   size_t iter;
   LgBufUint8 ts1;
   uint8_t buf[10];

   //Block fill the buffer with 0xA9 and down, so we can recognize the pattern.
   for (iter = 0; iter < 10; iter++)
   {
      buf[iter] = ((uint8_t)(0xA9 + iter));
   }

   //Form a buffer with known reproducible contents.
   for (iter = 0; iter < 5000; iter++)
   {
      ts1.AppendOne((uint8_t)(iter + 29));
   }

   //Assign at the location.
   ts1.Assign(4989, buf, 10);

   //Double-check that values are as expected.
   ASSERT_EQ(ts1.m_n_used, 5000);
   ASSERT_EQ(ts1.m_errs, 0);
   ASSERT_TRUE(ts1.m_n_allocd >= ts1.m_n_used);
   ASSERT_TRUE(ts1.m_bufptr != nullptr);

   for (iter = 0; iter < 4989; iter++)
   {
      ASSERT_EQ(ts1.m_bufptr[iter], (uint8_t)(iter + 29));
   }
   for (; iter < (4989 + 10); iter++)
   {
      ASSERT_EQ(ts1.m_bufptr[iter], buf[iter - 4989]);
   }
   for (; iter < 5000; iter++)
   {
      ASSERT_EQ(ts1.m_bufptr[iter], (uint8_t)(iter + 29));
      //std::cout << "Line " << __LINE__ << " reached.\n";
   }
}


//Checks ordinary assign case, populated object, subscript 4990, non-nullptr, n=10, no overrrun.
//This puts the assign block flush with the end of the buffer.
TEST_F(LgBufUint8Test, AssignObjPopSubs4990NnpN10)
{
   size_t iter;
   LgBufUint8 ts1;
   uint8_t buf[10];

   //Block fill the buffer with 0xA9 and down, so we can recognize the pattern.
   for (iter = 0; iter < 10; iter++)
   {
      buf[iter] = ((uint8_t)(0xA9 + iter));
   }

   //Form a buffer with known reproducible contents.  We want to bloat the buffer
   //out so we can check the items beyond the nominal end.
   for (iter = 0; iter < 10000; iter++)
   {
      ts1.AppendOne(0xFE);
   }

   //Clear the buffer.  This should keep the allocation.
   ts1.Clear();

   //Repopulate the buffer.  We want enough allocation (from the previous loop)
   //so that we can poke around past the end, we want a known fill, and some known
   //correct values past the end.
   for (iter = 0; iter < 5000; iter++)
   {
      ts1.AppendOne((uint8_t)(iter + 31));
   }

   //Double-check that our actions were successful.
   ASSERT_EQ(ts1.m_n_used, 5000);
   ASSERT_EQ(ts1.m_errs, 0);
   ASSERT_TRUE(ts1.m_n_allocd >= 10000);  //Overallocated.
   ASSERT_TRUE(ts1.m_bufptr != nullptr);

   //Assign at the location.
   ts1.Assign(4990, buf, 10);

   //Double-check that values are as expected.
   ASSERT_EQ(ts1.m_n_used, 5000);
   ASSERT_EQ(ts1.m_errs, 0);
   ASSERT_TRUE(ts1.m_n_allocd >= 10000);
   ASSERT_TRUE(ts1.m_bufptr != nullptr);

   for (iter = 0; iter < 4990; iter++)
   {
      ASSERT_EQ(ts1.m_bufptr[iter], (uint8_t)(iter + 31));
   }
   for (; iter < (4990 + 10); iter++)
   {
      ASSERT_EQ(ts1.m_bufptr[iter], buf[iter - 4990]);
   }
   for (; iter < 5010; iter++)
   {
      ASSERT_EQ(ts1.m_bufptr[iter], 0xFE);
      //std::cout << "Line " << __LINE__ << " reached, index: " << iter << ".\n";
   }
}


//Checks ordinary assign case, populated object, subscript 4991, non-nullptr, n=10, overrun.
//This puts the assign block end one past the buffer end.
TEST_F(LgBufUint8Test, AssignObjPopSubs4991NnpN10)
{
   size_t iter;
   LgBufUint8 ts1;
   uint8_t buf[10];

   //Block fill the buffer with 0xA9 and down, so we can recognize the pattern.
   for (iter = 0; iter < 10; iter++)
   {
      buf[iter] = ((uint8_t)(0xA9 + iter));
   }

   //Form a buffer with known reproducible contents.  We want to bloat the buffer
   //out so we can check the items beyond the nominal end.
   for (iter = 0; iter < 10000; iter++)
   {
      ts1.AppendOne(0xFE);
   }

   //Clear the buffer.  This should keep the allocation.
   ts1.Clear();

   //Repopulate the buffer.  We want enough allocation (from the previous loop)
   //so that we can poke around past the end, we want a known fill, and some known
   //correct values past the end.
   for (iter = 0; iter < 5000; iter++)
   {
      ts1.AppendOne((uint8_t)(iter + 31));
   }

   //Double-check that our actions were successful.
   ASSERT_EQ(ts1.m_n_used, 5000);
   ASSERT_EQ(ts1.m_errs, 0);
   ASSERT_TRUE(ts1.m_n_allocd >= 10000);  //Overallocated.
   ASSERT_TRUE(ts1.m_bufptr != nullptr);

   //Assign at the location.  The last byte should not be assigned.
   ts1.Assign(4991, buf, 10);

   //Double-check that values are as expected.
   ASSERT_EQ(ts1.m_n_used, 5000);
   ASSERT_EQ(ts1.m_errs, 0);
   ASSERT_TRUE(ts1.m_n_allocd >= 10000);
   ASSERT_TRUE(ts1.m_bufptr != nullptr);

   for (iter = 0; iter < 4991; iter++)
   {
      ASSERT_EQ(ts1.m_bufptr[iter], (uint8_t)(iter + 31));
   }
   for (; iter < (4991 + 9); iter++)
   {
      ASSERT_EQ(ts1.m_bufptr[iter], buf[iter - 4991]);
   }
   for (; iter < 5010; iter++)
   {
      ASSERT_EQ(ts1.m_bufptr[iter], 0xFE);
      //std::cout << "Line " << __LINE__ << " reached, index: " << iter << ".\n";
   }
}


//Checks ordinary assign case, populated object, subscript 4995, non-nullptr, n=10, overrun.
//This puts the assign block end well past buffer end.
TEST_F(LgBufUint8Test, AssignObjPopSubs4995NnpN10)
{
   size_t iter;
   LgBufUint8 ts1;
   uint8_t buf[10];

   //Block fill the buffer with 0xA9 and down, so we can recognize the pattern.
   for (iter = 0; iter < 10; iter++)
   {
      buf[iter] = ((uint8_t)(0xA9 + iter));
   }

   //Form a buffer with known reproducible contents.  We want to bloat the buffer
   //out so we can check the items beyond the nominal end.
   for (iter = 0; iter < 10000; iter++)
   {
      ts1.AppendOne(0xFE);
   }

   //Clear the buffer.  This should keep the allocation.
   ts1.Clear();

   //Repopulate the buffer.  We want enough allocation (from the previous loop)
   //so that we can poke around past the end, we want a known fill, and some known
   //correct values past the end.
   for (iter = 0; iter < 5000; iter++)
   {
      ts1.AppendOne((uint8_t)(iter + 31));
   }

   //Double-check that our actions were successful.
   ASSERT_EQ(ts1.m_n_used, 5000);
   ASSERT_EQ(ts1.m_errs, 0);
   ASSERT_TRUE(ts1.m_n_allocd >= 10000);  //Overallocated.
   ASSERT_TRUE(ts1.m_bufptr != nullptr);

   //Assign at the location.  The last byte should not be assigned.
   ts1.Assign(4995, buf, 10);

   //Double-check that values are as expected.
   ASSERT_EQ(ts1.m_n_used, 5000);
   ASSERT_EQ(ts1.m_errs, 0);
   ASSERT_TRUE(ts1.m_n_allocd >= 10000);
   ASSERT_TRUE(ts1.m_bufptr != nullptr);

   for (iter = 0; iter < 4995; iter++)
   {
      ASSERT_EQ(ts1.m_bufptr[iter], (uint8_t)(iter + 31));
   }
   for (; iter < (4995 + 5); iter++)
   {
      ASSERT_EQ(ts1.m_bufptr[iter], buf[iter - 4995]);
   }
   for (; iter < 5010; iter++)
   {
      ASSERT_EQ(ts1.m_bufptr[iter], 0xFE);
      //std::cout << "Line " << __LINE__ << " reached, index: " << iter << ".\n";
   }
}


//Checks ordinary assign case, populated object, subscript 4999, non-nullptr, n=10, overrun.
//This puts the assign block end well past buffer end, with only one byte overlap.
TEST_F(LgBufUint8Test, AssignObjPopSubs4999NnpN10)
{
   size_t iter;
   LgBufUint8 ts1;
   uint8_t buf[10];

   //Block fill the buffer with 0xA9 and down, so we can recognize the pattern.
   for (iter = 0; iter < 10; iter++)
   {
      buf[iter] = ((uint8_t)(0xA9 + iter));
   }

   //Form a buffer with known reproducible contents.  We want to bloat the buffer
   //out so we can check the items beyond the nominal end.
   for (iter = 0; iter < 10000; iter++)
   {
      ts1.AppendOne(0xFE);
   }

   //Clear the buffer.  This should keep the allocation.
   ts1.Clear();

   //Repopulate the buffer.  We want enough allocation (from the previous loop)
   //so that we can poke around past the end, we want a known fill, and some known
   //correct values past the end.
   for (iter = 0; iter < 5000; iter++)
   {
      ts1.AppendOne((uint8_t)(iter + 31));
   }

   //Double-check that our actions were successful.
   ASSERT_EQ(ts1.m_n_used, 5000);
   ASSERT_EQ(ts1.m_errs, 0);
   ASSERT_TRUE(ts1.m_n_allocd >= 10000);  //Overallocated.
   ASSERT_TRUE(ts1.m_bufptr != nullptr);

   //Assign at the location.  The last byte should not be assigned.
   ts1.Assign(4999, buf, 10);

   //Double-check that values are as expected.
   ASSERT_EQ(ts1.m_n_used, 5000);
   ASSERT_EQ(ts1.m_errs, 0);
   ASSERT_TRUE(ts1.m_n_allocd >= 10000);
   ASSERT_TRUE(ts1.m_bufptr != nullptr);

   for (iter = 0; iter < 4999; iter++)
   {
      ASSERT_EQ(ts1.m_bufptr[iter], (uint8_t)(iter + 31));
   }
   for (; iter < (4999 + 1); iter++)
   {
      ASSERT_EQ(ts1.m_bufptr[iter], buf[iter - 4999]);
   }
   for (; iter < 5010; iter++)
   {
      ASSERT_EQ(ts1.m_bufptr[iter], 0xFE);
      //std::cout << "Line " << __LINE__ << " reached, index: " << iter << ".\n";
   }
}


//Checks ordinary assign case, populated object, subscript 5000, non-nullptr, n=10, out of range.
//This puts the assign block end well past buffer end, with only one byte overlap.
TEST_F(LgBufUint8Test, AssignObjPopSubs5000NnpN10)
{
   size_t iter;
   LgBufUint8 ts1;
   uint8_t buf[10];

   //Block fill the buffer with 0xA9 and down, so we can recognize the pattern.
   for (iter = 0; iter < 10; iter++)
   {
      buf[iter] = ((uint8_t)(0xA9 + iter));
   }

   //Form a buffer with known reproducible contents.  We want to bloat the buffer
   //out so we can check the items beyond the nominal end.
   for (iter = 0; iter < 10000; iter++)
   {
      ts1.AppendOne(0xFE);
   }

   //Clear the buffer.  This should keep the allocation.
   ts1.Clear();

   //Repopulate the buffer.  We want enough allocation (from the previous loop)
   //so that we can poke around past the end, we want a known fill, and some known
   //correct values past the end.
   for (iter = 0; iter < 5000; iter++)
   {
      ts1.AppendOne((uint8_t)(iter + 31));
   }

   //Double-check that our actions were successful.
   ASSERT_EQ(ts1.m_n_used, 5000);
   ASSERT_EQ(ts1.m_errs, 0);
   ASSERT_TRUE(ts1.m_n_allocd >= 10000);  //Overallocated.
   ASSERT_TRUE(ts1.m_bufptr != nullptr);

   //Assign at the location.  The last byte should not be assigned.
   ts1.Assign(5000, buf, 10);

   //Double-check that values are as expected.
   ASSERT_EQ(ts1.m_n_used, 5000);
   ASSERT_EQ(ts1.m_errs, 0);
   ASSERT_TRUE(ts1.m_n_allocd >= 10000);
   ASSERT_TRUE(ts1.m_bufptr != nullptr);

   for (iter = 0; iter < 5000; iter++)
   {
      ASSERT_EQ(ts1.m_bufptr[iter], (uint8_t)(iter + 31));
   }
   for (; iter < 5010; iter++)
   {
      ASSERT_EQ(ts1.m_bufptr[iter], 0xFE);
      //std::cout << "Line " << __LINE__ << " reached, index: " << iter << ".\n";
   }
}


//Checks append one happy path.  For append one, at present we can
//only check one path.
TEST_F(LgBufUint8Test, AppendOneHappyPath)
{
   size_t iter;
   LgBufUint8 ts1;

   //Form a buffer with known reproducible contents.
   for (iter = 0; iter < 5000; iter++)
   {
      ts1.AppendOne((uint8_t)(iter + 29));
   }

   //Double-check that values are as expected.
   ASSERT_EQ(ts1.m_n_used, 5000);
   ASSERT_EQ(ts1.m_errs, 0);
   ASSERT_TRUE(ts1.m_n_allocd >= ts1.m_n_used);
   ASSERT_TRUE(ts1.m_bufptr != nullptr);
   for (iter = 0; iter < 5000; iter++)
   {
      ASSERT_EQ(ts1.m_bufptr[iter], (uint8_t)(iter+29));
   }
}


//Checks append happy path.
TEST_F(LgBufUint8Test, AppendHappyPath)
{
   size_t iter;
   LgBufUint8 ts1;
   uint8_t buf[25];

   //Fill up the test append buffer with known contents.
   for (iter = 0; iter < sizeof(buf); iter++)
   {
      buf[iter] = 0xA5 - (uint8_t)iter;
   }

   //Form a buffer with reproducible contents for the first 2500 entries.
   for (iter = 0; iter < 2500; iter++)
   {
      ts1.AppendOne((uint8_t)(iter + 31));
   }

   //Append the test append buffer.
   ts1.Append(buf, sizeof(buf));

   //Form a buffer with reproducible contents for another 5000.
   for (iter = 0; iter < 5000; iter++)
   {
      ts1.AppendOne((uint8_t)(iter + 31));
   }

   //Check the object control values.
   ASSERT_EQ(ts1.m_n_used, 2500 + sizeof(buf) + 5000);
   ASSERT_EQ(ts1.m_errs, 0);
   ASSERT_TRUE(ts1.m_n_allocd >= ts1.m_n_used);
   ASSERT_TRUE(ts1.m_bufptr != nullptr);

   //Check the first 2500 values, which should be known.
   for (iter = 0; iter < 2500; iter++)
   {
      ASSERT_EQ(ts1.m_bufptr[iter], (uint8_t)(iter + 31));
   }

   //Check the next values corresponding to the test buffer.
   for (iter = 2500; iter < (2500 + sizeof(buf)); iter++)
   {
      ASSERT_EQ(ts1.m_bufptr[iter], 0xA5 - (uint8_t)(iter - 2500));
   }

   //Check the final 5000 values, which should be known.
   for (iter = 2500 + sizeof(buf); iter < (2500 + sizeof(buf) + 5000); iter++)
   {
      ASSERT_EQ(ts1.m_bufptr[iter], (uint8_t)(iter - 2500 - sizeof(buf) + 31));
   }
}


//Checks insert one starting with empty object that has no history
//of allocation.  This test is not definitive because the memory might already,
//by chance, have the tested value.
TEST_F(LgBufUint8Test, InsertOneZeroLengthStartNoPrevAllocValidInsertPt)
{
   LgBufUint8 ts1;

   ts1.InsertOne(0, 0xA5);

   ASSERT_EQ(ts1.m_n_used, 1);
   ASSERT_EQ(ts1.m_errs, 0);
   ASSERT_TRUE(ts1.m_n_allocd >= ts1.m_n_used);
   ASSERT_TRUE(ts1.m_bufptr != nullptr);
   ASSERT_EQ(ts1.m_bufptr[0], 0xA5);
}


TEST_F(LgBufUint8Test, InsertOneZeroLengthStartNoPrevAllocInvalidInsertPt1)
{
   LgBufUint8 ts1;

   ts1.InsertOne(1, 0xD6);

   ASSERT_EQ(ts1.m_n_used, 1);
   ASSERT_EQ(ts1.m_errs, 0);
   ASSERT_TRUE(ts1.m_n_allocd >= ts1.m_n_used);
   ASSERT_TRUE(ts1.m_bufptr != nullptr);
   ASSERT_EQ(ts1.m_bufptr[0], 0xD6);
}


TEST_F(LgBufUint8Test, InsertOneZeroLengthStartNoPrevAllocInvalidInsertPt2)
{
   LgBufUint8 ts1;

   ts1.InsertOne(2, 0xE9);

   ASSERT_EQ(ts1.m_n_used, 1);
   ASSERT_EQ(ts1.m_errs, 0);
   ASSERT_TRUE(ts1.m_n_allocd >= ts1.m_n_used);
   ASSERT_TRUE(ts1.m_bufptr != nullptr);
   ASSERT_EQ(ts1.m_bufptr[0], 0xE9);
}


TEST_F(LgBufUint8Test, InsertOneOccupiedBufIndex0)
{
   size_t iter;
   LgBufUint8 ts1;

   //Form a buffer with reproducible contents for the first 2500 entries.
   for (iter = 0; iter < 2500; iter++)
   {
      ts1.AppendOne((uint8_t)(iter + 41));
   }

   ts1.InsertOne(0, 0x31);

   ASSERT_EQ(ts1.m_n_used, 2501);
   ASSERT_EQ(ts1.m_errs, 0);
   ASSERT_TRUE(ts1.m_n_allocd >= ts1.m_n_used);
   ASSERT_TRUE(ts1.m_bufptr != nullptr);
   ASSERT_EQ(ts1.m_bufptr[0], 0x31);
   for (iter = 1; iter < 2501; iter++)
   {
      ASSERT_EQ(ts1.m_bufptr[iter], (uint8_t)(iter - 1 + 41));
   }
}


TEST_F(LgBufUint8Test, InsertOneOccupiedBufIndex1)
{
   size_t iter;
   LgBufUint8 ts1;

   //Form a buffer with reproducible contents for the first 2500 entries.
   for (iter = 0; iter < 2500; iter++)
   {
      ts1.AppendOne((uint8_t)(iter + 41));
   }

   ts1.InsertOne(1, 0x31);

   ASSERT_EQ(ts1.m_n_used, 2501);
   ASSERT_EQ(ts1.m_errs, 0);
   ASSERT_TRUE(ts1.m_n_allocd >= ts1.m_n_used);
   ASSERT_TRUE(ts1.m_bufptr != nullptr);
   ASSERT_EQ(ts1.m_bufptr[0], 41);
   ASSERT_EQ(ts1.m_bufptr[1], 0x31);
   for (iter = 2; iter < 2501; iter++)
   {
      ASSERT_EQ(ts1.m_bufptr[iter], (uint8_t)(iter - 1 + 41));
   }
}


TEST_F(LgBufUint8Test, InsertOneOccupiedBufIndexLastMinus2)
{
   size_t iter;
   LgBufUint8 ts1;

   //Form a buffer with reproducible contents for the first 2500 entries.
   for (iter = 0; iter < 2500; iter++)
   {
      ts1.AppendOne((uint8_t)(iter + 43));
   }

   ts1.InsertOne(2498, 0xD3);

   ASSERT_EQ(ts1.m_n_used, 2501);
   ASSERT_EQ(ts1.m_errs, 0);
   ASSERT_TRUE(ts1.m_n_allocd >= ts1.m_n_used);
   ASSERT_TRUE(ts1.m_bufptr != nullptr);
   for (iter = 0; iter < 2498; iter++)
   {
      ASSERT_EQ(ts1.m_bufptr[iter], (uint8_t)(iter + 43));
   }

   ASSERT_EQ(ts1.m_bufptr[2498], 0xD3);
   ASSERT_EQ(ts1.m_bufptr[2499], (uint8_t)(2498+43));
   ASSERT_EQ(ts1.m_bufptr[2500], (uint8_t)(2499+43));
}


TEST_F(LgBufUint8Test, InsertOneOccupiedBufIndexLastMinus1)
{
   size_t iter;
   LgBufUint8 ts1;

   //Form a buffer with reproducible contents for the first 2500 entries.
   for (iter = 0; iter < 2500; iter++)
   {
      ts1.AppendOne((uint8_t)(iter + 47));
   }

   ts1.InsertOne(2499, 0xD4);

   ASSERT_EQ(ts1.m_n_used, 2501);
   ASSERT_EQ(ts1.m_errs, 0);
   ASSERT_TRUE(ts1.m_n_allocd >= ts1.m_n_used);
   ASSERT_TRUE(ts1.m_bufptr != nullptr);
   for (iter = 0; iter < 2499; iter++)
   {
      ASSERT_EQ(ts1.m_bufptr[iter], (uint8_t)(iter + 47));
   }

   ASSERT_EQ(ts1.m_bufptr[2499], 0xD4);
   ASSERT_EQ(ts1.m_bufptr[2500], (uint8_t)(2499 + 47));
}


TEST_F(LgBufUint8Test, InsertOneOccupiedBufIndexLast)
{
   size_t iter;
   LgBufUint8 ts1;

   //Form a buffer with reproducible contents for the first 2500 entries.
   for (iter = 0; iter < 2500; iter++)
   {
      ts1.AppendOne((uint8_t)(iter + 53));
   }

   ts1.InsertOne(2500, 0xD5);

   ASSERT_EQ(ts1.m_n_used, 2501);
   ASSERT_EQ(ts1.m_errs, 0);
   ASSERT_TRUE(ts1.m_n_allocd >= ts1.m_n_used);
   ASSERT_TRUE(ts1.m_bufptr != nullptr);
   for (iter = 0; iter < 2500; iter++)
   {
      ASSERT_EQ(ts1.m_bufptr[iter], (uint8_t)(iter + 53));
   }

   ASSERT_EQ(ts1.m_bufptr[2500], 0xD5);
}


TEST_F(LgBufUint8Test, InsertOneOccupiedBufIndexLastPlus1)
{
   size_t iter;
   LgBufUint8 ts1;

   //Form a buffer with reproducible contents for the first 2500 entries.
   for (iter = 0; iter < 2500; iter++)
   {
      ts1.AppendOne((uint8_t)(iter + 57));
   }

   ts1.InsertOne(2501, 0xD6);

   ASSERT_EQ(ts1.m_n_used, 2501);
   ASSERT_EQ(ts1.m_errs, 0);
   ASSERT_TRUE(ts1.m_n_allocd >= ts1.m_n_used);
   ASSERT_TRUE(ts1.m_bufptr != nullptr);
   for (iter = 0; iter < 2500; iter++)
   {
      ASSERT_EQ(ts1.m_bufptr[iter], (uint8_t)(iter + 57));
   }

   ASSERT_EQ(ts1.m_bufptr[2500], 0xD6);
}


TEST_F(LgBufUint8Test, InsertOneOccupiedBufIndexLastPlus2)
{
   size_t iter;
   LgBufUint8 ts1;

   //Form a buffer with reproducible contents for the first 2500 entries.
   for (iter = 0; iter < 2500; iter++)
   {
      ts1.AppendOne((uint8_t)(iter + 59));
   }

   ts1.InsertOne(2502, 0xD7);

   ASSERT_EQ(ts1.m_n_used, 2501);
   ASSERT_EQ(ts1.m_errs, 0);
   ASSERT_TRUE(ts1.m_n_allocd >= ts1.m_n_used);
   ASSERT_TRUE(ts1.m_bufptr != nullptr);
   for (iter = 0; iter < 2500; iter++)
   {
      ASSERT_EQ(ts1.m_bufptr[iter], (uint8_t)(iter + 59));
   }

   ASSERT_EQ(ts1.m_bufptr[2500], 0xD7);
}


//Insert at beginning, null pointer, 1 item.
TEST_F(LgBufUint8Test, InsertLoc0NullPtrQty1)
{
   size_t iter;
   LgBufUint8 ts1;

   //Form a buffer with reproducible contents for the first 5000 entries.
   for (iter = 0; iter < 5000; iter++)
   {
      ts1.AppendOne((uint8_t)(iter + 61));
   }

   //Insert 1 at beginning.  Because of of null pointer, should be 0.
   ts1.Insert(0, nullptr, 1);

   //Verify.
   ASSERT_EQ(ts1.m_n_used, 5001);
   ASSERT_EQ(ts1.m_errs, 0);
   ASSERT_TRUE(ts1.m_n_allocd >= ts1.m_n_used);
   ASSERT_TRUE(ts1.m_bufptr != nullptr);
   ASSERT_EQ(ts1.m_bufptr[0], 0);
   for (iter = 1; iter < 5001; iter++)
   {
      ASSERT_EQ(ts1.m_bufptr[iter], (uint8_t)(iter + 60));
   }
}


//Insert at beginning, null pointer, 10 items.
TEST_F(LgBufUint8Test, InsertLoc0NullPtrQty10)
{
   size_t iter;
   LgBufUint8 ts1;

   //Form a buffer with reproducible contents for the first 5000 entries.
   for (iter = 0; iter < 5000; iter++)
   {
      ts1.AppendOne((uint8_t)(iter + 67));
   }

   //Insert 1 at beginning.  Because of of null pointer, should be 0.
   ts1.Insert(0, nullptr, 10);

   //Verify.
   ASSERT_EQ(ts1.m_n_used, 5010);
   ASSERT_EQ(ts1.m_errs, 0);
   ASSERT_TRUE(ts1.m_n_allocd >= ts1.m_n_used);
   ASSERT_TRUE(ts1.m_bufptr != nullptr);
   for (iter = 0; iter < 10; iter++)
   {
      ASSERT_EQ(ts1.m_bufptr[iter], 0);
   }
   for (iter = 10; iter < 5010; iter++)
   {
      ASSERT_EQ(ts1.m_bufptr[iter], (uint8_t)(iter + 57));
   }
}


//Insert at location 1, null pointer, 1 item.
TEST_F(LgBufUint8Test, InsertLoc1NullPtrQty1)
{
   size_t iter;
   LgBufUint8 ts1;

   //Form a buffer with reproducible contents for the first 5000 entries.
   for (iter = 0; iter < 5000; iter++)
   {
      ts1.AppendOne((uint8_t)(iter + 67));
   }

   //Insert 1 at location 1.  Because of of null pointer, should be 0.
   ts1.Insert(1, nullptr, 1);

   //Verify.
   ASSERT_EQ(ts1.m_n_used, 5001);
   ASSERT_EQ(ts1.m_errs, 0);
   ASSERT_TRUE(ts1.m_n_allocd >= ts1.m_n_used);
   ASSERT_TRUE(ts1.m_bufptr != nullptr);
   ASSERT_EQ(ts1.m_bufptr[0], 67);
   ASSERT_EQ(ts1.m_bufptr[1], 0);
   for (iter = 2; iter < 5001; iter++)
   {
      ASSERT_EQ(ts1.m_bufptr[iter], (uint8_t)(iter + 66));
   }
}


//Insert at location 1, null pointer, 10 items.
TEST_F(LgBufUint8Test, InsertLoc1NullPtrQty10)
{
   size_t iter;
   LgBufUint8 ts1;

   //Form a buffer with reproducible contents for the first 5000 entries.
   for (iter = 0; iter < 5000; iter++)
   {
      ts1.AppendOne((uint8_t)(iter + 71));
   }

   //Insert 1 at beginning.  Because of of null pointer, should be 0.
   ts1.Insert(1, nullptr, 10);

   //Verify.
   ASSERT_EQ(ts1.m_n_used, 5010);
   ASSERT_EQ(ts1.m_errs, 0);
   ASSERT_TRUE(ts1.m_n_allocd >= ts1.m_n_used);
   ASSERT_TRUE(ts1.m_bufptr != nullptr);
   ASSERT_EQ(ts1.m_bufptr[0], 71);
   for (iter = 1; iter < 11; iter++)
   {
      ASSERT_EQ(ts1.m_bufptr[iter], 0);
   }
   for (iter = 11; iter < 5010; iter++)
   {
      ASSERT_EQ(ts1.m_bufptr[iter], (uint8_t)(iter + 61));
   }
}


//Insert at location 1000, null pointer, 1 item.
TEST_F(LgBufUint8Test, InsertLoc1000NullPtrQty1)
{
   size_t iter;
   LgBufUint8 ts1;

   //Form a buffer with reproducible contents for the first 5000 entries.
   for (iter = 0; iter < 5000; iter++)
   {
      ts1.AppendOne((uint8_t)(iter + 3));
   }

   //Insert 1 at location 1000.  Because of of null pointer, should be 0.
   ts1.Insert(1000, nullptr, 1);

   //Verify.
   ASSERT_EQ(ts1.m_n_used, 5001);
   ASSERT_EQ(ts1.m_errs, 0);
   ASSERT_TRUE(ts1.m_n_allocd >= ts1.m_n_used);
   ASSERT_TRUE(ts1.m_bufptr != nullptr);
   for (iter = 0; iter < 1000; iter++)
   {
      ASSERT_EQ(ts1.m_bufptr[iter], (uint8_t)(iter + 3));
   }
   ASSERT_EQ(ts1.m_bufptr[1000], 0);
   for (iter = 1001; iter < 5001; iter++)
   {
      ASSERT_EQ(ts1.m_bufptr[iter], (uint8_t)(iter + 2));
   }
}


//Insert at location 1000, null pointer, 10 items.
TEST_F(LgBufUint8Test, InsertLoc1000NullPtrQty10)
{
   size_t iter;
   LgBufUint8 ts1;

   //Form a buffer with reproducible contents for the first 5000 entries.
   for (iter = 0; iter < 5000; iter++)
   {
      ts1.AppendOne((uint8_t)(iter + 5));
   }

   //Insert 10 at location 1000.  Because of of null pointer, should be 0.
   ts1.Insert(1000, nullptr, 10);

   //Verify.
   ASSERT_EQ(ts1.m_n_used, 5010);
   ASSERT_EQ(ts1.m_errs, 0);
   ASSERT_TRUE(ts1.m_n_allocd >= ts1.m_n_used);
   ASSERT_TRUE(ts1.m_bufptr != nullptr);
   for (iter = 0; iter < 1000; iter++)
   {
      ASSERT_EQ(ts1.m_bufptr[iter], (uint8_t)(iter + 5));
   }
   for (iter = 1000; iter < 1010; iter++)
   {
      ASSERT_EQ(ts1.m_bufptr[iter], 0);
   }
   for (iter = 1010; iter < 5010; iter++)
   {
      ASSERT_EQ(ts1.m_bufptr[iter], (uint8_t)(iter + 256 + 5 - 10));
   }
}


//Insert at 1 before end, null pointer, 1 item.
TEST_F(LgBufUint8Test, InsertLocObeNullPtrQty1)
{
   size_t iter;
   LgBufUint8 ts1;

   //Form a buffer with reproducible contents for the first 5000 entries.
   for (iter = 0; iter < 5000; iter++)
   {
      ts1.AppendOne((uint8_t)(iter + 7));
   }

   //Insert 1 at location 4999.  Because of of null pointer, should be 0.
   ts1.Insert(4999, nullptr, 1);

   //Verify.
   ASSERT_EQ(ts1.m_n_used, 5001);
   ASSERT_EQ(ts1.m_errs, 0);
   ASSERT_TRUE(ts1.m_n_allocd >= ts1.m_n_used);
   ASSERT_TRUE(ts1.m_bufptr != nullptr);
   for (iter = 0; iter < 4999; iter++)
   {
      ASSERT_EQ(ts1.m_bufptr[iter], (uint8_t)(iter + 7));
   }
   ASSERT_EQ(ts1.m_bufptr[4999], 0);
   ASSERT_EQ(ts1.m_bufptr[5000], (uint8_t)(5000 + 6));
}


//Insert at location 1 before end, null pointer, 10 items.
TEST_F(LgBufUint8Test, InsertLocObeNullPtrQty10)
{
   size_t iter;
   LgBufUint8 ts1;

   //Form a buffer with reproducible contents for the first 5000 entries.
   for (iter = 0; iter < 5000; iter++)
   {
      ts1.AppendOne((uint8_t)(iter + 11));
   }

   //Insert 10 at location 1000.  Because of of null pointer, should be 0.
   ts1.Insert(4999, nullptr, 10);

   //Verify.
   ASSERT_EQ(ts1.m_n_used, 5010);
   ASSERT_EQ(ts1.m_errs, 0);
   ASSERT_TRUE(ts1.m_n_allocd >= ts1.m_n_used);
   ASSERT_TRUE(ts1.m_bufptr != nullptr);
   for (iter = 0; iter < 4999; iter++)
   {
      ASSERT_EQ(ts1.m_bufptr[iter], (uint8_t)(iter + 11));
   }
   for (iter = 4999; iter < 5009; iter++)
   {
      ASSERT_EQ(ts1.m_bufptr[iter], 0);
   }
   for (iter = 5009; iter < 5010; iter++)
   {
      ASSERT_EQ(ts1.m_bufptr[iter], (uint8_t)(iter + 256 + 11 - 10));
   }
}


//Insert at end, null pointer, 1 item.
TEST_F(LgBufUint8Test, InsertLocENullPtrQty1)
{
   size_t iter;
   LgBufUint8 ts1;

   //Form a buffer with reproducible contents for the first 5000 entries.
   for (iter = 0; iter < 5000; iter++)
   {
      ts1.AppendOne((uint8_t)(iter + 11));
   }

   //Insert 1 at location 5000.  Because of of null pointer, should be 0.
   ts1.Insert(5000, nullptr, 1);

   //Verify.
   ASSERT_EQ(ts1.m_n_used, 5001);
   ASSERT_EQ(ts1.m_errs, 0);
   ASSERT_TRUE(ts1.m_n_allocd >= ts1.m_n_used);
   ASSERT_TRUE(ts1.m_bufptr != nullptr);
   for (iter = 0; iter < 5000; iter++)
   {
      ASSERT_EQ(ts1.m_bufptr[iter], (uint8_t)(iter + 11));
   }
   ASSERT_EQ(ts1.m_bufptr[5000], 0);
}


//Insert at end, null pointer, 10 items.
TEST_F(LgBufUint8Test, InsertLocENullPtrQty10)
{
   size_t iter;
   LgBufUint8 ts1;

   //Form a buffer with reproducible contents for the first 5000 entries.
   for (iter = 0; iter < 5000; iter++)
   {
      ts1.AppendOne((uint8_t)(iter + 17));
   }

   //Insert 10 at location 1000.  Because of of null pointer, should be 0.
   ts1.Insert(5000, nullptr, 10);

   //Verify.
   ASSERT_EQ(ts1.m_n_used, 5010);
   ASSERT_EQ(ts1.m_errs, 0);
   ASSERT_TRUE(ts1.m_n_allocd >= ts1.m_n_used);
   ASSERT_TRUE(ts1.m_bufptr != nullptr);
   for (iter = 0; iter < 5000; iter++)
   {
      ASSERT_EQ(ts1.m_bufptr[iter], (uint8_t)(iter + 17));
   }
   for (iter = 5000; iter < 5010; iter++)
   {
      ASSERT_EQ(ts1.m_bufptr[iter], 0);
   }
}


//Insert at end plus 1, null pointer, 1 item.
TEST_F(LgBufUint8Test, InsertLocEp1NullPtrQty1)
{
   size_t iter;
   LgBufUint8 ts1;

   //Form a buffer with reproducible contents for the first 5000 entries.
   for (iter = 0; iter < 5000; iter++)
   {
      ts1.AppendOne((uint8_t)(iter + 19));
   }

   //Insert 1 at location 5001.  Because of of null pointer, should be 0.
   ts1.Insert(5001, nullptr, 1);

   //Verify.
   ASSERT_EQ(ts1.m_n_used, 5001);
   ASSERT_EQ(ts1.m_errs, 0);
   ASSERT_TRUE(ts1.m_n_allocd >= ts1.m_n_used);
   ASSERT_TRUE(ts1.m_bufptr != nullptr);
   for (iter = 0; iter < 5000; iter++)
   {
      ASSERT_EQ(ts1.m_bufptr[iter], (uint8_t)(iter + 19));
   }
   ASSERT_EQ(ts1.m_bufptr[5000], 0);
}


//Insert at end plus 1, null pointer, 10 items.
TEST_F(LgBufUint8Test, InsertLocEp1NullPtrQty10)
{
   size_t iter;
   LgBufUint8 ts1;

   //Form a buffer with reproducible contents for the first 5000 entries.
   for (iter = 0; iter < 5000; iter++)
   {
      ts1.AppendOne((uint8_t)(iter + 23));
   }

   //Insert 10 at location 5001.  Because of of null pointer, should be 0.
   ts1.Insert(5001, nullptr, 10);

   //Verify.
   ASSERT_EQ(ts1.m_n_used, 5010);
   ASSERT_EQ(ts1.m_errs, 0);
   ASSERT_TRUE(ts1.m_n_allocd >= ts1.m_n_used);
   ASSERT_TRUE(ts1.m_bufptr != nullptr);
   for (iter = 0; iter < 5000; iter++)
   {
      ASSERT_EQ(ts1.m_bufptr[iter], (uint8_t)(iter + 23));
   }
   for (iter = 5000; iter < 5010; iter++)
   {
      ASSERT_EQ(ts1.m_bufptr[iter], 0);
   }
}


//Insert at end plus 1000, null pointer, 1 item.
TEST_F(LgBufUint8Test, InsertLocEp1000NullPtrQty1)
{
   size_t iter;
   LgBufUint8 ts1;

   //Form a buffer with reproducible contents for the first 5000 entries.
   for (iter = 0; iter < 5000; iter++)
   {
      ts1.AppendOne((uint8_t)(iter + 29));
   }

   //Insert 1 at location 6000.  Because of of null pointer, should be 0.
   ts1.Insert(6000, nullptr, 1);

   //Verify.
   ASSERT_EQ(ts1.m_n_used, 5001);
   ASSERT_EQ(ts1.m_errs, 0);
   ASSERT_TRUE(ts1.m_n_allocd >= ts1.m_n_used);
   ASSERT_TRUE(ts1.m_bufptr != nullptr);
   for (iter = 0; iter < 5000; iter++)
   {
      ASSERT_EQ(ts1.m_bufptr[iter], (uint8_t)(iter + 29));
   }
   ASSERT_EQ(ts1.m_bufptr[5000], 0);
}


//Insert at end plus 1000, null pointer, 10 items.
TEST_F(LgBufUint8Test, InsertLocEp1000NullPtrQty10)
{
   size_t iter;
   LgBufUint8 ts1;

   //Form a buffer with reproducible contents for the first 5000 entries.
   for (iter = 0; iter < 5000; iter++)
   {
      ts1.AppendOne((uint8_t)(iter + 31));
   }

   //Insert 10 at location 6000.  Because of of null pointer, should be 0.
   ts1.Insert(6000, nullptr, 10);

   //Verify.
   ASSERT_EQ(ts1.m_n_used, 5010);
   ASSERT_EQ(ts1.m_errs, 0);
   ASSERT_TRUE(ts1.m_n_allocd >= ts1.m_n_used);
   ASSERT_TRUE(ts1.m_bufptr != nullptr);
   for (iter = 0; iter < 5000; iter++)
   {
      ASSERT_EQ(ts1.m_bufptr[iter], (uint8_t)(iter + 31));
   }
   for (iter = 5000; iter < 5010; iter++)
   {
      ASSERT_EQ(ts1.m_bufptr[iter], 0);
   }
}


//Insert at beginning, non-null pointer, 1 item.
TEST_F(LgBufUint8Test, InsertLoc0NonNullPtrQty1)
{
   size_t iter;
   LgBufUint8 ts1;
   uint8_t insert_item = 0xD5;

   //Form a buffer with reproducible contents for the first 5000 entries.
   for (iter = 0; iter < 5000; iter++)
   {
      ts1.AppendOne((uint8_t)(iter + 61));
   }

   //Insert 1 at beginning.
   ts1.Insert(0, &insert_item, 1);

   //Verify.
   ASSERT_EQ(ts1.m_n_used, 5001);
   ASSERT_EQ(ts1.m_errs, 0);
   ASSERT_TRUE(ts1.m_n_allocd >= ts1.m_n_used);
   ASSERT_TRUE(ts1.m_bufptr != nullptr);
   ASSERT_EQ(ts1.m_bufptr[0], 0xD5);
   for (iter = 1; iter < 5001; iter++)
   {
      ASSERT_EQ(ts1.m_bufptr[iter], (uint8_t)(iter + 60));
   }
}


//Insert at beginning, non-null pointer, 10 items.
TEST_F(LgBufUint8Test, InsertLoc0NonNullPtrQty10)
{
   size_t iter;
   LgBufUint8 ts1;
   uint8_t buf[10];

   //Populate the buffer with contents from 0xD0 on down.
   for (iter = 0; iter < 10; iter++)
   {
      buf[iter] = (uint8_t)(0xD0 - iter);
   }

   //Form a buffer with reproducible contents for the first 5000 entries.
   for (iter = 0; iter < 5000; iter++)
   {
      ts1.AppendOne((uint8_t)(iter + 29));
   }

   //Insert 10 at beginning.
   ts1.Insert(0, buf, 10);

   //Verify.
   ASSERT_EQ(ts1.m_n_used, 5010);
   ASSERT_EQ(ts1.m_errs, 0);
   ASSERT_TRUE(ts1.m_n_allocd >= ts1.m_n_used);
   ASSERT_TRUE(ts1.m_bufptr != nullptr);
   for (iter = 0; iter < 10; iter++)
   {
      ASSERT_EQ(ts1.m_bufptr[iter], (uint8_t)(0xD0-iter));
   }
   for (iter = 10; iter < 5010; iter++)
   {
      ASSERT_EQ(ts1.m_bufptr[iter], (uint8_t)(iter + 19));
   }
}


//Insert at location 1, non-null pointer, 1 item.
TEST_F(LgBufUint8Test, InsertLoc1NonNullPtrQty1)
{
   size_t iter;
   LgBufUint8 ts1;
   uint8_t insert_item = 0xD4;

   //Form a buffer with reproducible contents for the first 5000 entries.
   for (iter = 0; iter < 5000; iter++)
   {
      ts1.AppendOne((uint8_t)(iter + 37));
   }

   //Insert one at location 1.
   ts1.Insert(1, &insert_item, 1);

   //Verify.
   ASSERT_EQ(ts1.m_n_used, 5001);
   ASSERT_EQ(ts1.m_errs, 0);
   ASSERT_TRUE(ts1.m_n_allocd >= ts1.m_n_used);
   ASSERT_TRUE(ts1.m_bufptr != nullptr);
   ASSERT_EQ(ts1.m_bufptr[0], 37);
   ASSERT_EQ(ts1.m_bufptr[1], 0xD4);
   for (iter = 2; iter < 5001; iter++)
   {
      ASSERT_EQ(ts1.m_bufptr[iter], (uint8_t)(iter + 36));
   }
}


//Insert at location 1, non-null pointer, 10 items.
TEST_F(LgBufUint8Test, InsertLoc1NonNullPtrQty10)
{
   size_t iter;
   LgBufUint8 ts1;
   uint8_t buf[10];

   //Populate the buffer with contents from 0xC5 on down.
   for (iter = 0; iter < 10; iter++)
   {
      buf[iter] = (uint8_t)(0xC5 - iter);
   }

   //Form a buffer with reproducible contents for the first 5000 entries.
   for (iter = 0; iter < 5000; iter++)
   {
      ts1.AppendOne((uint8_t)(iter + 43));
   }

   //Insert 10 at location 1.
   ts1.Insert(1, buf, 10);

   //Verify.
   ASSERT_EQ(ts1.m_n_used, 5010);
   ASSERT_EQ(ts1.m_errs, 0);
   ASSERT_TRUE(ts1.m_n_allocd >= ts1.m_n_used);
   ASSERT_TRUE(ts1.m_bufptr != nullptr);
   ASSERT_EQ(ts1.m_bufptr[0], 43);
   for (iter = 1; iter < 11; iter++)
   {
      ASSERT_EQ(ts1.m_bufptr[iter], (uint8_t)(0xC5 - iter + 1));
   }
   for (iter = 11; iter < 5010; iter++)
   {
      ASSERT_EQ(ts1.m_bufptr[iter], (uint8_t)(iter + 33));
   }
}


//Insert at location 1000, non-null pointer, 1 item.
TEST_F(LgBufUint8Test, InsertLoc1000NonNullPtrQty1)
{
   size_t iter;
   LgBufUint8 ts1;
   uint8_t insert_item = 0xD3;

   //Form a buffer with reproducible contents for the first 5000 entries.
   for (iter = 0; iter < 5000; iter++)
   {
      ts1.AppendOne((uint8_t)(iter + 83));
   }

   //Insert 1 at location 1000.
   ts1.Insert(1000, &insert_item, 1);

   //Verify.
   ASSERT_EQ(ts1.m_n_used, 5001);
   ASSERT_EQ(ts1.m_errs, 0);
   ASSERT_TRUE(ts1.m_n_allocd >= ts1.m_n_used);
   ASSERT_TRUE(ts1.m_bufptr != nullptr);
   for (iter = 0; iter < 1000; iter++)
   {
      ASSERT_EQ(ts1.m_bufptr[iter], (uint8_t)(iter + 83));
   }
   ASSERT_EQ(ts1.m_bufptr[1000], 0xD3);
   for (iter = 1001; iter < 5001; iter++)
   {
      ASSERT_EQ(ts1.m_bufptr[iter], (uint8_t)(iter + 82));
   }
}


//Insert at location 1000, non-null pointer, 10 items.
TEST_F(LgBufUint8Test, InsertLoc1000NonNullPtrQty10)
{
   size_t iter;
   LgBufUint8 ts1;
   uint8_t buf[10];

   //Populate the buffer with contents from 0xBF on down.
   for (iter = 0; iter < 10; iter++)
   {
      buf[iter] = (uint8_t)(0xBF - iter);
   }

   //Form a buffer with reproducible contents for the first 5000 entries.
   for (iter = 0; iter < 5000; iter++)
   {
      ts1.AppendOne((uint8_t)(iter + 73));
   }

   //Insert 10 at location 1000.
   ts1.Insert(1000, buf, 10);

   //Verify.
   ASSERT_EQ(ts1.m_n_used, 5010);
   ASSERT_EQ(ts1.m_errs, 0);
   ASSERT_TRUE(ts1.m_n_allocd >= ts1.m_n_used);
   ASSERT_TRUE(ts1.m_bufptr != nullptr);
   for (iter = 0; iter < 1000; iter++)
   {
      ASSERT_EQ(ts1.m_bufptr[iter], (uint8_t)(iter + 73));
   }
   for (iter = 1000; iter < 1010; iter++)
   {
      ASSERT_EQ(ts1.m_bufptr[iter], (uint8_t)(0xBF + 1000 - iter));
   }
   for (iter = 1010; iter < 5010; iter++)
   {
      ASSERT_EQ(ts1.m_bufptr[iter], (uint8_t)(iter + 73 - 10));
   }
}


//Insert at 1 before end, non-null pointer, 1 item.
TEST_F(LgBufUint8Test, InsertLocObeNonNullPtrQty1)
{
   size_t iter;
   LgBufUint8 ts1;
   uint8_t insert_item = 0xD2;

   //Form a buffer with reproducible contents for the first 5000 entries.
   for (iter = 0; iter < 5000; iter++)
   {
      ts1.AppendOne((uint8_t)(iter + 97));
   }

   //Insert 1 at location 4999.
   ts1.Insert(4999, &insert_item, 1);

   //Verify.
   ASSERT_EQ(ts1.m_n_used, 5001);
   ASSERT_EQ(ts1.m_errs, 0);
   ASSERT_TRUE(ts1.m_n_allocd >= ts1.m_n_used);
   ASSERT_TRUE(ts1.m_bufptr != nullptr);
   for (iter = 0; iter < 4999; iter++)
   {
      ASSERT_EQ(ts1.m_bufptr[iter], (uint8_t)(iter + 97));
   }
   ASSERT_EQ(ts1.m_bufptr[4999], 0xD2);
   ASSERT_EQ(ts1.m_bufptr[5000], (uint8_t)(5000 + 96));
}


//Insert at location 1 before end, non-null pointer, 10 items.
TEST_F(LgBufUint8Test, InsertLocObeNonNullPtrQty10)
{
   size_t iter;
   LgBufUint8 ts1;
   uint8_t buf[10];

   //Populate the buffer with contents from 0xBF on down.
   for (iter = 0; iter < 10; iter++)
   {
      buf[iter] = (uint8_t)(0xAC - iter);
   }

   //Form a buffer with reproducible contents for the first 5000 entries.
   for (iter = 0; iter < 5000; iter++)
   {
      ts1.AppendOne((uint8_t)(iter + 11));
   }

   //Insert 10 at location 1000.
   ts1.Insert(4999, buf, 10);

   //Verify.
   ASSERT_EQ(ts1.m_n_used, 5010);
   ASSERT_EQ(ts1.m_errs, 0);
   ASSERT_TRUE(ts1.m_n_allocd >= ts1.m_n_used);
   ASSERT_TRUE(ts1.m_bufptr != nullptr);
   for (iter = 0; iter < 4999; iter++)
   {
      ASSERT_EQ(ts1.m_bufptr[iter], (uint8_t)(iter + 11));
   }
   for (iter = 4999; iter < 5009; iter++)
   {
      ASSERT_EQ(ts1.m_bufptr[iter], (uint8_t)(0xAC + 4999 - iter));
   }
   for (iter = 5009; iter < 5010; iter++)
   {
      ASSERT_EQ(ts1.m_bufptr[iter], (uint8_t)(iter + 256 + 11 - 10));
   }
}


//Insert at end, non-null pointer, 1 item.
TEST_F(LgBufUint8Test, InsertLocENonNullPtrQty1)
{
   size_t iter;
   LgBufUint8 ts1;
   uint8_t insert_item = 0xD1;

   //Form a buffer with reproducible contents for the first 5000 entries.
   for (iter = 0; iter < 5000; iter++)
   {
      ts1.AppendOne((uint8_t)(iter + 59));
   }

   //Insert 1 at location 5000.
   ts1.Insert(5000, &insert_item, 1);

   //Verify.
   ASSERT_EQ(ts1.m_n_used, 5001);
   ASSERT_EQ(ts1.m_errs, 0);
   ASSERT_TRUE(ts1.m_n_allocd >= ts1.m_n_used);
   ASSERT_TRUE(ts1.m_bufptr != nullptr);
   for (iter = 0; iter < 5000; iter++)
   {
      ASSERT_EQ(ts1.m_bufptr[iter], (uint8_t)(iter + 59));
   }
   ASSERT_EQ(ts1.m_bufptr[5000], 0xD1);
}


//Insert at end, non-null pointer, 10 items.
TEST_F(LgBufUint8Test, InsertLocENonNullPtrQty10)
{
   size_t iter;
   LgBufUint8 ts1;
   uint8_t buf[10];

   //Populate the buffer with contents from 0x37 on down.
   for (iter = 0; iter < 10; iter++)
   {
      buf[iter] = (uint8_t)(0x37 - iter);
   }

   //Form a buffer with reproducible contents for the first 5000 entries.
   for (iter = 0; iter < 5000; iter++)
   {
      ts1.AppendOne((uint8_t)(iter + 19));
   }

   //Insert 10 at location 1000.  Because of of null pointer, should be 0.
   ts1.Insert(5000, buf, 10);

   //Verify.
   ASSERT_EQ(ts1.m_n_used, 5010);
   ASSERT_EQ(ts1.m_errs, 0);
   ASSERT_TRUE(ts1.m_n_allocd >= ts1.m_n_used);
   ASSERT_TRUE(ts1.m_bufptr != nullptr);
   for (iter = 0; iter < 5000; iter++)
   {
      ASSERT_EQ(ts1.m_bufptr[iter], (uint8_t)(iter + 19));
   }
   for (iter = 5000; iter < 5010; iter++)
   {
      ASSERT_EQ(ts1.m_bufptr[iter], (uint8_t)(0x37 - (iter - 5000)));
   }
}


//Insert at end plus 1, non-null pointer, 1 item.
TEST_F(LgBufUint8Test, InsertLocEp1NonNullPtrQty1)
{
   size_t iter;
   LgBufUint8 ts1;
   uint8_t insert_item = 0xCC;

   //Form a buffer with reproducible contents for the first 5000 entries.
   for (iter = 0; iter < 5000; iter++)
   {
      ts1.AppendOne((uint8_t)(iter + 23));
   }

   //Insert 1 at location 5000.
   ts1.Insert(5001, &insert_item, 1);

   //Verify.
   ASSERT_EQ(ts1.m_n_used, 5001);
   ASSERT_EQ(ts1.m_errs, 0);
   ASSERT_TRUE(ts1.m_n_allocd >= ts1.m_n_used);
   ASSERT_TRUE(ts1.m_bufptr != nullptr);
   for (iter = 0; iter < 5000; iter++)
   {
      ASSERT_EQ(ts1.m_bufptr[iter], (uint8_t)(iter + 23));
   }
   ASSERT_EQ(ts1.m_bufptr[5000], 0xCC);
}


//Insert at end plus 1, non-null pointer, 10 items.
TEST_F(LgBufUint8Test, InsertLocEp1NonNullPtrQty10)
{
   size_t iter;
   LgBufUint8 ts1;
   uint8_t buf[10];

   //Populate the buffer with contents from 0x37 on down.
   for (iter = 0; iter < 10; iter++)
   {
      buf[iter] = (uint8_t)(0x23 - iter);
   }

   //Form a buffer with reproducible contents for the first 5000 entries.
   for (iter = 0; iter < 5000; iter++)
   {
      ts1.AppendOne((uint8_t)(iter + 23));
   }

   //Insert 10 at location 1000.
   ts1.Insert(5001, buf, 10);

   //Verify.
   ASSERT_EQ(ts1.m_n_used, 5010);
   ASSERT_EQ(ts1.m_errs, 0);
   ASSERT_TRUE(ts1.m_n_allocd >= ts1.m_n_used);
   ASSERT_TRUE(ts1.m_bufptr != nullptr);
   for (iter = 0; iter < 5000; iter++)
   {
      ASSERT_EQ(ts1.m_bufptr[iter], (uint8_t)(iter + 23));
   }
   for (iter = 5000; iter < 5010; iter++)
   {
      ASSERT_EQ(ts1.m_bufptr[iter], (uint8_t)(0x23 - (iter - 5000)));
   }
}


//Insert at end plus 1000, non-null pointer, 1 item.
TEST_F(LgBufUint8Test, InsertLocEp1000NonNullPtrQty1)
{
   size_t iter;
   LgBufUint8 ts1;
   uint8_t insert_item = 0xC1;

   //Form a buffer with reproducible contents for the first 5000 entries.
   for (iter = 0; iter < 5000; iter++)
   {
      ts1.AppendOne((uint8_t)(iter + 29));
   }

   //Insert 1 at location 5000.
   ts1.Insert(6000, &insert_item, 1);

   //Verify.
   ASSERT_EQ(ts1.m_n_used, 5001);
   ASSERT_EQ(ts1.m_errs, 0);
   ASSERT_TRUE(ts1.m_n_allocd >= ts1.m_n_used);
   ASSERT_TRUE(ts1.m_bufptr != nullptr);
   for (iter = 0; iter < 5000; iter++)
   {
      ASSERT_EQ(ts1.m_bufptr[iter], (uint8_t)(iter + 29));
   }
   ASSERT_EQ(ts1.m_bufptr[5000], 0xC1);
}


//Insert at end plus 1000, non-null pointer, 10 items.
TEST_F(LgBufUint8Test, InsertLocEp1000NonNullPtrQty10)
{
   size_t iter;
   LgBufUint8 ts1;
   uint8_t buf[10];

   //Populate the buffer with contents from 0x37 on down.
   for (iter = 0; iter < 10; iter++)
   {
      buf[iter] = (uint8_t)(0xC6 - iter);
   }

   //Form a buffer with reproducible contents for the first 5000 entries.
   for (iter = 0; iter < 5000; iter++)
   {
      ts1.AppendOne((uint8_t)(iter + 37));
   }

   //Insert 10 at location 1000.
   ts1.Insert(6000, buf, 10);

   //Verify.
   ASSERT_EQ(ts1.m_n_used, 5010);
   ASSERT_EQ(ts1.m_errs, 0);
   ASSERT_TRUE(ts1.m_n_allocd >= ts1.m_n_used);
   ASSERT_TRUE(ts1.m_bufptr != nullptr);
   for (iter = 0; iter < 5000; iter++)
   {
      ASSERT_EQ(ts1.m_bufptr[iter], (uint8_t)(iter + 37));
   }
   for (iter = 5000; iter < 5010; iter++)
   {
      ASSERT_EQ(ts1.m_bufptr[iter], (uint8_t)(0xC6 - (iter - 5000)));
   }
}


//Delete one, fresh constructed object, delete point 0.
TEST_F(LgBufUint8Test, DeleteOneFreshConstructedDp0)
{
   LgBufUint8 ts1;

   ts1.DeleteOne(0);

   ASSERT_EQ(ts1.m_n_used, 0);
   ASSERT_EQ(ts1.m_errs, 0);
   ASSERT_EQ(ts1.m_n_allocd, 0);
   ASSERT_EQ(ts1.m_bufptr, nullptr);
}


//Delete one, fresh constructed object, delete point 1.
TEST_F(LgBufUint8Test, DeleteOneFreshConstructedDp1)
{
   LgBufUint8 ts1;

   ts1.DeleteOne(1);

   ASSERT_EQ(ts1.m_n_used, 0);
   ASSERT_EQ(ts1.m_errs, 0);
   ASSERT_EQ(ts1.m_n_allocd, 0);
   ASSERT_EQ(ts1.m_bufptr, nullptr);
}


//Delete one, fresh constructed object, delete point 2.
TEST_F(LgBufUint8Test, DeleteOneFreshConstructedDp2)
{
   LgBufUint8 ts1;

   ts1.DeleteOne(2);

   ASSERT_EQ(ts1.m_n_used, 0);
   ASSERT_EQ(ts1.m_errs, 0);
   ASSERT_EQ(ts1.m_n_allocd, 0);
   ASSERT_EQ(ts1.m_bufptr, nullptr);
}

//Delete one, fresh constructed object, delete point 3.
TEST_F(LgBufUint8Test, DeleteOneFreshConstructedDp3)
{
   LgBufUint8 ts1;

   ts1.DeleteOne(3);

   ASSERT_EQ(ts1.m_n_used, 0);
   ASSERT_EQ(ts1.m_errs, 0);
   ASSERT_EQ(ts1.m_n_allocd, 0);
   ASSERT_EQ(ts1.m_bufptr, nullptr);
}


//Delete one, object with 1 element, delete point 0.
TEST_F(LgBufUint8Test, DeleteOne1ElDp0)
{
   LgBufUint8 ts1;

   ts1.AppendOne(0xD3);

   ts1.DeleteOne(0);

   ASSERT_EQ(ts1.m_n_used, 0);
   ASSERT_EQ(ts1.m_errs, 0);
   ASSERT_TRUE(ts1.m_n_allocd > 0);
   ASSERT_TRUE(ts1.m_bufptr != nullptr);
}


//Delete one, object with 1 element, delete point 1.
TEST_F(LgBufUint8Test, DeleteOne1ElDp1)
{
   LgBufUint8 ts1;

   ts1.AppendOne(0xD3);

   ts1.DeleteOne(1);

   ASSERT_EQ(ts1.m_n_used, 1);
   ASSERT_EQ(ts1.m_errs, 0);
   ASSERT_TRUE(ts1.m_n_allocd > 0);
   ASSERT_TRUE(ts1.m_bufptr != nullptr);
}


//Delete one, object with 1 element, delete point 2.
TEST_F(LgBufUint8Test, DeleteOne1ElDp2)
{
   LgBufUint8 ts1;

   ts1.AppendOne(0xD3);

   ts1.DeleteOne(2);

   ASSERT_EQ(ts1.m_n_used, 1);
   ASSERT_EQ(ts1.m_errs, 0);
   ASSERT_TRUE(ts1.m_n_allocd > 0);
   ASSERT_TRUE(ts1.m_bufptr != nullptr);
}


//Delete one, object with 1 element, delete point 3.
TEST_F(LgBufUint8Test, DeleteOne1ElDp3)
{
   LgBufUint8 ts1;

   ts1.AppendOne(0xD3);

   ts1.DeleteOne(3);

   ASSERT_EQ(ts1.m_n_used, 1);
   ASSERT_EQ(ts1.m_errs, 0);
   ASSERT_TRUE(ts1.m_n_allocd > 0);
   ASSERT_TRUE(ts1.m_bufptr != nullptr);
}


//Delete one, object with 2 elements, delete point 0.
TEST_F(LgBufUint8Test, DeleteOne2ElDp0)
{
   LgBufUint8 ts1;

   ts1.AppendOne(0xD3);
   ts1.AppendOne(0xC9);

   ts1.DeleteOne(0);

   ASSERT_EQ(ts1.m_n_used, 1);
   ASSERT_EQ(ts1.m_errs, 0);
   ASSERT_TRUE(ts1.m_n_allocd > 0);
   ASSERT_TRUE(ts1.m_bufptr != nullptr);
   ASSERT_EQ(ts1.m_bufptr[0], 0xC9);
}


//Delete one, object with 2 elements, delete point 1.
TEST_F(LgBufUint8Test, DeleteOne2ElDp1)
{
   LgBufUint8 ts1;

   ts1.AppendOne(0xD3);
   ts1.AppendOne(0xC9);

   ts1.DeleteOne(1);

   ASSERT_EQ(ts1.m_n_used, 1);
   ASSERT_EQ(ts1.m_errs, 0);
   ASSERT_TRUE(ts1.m_n_allocd > 0);
   ASSERT_TRUE(ts1.m_bufptr != nullptr);
   ASSERT_EQ(ts1.m_bufptr[0], 0xD3);
}


//Delete one, object with 2 elements, delete point 2.
TEST_F(LgBufUint8Test, DeleteOne2ElDp2)
{
   LgBufUint8 ts1;

   ts1.AppendOne(0xD3);
   ts1.AppendOne(0xC9);

   ts1.DeleteOne(2);

   ASSERT_EQ(ts1.m_n_used, 2);
   ASSERT_EQ(ts1.m_errs, 0);
   ASSERT_TRUE(ts1.m_n_allocd > 0);
   ASSERT_TRUE(ts1.m_bufptr != nullptr);
   ASSERT_EQ(ts1.m_bufptr[0], 0xD3);
   ASSERT_EQ(ts1.m_bufptr[1], 0xC9);
}


//Delete one, object with 2 elements, delete point 3.
TEST_F(LgBufUint8Test, DeleteOne2ElDp3)
{
   LgBufUint8 ts1;

   ts1.AppendOne(0xD3);
   ts1.AppendOne(0xC9);

   ts1.DeleteOne(3);

   ASSERT_EQ(ts1.m_n_used, 2);
   ASSERT_EQ(ts1.m_errs, 0);
   ASSERT_TRUE(ts1.m_n_allocd > 0);
   ASSERT_TRUE(ts1.m_bufptr != nullptr);
   ASSERT_EQ(ts1.m_bufptr[0], 0xD3);
   ASSERT_EQ(ts1.m_bufptr[1], 0xC9);
}


//Delete one, object with 5000 elements, delete point 0.
TEST_F(LgBufUint8Test, DeleteOne5000ElDp0)
{
   LgBufUint8 ts1;
   size_t iter;

   //Form a buffer with reproducible contents for the first 5000 entries.
   for (iter = 0; iter < 5000; iter++)
   {
      ts1.AppendOne((uint8_t)(iter + 3));
   }

   ts1.DeleteOne(0);

   ASSERT_EQ(ts1.m_n_used, 4999);
   ASSERT_EQ(ts1.m_errs, 0);
   ASSERT_TRUE(ts1.m_n_allocd > 0);
   ASSERT_TRUE(ts1.m_bufptr != nullptr);

   for (iter = 0; iter < 4999; iter++)
   {
      ASSERT_EQ(ts1.m_bufptr[iter], (uint8_t)(iter + 4));
   }
}


//Delete one, object with 5000 elements, delete point 1.
TEST_F(LgBufUint8Test, DeleteOne5000ElDp1)
{
   LgBufUint8 ts1;
   size_t iter;

   //Form a buffer with reproducible contents for the first 5000 entries.
   for (iter = 0; iter < 5000; iter++)
   {
      ts1.AppendOne((uint8_t)(iter + 3));
   }

   ts1.DeleteOne(1);

   ASSERT_EQ(ts1.m_n_used, 4999);
   ASSERT_EQ(ts1.m_errs, 0);
   ASSERT_TRUE(ts1.m_n_allocd > 0);
   ASSERT_TRUE(ts1.m_bufptr != nullptr);

   for (iter = 0; iter < 1; iter++)
   {
      ASSERT_EQ(ts1.m_bufptr[iter], (uint8_t)(iter + 3));
   }
   for (iter = 1; iter < 4999; iter++)
   {
      ASSERT_EQ(ts1.m_bufptr[iter], (uint8_t)(iter + 4));
   }
}


//Delete one, object with 5000 elements, delete point 2.
TEST_F(LgBufUint8Test, DeleteOne5000ElDp2)
{
   LgBufUint8 ts1;
   size_t iter;

   //Form a buffer with reproducible contents for the first 5000 entries.
   for (iter = 0; iter < 5000; iter++)
   {
      ts1.AppendOne((uint8_t)(iter + 3));
   }

   ts1.DeleteOne(2);

   ASSERT_EQ(ts1.m_n_used, 4999);
   ASSERT_EQ(ts1.m_errs, 0);
   ASSERT_TRUE(ts1.m_n_allocd > 0);
   ASSERT_TRUE(ts1.m_bufptr != nullptr);

   for (iter = 0; iter < 2; iter++)
   {
      ASSERT_EQ(ts1.m_bufptr[iter], (uint8_t)(iter + 3));
   }
   for (iter = 2; iter < 4999; iter++)
   {
      ASSERT_EQ(ts1.m_bufptr[iter], (uint8_t)(iter + 4));
   }
}


//Delete one, object with 5000 elements, delete point 500.
TEST_F(LgBufUint8Test, DeleteOne5000ElDp500)
{
   LgBufUint8 ts1;
   size_t iter;

   //Form a buffer with reproducible contents for the first 5000 entries.
   for (iter = 0; iter < 5000; iter++)
   {
      ts1.AppendOne((uint8_t)(iter + 3));
   }

   ts1.DeleteOne(500);

   ASSERT_EQ(ts1.m_n_used, 4999);
   ASSERT_EQ(ts1.m_errs, 0);
   ASSERT_TRUE(ts1.m_n_allocd > 0);
   ASSERT_TRUE(ts1.m_bufptr != nullptr);

   for (iter = 0; iter < 500; iter++)
   {
      ASSERT_EQ(ts1.m_bufptr[iter], (uint8_t)(iter + 3));
   }
   for (iter = 500; iter < 4999; iter++)
   {
      ASSERT_EQ(ts1.m_bufptr[iter], (uint8_t)(iter + 4));
   }
}


//Delete one, object with 5000 elements, delete point 4998.
TEST_F(LgBufUint8Test, DeleteOne5000ElDp4998)
{
   LgBufUint8 ts1;
   size_t iter;

   //Form a buffer with reproducible contents for the first 5000 entries.
   for (iter = 0; iter < 5000; iter++)
   {
      ts1.AppendOne((uint8_t)(iter + 3));
   }

   ts1.DeleteOne(4998);

   ASSERT_EQ(ts1.m_n_used, 4999);
   ASSERT_EQ(ts1.m_errs, 0);
   ASSERT_TRUE(ts1.m_n_allocd > 0);
   ASSERT_TRUE(ts1.m_bufptr != nullptr);

   for (iter = 0; iter < 4998; iter++)
   {
      ASSERT_EQ(ts1.m_bufptr[iter], (uint8_t)(iter + 3));
   }
   for (iter = 4998; iter < 4999; iter++)
   {
      ASSERT_EQ(ts1.m_bufptr[iter], (uint8_t)(iter + 4));
   }
}


//Delete one, object with 5000 elements, delete point 4999.
TEST_F(LgBufUint8Test, DeleteOne5000ElDp4999)
{
   LgBufUint8 ts1;
   size_t iter;

   //Form a buffer with reproducible contents for the first 5000 entries.
   for (iter = 0; iter < 5000; iter++)
   {
      ts1.AppendOne((uint8_t)(iter + 3));
   }

   ts1.DeleteOne(4999);

   ASSERT_EQ(ts1.m_n_used, 4999);
   ASSERT_EQ(ts1.m_errs, 0);
   ASSERT_TRUE(ts1.m_n_allocd > 0);
   ASSERT_TRUE(ts1.m_bufptr != nullptr);

   for (iter = 0; iter < 4999; iter++)
   {
      ASSERT_EQ(ts1.m_bufptr[iter], (uint8_t)(iter + 3));
   }
}


//Delete one, object with 5000 elements, delete point 5000.
TEST_F(LgBufUint8Test, DeleteOne5000ElDp5000)
{
   LgBufUint8 ts1;
   size_t iter;

   //Form a buffer with reproducible contents for the first 5000 entries.
   for (iter = 0; iter < 5000; iter++)
   {
      ts1.AppendOne((uint8_t)(iter + 3));
   }

   ts1.DeleteOne(5000);

   ASSERT_EQ(ts1.m_n_used, 5000);
   ASSERT_EQ(ts1.m_errs, 0);
   ASSERT_TRUE(ts1.m_n_allocd > 0);
   ASSERT_TRUE(ts1.m_bufptr != nullptr);

   for (iter = 0; iter < 5000; iter++)
   {
      ASSERT_EQ(ts1.m_bufptr[iter], (uint8_t)(iter + 3));
   }
}


//Delete one, object with 5000 elements, delete point 5001.
TEST_F(LgBufUint8Test, DeleteOne5000ElDp5001)
{
   LgBufUint8 ts1;
   size_t iter;

   //Form a buffer with reproducible contents for the first 5000 entries.
   for (iter = 0; iter < 5000; iter++)
   {
      ts1.AppendOne((uint8_t)(iter + 3));
   }

   ts1.DeleteOne(5001);

   ASSERT_EQ(ts1.m_n_used, 5000);
   ASSERT_EQ(ts1.m_errs, 0);
   ASSERT_TRUE(ts1.m_n_allocd > 0);
   ASSERT_TRUE(ts1.m_bufptr != nullptr);

   for (iter = 0; iter < 5000; iter++)
   {
      ASSERT_EQ(ts1.m_bufptr[iter], (uint8_t)(iter + 3));
   }
}


//Delete one, object with 5000 elements, delete point 6000.
TEST_F(LgBufUint8Test, DeleteOne5000ElDp6000)
{
   LgBufUint8 ts1;
   size_t iter;

   //Form a buffer with reproducible contents for the first 5000 entries.
   for (iter = 0; iter < 5000; iter++)
   {
      ts1.AppendOne((uint8_t)(iter + 3));
   }

   ts1.DeleteOne(6000);

   ASSERT_EQ(ts1.m_n_used, 5000);
   ASSERT_EQ(ts1.m_errs, 0);
   ASSERT_TRUE(ts1.m_n_allocd > 0);
   ASSERT_TRUE(ts1.m_bufptr != nullptr);

   for (iter = 0; iter < 5000; iter++)
   {
      ASSERT_EQ(ts1.m_bufptr[iter], (uint8_t)(iter + 3));
   }
}


//Checks delete function with object empty, delete point at start, and Dn=0.
TEST_F(LgBufUint8Test, DeleteOoDpsDn0)
{
   LgBufUint8 ts1;

   ts1.Delete(0, 0);

   //Buffer should be unchanged.
   ASSERT_EQ(ts1.m_n_used, 0);
   ASSERT_EQ(ts1.m_errs, 0);
   ASSERT_EQ(ts1.m_n_allocd, 0);
   ASSERT_EQ(ts1.m_bufptr, nullptr);
}


//Checks delete function with object empty, delete point invalid, and Dn=0.
TEST_F(LgBufUint8Test, DeleteOoDpiDn0)
{
   LgBufUint8 ts1;

   ts1.Delete(5000, 0);

   //Buffer should be unchanged.
   ASSERT_EQ(ts1.m_n_used, 0);
   ASSERT_EQ(ts1.m_errs, 0);
   ASSERT_EQ(ts1.m_n_allocd, 0);
   ASSERT_EQ(ts1.m_bufptr, nullptr);
}


//Checks delete function with object occupied, delete point in the middle, and Dn=0.
TEST_F(LgBufUint8Test, DeleteOoDpmDn0)
{
   LgBufUint8 ts1;
   size_t iter;

   //Form a buffer with reproducible contents for the first 5000 entries.
   for (iter = 0; iter < 5000; iter++)
   {
      ts1.AppendOne((uint8_t)(iter + 3));
   }

   ts1.Delete(2500, 0);

   //Buffer should be unchanged.
   ASSERT_EQ(ts1.m_n_used, 5000);
   ASSERT_EQ(ts1.m_errs, 0);
   ASSERT_TRUE(ts1.m_n_allocd > 0);
   ASSERT_TRUE(ts1.m_bufptr != nullptr);

   for (iter = 0; iter < 5000; iter++)
   {
      ASSERT_EQ(ts1.m_bufptr[iter], (uint8_t)(iter + 3));
   }
}


//Checks delete function with object occupied, delete point at the start, and Dn=1.
TEST_F(LgBufUint8Test, DeleteOoDpsDn1)
{
   LgBufUint8 ts1;
   size_t iter;

   //Form a buffer with reproducible contents for the first 5000 entries.
   for (iter = 0; iter < 5000; iter++)
   {
      ts1.AppendOne((uint8_t)(iter + 3));
   }

   ts1.Delete(0, 1);

   //First element should be missing.
   ASSERT_EQ(ts1.m_n_used, 4999);
   ASSERT_EQ(ts1.m_errs, 0);
   ASSERT_TRUE(ts1.m_n_allocd >= ts1.m_n_used);
   ASSERT_TRUE(ts1.m_bufptr != nullptr);

   for (iter = 0; iter < 4999; iter++)
   {
      ASSERT_EQ(ts1.m_bufptr[iter], (uint8_t)(iter + 4));
   }
}


//Checks delete function with object occupied, delete point 1 past the start, and Dn=1.
TEST_F(LgBufUint8Test, DeleteOoDp1Dn1)
{
   LgBufUint8 ts1;
   size_t iter;

   //Form a buffer with reproducible contents for the first 5000 entries.
   for (iter = 0; iter < 5000; iter++)
   {
      ts1.AppendOne((uint8_t)(iter + 3));
   }

   ts1.Delete(1, 1);

   //Second element should be missing.
   ASSERT_EQ(ts1.m_n_used, 4999);
   ASSERT_EQ(ts1.m_errs, 0);
   ASSERT_TRUE(ts1.m_n_allocd >= ts1.m_n_used);
   ASSERT_TRUE(ts1.m_bufptr != nullptr);

   for (iter = 0; iter < 1; iter++)
   {
      ASSERT_EQ(ts1.m_bufptr[iter], (uint8_t)(iter + 3));
   }
   for (iter = 1; iter < 4999; iter++)
   {
      ASSERT_EQ(ts1.m_bufptr[iter], (uint8_t)(iter + 4));
   }
}


//Checks delete function with object occupied, delete point 2 past the start, and Dn=1.
TEST_F(LgBufUint8Test, DeleteOoDp2Dn1)
{
   LgBufUint8 ts1;
   size_t iter;

   //Form a buffer with reproducible contents for the first 5000 entries.
   for (iter = 0; iter < 5000; iter++)
   {
      ts1.AppendOne((uint8_t)(iter + 3));
   }

   ts1.Delete(2, 1);

   //Second element should be missing.
   ASSERT_EQ(ts1.m_n_used, 4999);
   ASSERT_EQ(ts1.m_errs, 0);
   ASSERT_TRUE(ts1.m_n_allocd >= ts1.m_n_used);
   ASSERT_TRUE(ts1.m_bufptr != nullptr);

   for (iter = 0; iter < 2; iter++)
   {
      ASSERT_EQ(ts1.m_bufptr[iter], (uint8_t)(iter + 3));
   }
   for (iter = 2; iter < 4999; iter++)
   {
      ASSERT_EQ(ts1.m_bufptr[iter], (uint8_t)(iter + 4));
   }
}


//Checks delete function with object occupied, delete point mid, and Dn=1.
TEST_F(LgBufUint8Test, DeleteOoDpmidDn1)
{
   LgBufUint8 ts1;
   size_t iter;

   //Form a buffer with reproducible contents for the first 5000 entries.
   for (iter = 0; iter < 5000; iter++)
   {
      ts1.AppendOne((uint8_t)(iter + 3));
   }

   ts1.Delete(2023, 1);

   //Second element should be missing.
   ASSERT_EQ(ts1.m_n_used, 4999);
   ASSERT_EQ(ts1.m_errs, 0);
   ASSERT_TRUE(ts1.m_n_allocd >= ts1.m_n_used);
   ASSERT_TRUE(ts1.m_bufptr != nullptr);

   for (iter = 0; iter < 2023; iter++)
   {
      ASSERT_EQ(ts1.m_bufptr[iter], (uint8_t)(iter + 3));
   }
   for (iter = 2023; iter < 4999; iter++)
   {
      ASSERT_EQ(ts1.m_bufptr[iter], (uint8_t)(iter + 4));
   }
}


//Checks delete function with object occupied, delete point end minus 1, and Dn=1.
TEST_F(LgBufUint8Test, DeleteOoDpe1Dn1)
{
   LgBufUint8 ts1;
   size_t iter;

   //Form a buffer with reproducible contents for the first 5000 entries.
   for (iter = 0; iter < 5000; iter++)
   {
      ts1.AppendOne((uint8_t)(iter + 3));
   }

   ts1.Delete(4998, 1);

   //Second element should be missing.
   ASSERT_EQ(ts1.m_n_used, 4999);
   ASSERT_EQ(ts1.m_errs, 0);
   ASSERT_TRUE(ts1.m_n_allocd >= ts1.m_n_used);
   ASSERT_TRUE(ts1.m_bufptr != nullptr);

   for (iter = 0; iter < 4998; iter++)
   {
      ASSERT_EQ(ts1.m_bufptr[iter], (uint8_t)(iter + 3));
   }
   for (iter = 4998; iter < 4999; iter++)
   {
      ASSERT_EQ(ts1.m_bufptr[iter], (uint8_t)(iter + 4));
   }
}


//Checks delete function with object occupied, delete point end, and Dn=1.
TEST_F(LgBufUint8Test, DeleteOoDpeDn1)
{
   LgBufUint8 ts1;
   size_t iter;

   //Form a buffer with reproducible contents for the first 5000 entries.
   for (iter = 0; iter < 5000; iter++)
   {
      ts1.AppendOne((uint8_t)(iter + 3));
   }

   ts1.Delete(4999, 1);

   //Second element should be missing.
   ASSERT_EQ(ts1.m_n_used, 4999);
   ASSERT_EQ(ts1.m_errs, 0);
   ASSERT_TRUE(ts1.m_n_allocd >= ts1.m_n_used);
   ASSERT_TRUE(ts1.m_bufptr != nullptr);

   for (iter = 0; iter < 4999; iter++)
   {
      ASSERT_EQ(ts1.m_bufptr[iter], (uint8_t)(iter + 3));
   }
}


//Checks delete function with object occupied, delete point end plus 1, and Dn=1.
TEST_F(LgBufUint8Test, DeleteOoDpep1Dn1)
{
   LgBufUint8 ts1;
   size_t iter;

   //Form a buffer with reproducible contents for the first 5000 entries.
   for (iter = 0; iter < 5000; iter++)
   {
      ts1.AppendOne((uint8_t)(iter + 3));
   }

   ts1.Delete(5000, 1);

   //Second element should be missing.
   ASSERT_EQ(ts1.m_n_used, 5000);
   ASSERT_EQ(ts1.m_errs, 0);
   ASSERT_TRUE(ts1.m_n_allocd >= ts1.m_n_used);
   ASSERT_TRUE(ts1.m_bufptr != nullptr);

   for (iter = 0; iter < 5000; iter++)
   {
      ASSERT_EQ(ts1.m_bufptr[iter], (uint8_t)(iter + 3));
   }
}


//Checks delete function with object occupied, delete point at the start, and Dn=2.
TEST_F(LgBufUint8Test, DeleteOoDpsDn2)
{
   LgBufUint8 ts1;
   size_t iter;

   //Form a buffer with reproducible contents for the first 5000 entries.
   for (iter = 0; iter < 5000; iter++)
   {
      ts1.AppendOne((uint8_t)(iter + 3));
   }

   ts1.Delete(0, 2);

   //First element should be missing.
   ASSERT_EQ(ts1.m_n_used, 4998);
   ASSERT_EQ(ts1.m_errs, 0);
   ASSERT_TRUE(ts1.m_n_allocd >= ts1.m_n_used);
   ASSERT_TRUE(ts1.m_bufptr != nullptr);

   for (iter = 0; iter < 4998; iter++)
   {
      ASSERT_EQ(ts1.m_bufptr[iter], (uint8_t)(iter + 5));
   }
}


//Checks delete function with object occupied, delete point 1 past the start, and Dn=2.
TEST_F(LgBufUint8Test, DeleteOoDp1Dn2)
{
   LgBufUint8 ts1;
   size_t iter;

   //Form a buffer with reproducible contents for the first 5000 entries.
   for (iter = 0; iter < 5000; iter++)
   {
      ts1.AppendOne((uint8_t)(iter + 3));
   }

   ts1.Delete(1, 2);

   //Second and third elements should be missing.
   ASSERT_EQ(ts1.m_n_used, 4998);
   ASSERT_EQ(ts1.m_errs, 0);
   ASSERT_TRUE(ts1.m_n_allocd >= ts1.m_n_used);
   ASSERT_TRUE(ts1.m_bufptr != nullptr);

   for (iter = 0; iter < 1; iter++)
   {
      ASSERT_EQ(ts1.m_bufptr[iter], (uint8_t)(iter + 3));
   }
   for (iter = 1; iter < 4998; iter++)
   {
      ASSERT_EQ(ts1.m_bufptr[iter], (uint8_t)(iter + 5));
   }
}


//Checks delete function with object occupied, delete point 2 past the start, and Dn=2.
TEST_F(LgBufUint8Test, DeleteOoDp2Dn2)
{
   LgBufUint8 ts1;
   size_t iter;

   //Form a buffer with reproducible contents for the first 5000 entries.
   for (iter = 0; iter < 5000; iter++)
   {
      ts1.AppendOne((uint8_t)(iter + 3));
   }

   ts1.Delete(2, 2);

   //Third and fourth elements should be missing.
   ASSERT_EQ(ts1.m_n_used, 4998);
   ASSERT_EQ(ts1.m_errs, 0);
   ASSERT_TRUE(ts1.m_n_allocd >= ts1.m_n_used);
   ASSERT_TRUE(ts1.m_bufptr != nullptr);

   for (iter = 0; iter < 2; iter++)
   {
      ASSERT_EQ(ts1.m_bufptr[iter], (uint8_t)(iter + 3));
   }
   for (iter = 2; iter < 4998; iter++)
   {
      ASSERT_EQ(ts1.m_bufptr[iter], (uint8_t)(iter + 5));
   }
}


//Checks delete function with object occupied, delete point mid, and Dn=2.
TEST_F(LgBufUint8Test, DeleteOoDpmidDn2)
{
   LgBufUint8 ts1;
   size_t iter;

   //Form a buffer with reproducible contents for the first 5000 entries.
   for (iter = 0; iter < 5000; iter++)
   {
      ts1.AppendOne((uint8_t)(iter + 3));
   }

   ts1.Delete(2023, 2);

   ASSERT_EQ(ts1.m_n_used, 4998);
   ASSERT_EQ(ts1.m_errs, 0);
   ASSERT_TRUE(ts1.m_n_allocd >= ts1.m_n_used);
   ASSERT_TRUE(ts1.m_bufptr != nullptr);

   for (iter = 0; iter < 2023; iter++)
   {
      ASSERT_EQ(ts1.m_bufptr[iter], (uint8_t)(iter + 3));
   }
   for (iter = 2023; iter < 4998; iter++)
   {
      ASSERT_EQ(ts1.m_bufptr[iter], (uint8_t)(iter + 5));
   }
}


//Checks delete function with object occupied, delete point end minus 2, and Dn=2.
TEST_F(LgBufUint8Test, DeleteOoDpe2Dn2)
{
   LgBufUint8 ts1;
   size_t iter;

   //Form a buffer with reproducible contents for the first 5000 entries.
   for (iter = 0; iter < 5000; iter++)
   {
      ts1.AppendOne((uint8_t)(iter + 3));
   }

   ts1.Delete(4997, 2);

   //4997 and 4998 should be missing, 4999 in place.
   ASSERT_EQ(ts1.m_n_used, 4998);
   ASSERT_EQ(ts1.m_errs, 0);
   ASSERT_TRUE(ts1.m_n_allocd >= ts1.m_n_used);
   ASSERT_TRUE(ts1.m_bufptr != nullptr);

   for (iter = 0; iter < 4997; iter++)
   {
      ASSERT_EQ(ts1.m_bufptr[iter], (uint8_t)(iter + 3));
   }
   for (iter = 4997; iter < 4998; iter++)
   {
      ASSERT_EQ(ts1.m_bufptr[iter], (uint8_t)(iter + 5));
   }
}


//Checks delete function with object occupied, delete point end minus 1, and Dn=2.
TEST_F(LgBufUint8Test, DeleteOoDpe1Dn2)
{
   LgBufUint8 ts1;
   size_t iter;

   //Form a buffer with reproducible contents for the first 5000 entries.
   for (iter = 0; iter < 5000; iter++)
   {
      ts1.AppendOne((uint8_t)(iter + 3));
   }

   ts1.Delete(4998, 2);

   //4998 and 4999 should be missing.
   ASSERT_EQ(ts1.m_n_used, 4998);
   ASSERT_EQ(ts1.m_errs, 0);
   ASSERT_TRUE(ts1.m_n_allocd >= ts1.m_n_used);
   ASSERT_TRUE(ts1.m_bufptr != nullptr);

   for (iter = 0; iter < 4998; iter++)
   {
      ASSERT_EQ(ts1.m_bufptr[iter], (uint8_t)(iter + 3));
   }
}


//Checks delete function with object occupied, delete point end, and Dn=2.
//This should result in a truncation of the request, and only 1 element deleted.
TEST_F(LgBufUint8Test, DeleteOoDpeDn2)
{
   LgBufUint8 ts1;
   size_t iter;

   //Form a buffer with reproducible contents for the first 5000 entries.
   for (iter = 0; iter < 5000; iter++)
   {
      ts1.AppendOne((uint8_t)(iter + 3));
   }

   ts1.Delete(4999, 2);

   ASSERT_EQ(ts1.m_n_used, 4999);
   ASSERT_EQ(ts1.m_errs, 0);
   ASSERT_TRUE(ts1.m_n_allocd >= ts1.m_n_used);
   ASSERT_TRUE(ts1.m_bufptr != nullptr);

   for (iter = 0; iter < 4999; iter++)
   {
      ASSERT_EQ(ts1.m_bufptr[iter], (uint8_t)(iter + 3));
   }
}


//Checks delete function with object occupied, delete point end plus 1, and Dn=2.
TEST_F(LgBufUint8Test, DeleteOoDpep1Dn2)
{
   LgBufUint8 ts1;
   size_t iter;

   //Form a buffer with reproducible contents for the first 5000 entries.
   for (iter = 0; iter < 5000; iter++)
   {
      ts1.AppendOne((uint8_t)(iter + 3));
   }

   ts1.Delete(5000, 1);

   //Second element should be missing.
   ASSERT_EQ(ts1.m_n_used, 5000);
   ASSERT_EQ(ts1.m_errs, 0);
   ASSERT_TRUE(ts1.m_n_allocd >= ts1.m_n_used);
   ASSERT_TRUE(ts1.m_bufptr != nullptr);

   for (iter = 0; iter < 5000; iter++)
   {
      ASSERT_EQ(ts1.m_bufptr[iter], (uint8_t)(iter + 3));
   }
}


//Checks delete function with object occupied, delete point at the start, and Dn=100.
TEST_F(LgBufUint8Test, DeleteOoDpsDn100)
{
   LgBufUint8 ts1;
   size_t iter;

   //Form a buffer with reproducible contents for the first 5000 entries.
   for (iter = 0; iter < 5000; iter++)
   {
      ts1.AppendOne((uint8_t)(iter + 3));
   }

   ts1.Delete(0, 100);

   //First element should be missing.
   ASSERT_EQ(ts1.m_n_used, 4900);
   ASSERT_EQ(ts1.m_errs, 0);
   ASSERT_TRUE(ts1.m_n_allocd >= ts1.m_n_used);
   ASSERT_TRUE(ts1.m_bufptr != nullptr);

   for (iter = 0; iter < 4900; iter++)
   {
      ASSERT_EQ(ts1.m_bufptr[iter], (uint8_t)(iter + 103));
   }
}


//Checks delete function with object occupied, delete point 1 past the start, and Dn=100.
TEST_F(LgBufUint8Test, DeleteOoDp1Dn100)
{
   LgBufUint8 ts1;
   size_t iter;

   //Form a buffer with reproducible contents for the first 5000 entries.
   for (iter = 0; iter < 5000; iter++)
   {
      ts1.AppendOne((uint8_t)(iter + 3));
   }

   ts1.Delete(1, 100);

   ASSERT_EQ(ts1.m_n_used, 4900);
   ASSERT_EQ(ts1.m_errs, 0);
   ASSERT_TRUE(ts1.m_n_allocd >= ts1.m_n_used);
   ASSERT_TRUE(ts1.m_bufptr != nullptr);

   for (iter = 0; iter < 1; iter++)
   {
      ASSERT_EQ(ts1.m_bufptr[iter], (uint8_t)(iter + 3));
   }
   for (iter = 1; iter < 4900; iter++)
   {
      ASSERT_EQ(ts1.m_bufptr[iter], (uint8_t)(iter + 103));
   }
}


//Checks delete function with object occupied, delete point 2 past the start, and Dn=100.
TEST_F(LgBufUint8Test, DeleteOoDp2Dn100)
{
   LgBufUint8 ts1;
   size_t iter;

   //Form a buffer with reproducible contents for the first 5000 entries.
   for (iter = 0; iter < 5000; iter++)
   {
      ts1.AppendOne((uint8_t)(iter + 3));
   }

   ts1.Delete(2, 100);

   //Third and fourth elements should be missing.
   ASSERT_EQ(ts1.m_n_used, 4900);
   ASSERT_EQ(ts1.m_errs, 0);
   ASSERT_TRUE(ts1.m_n_allocd >= ts1.m_n_used);
   ASSERT_TRUE(ts1.m_bufptr != nullptr);

   for (iter = 0; iter < 2; iter++)
   {
      ASSERT_EQ(ts1.m_bufptr[iter], (uint8_t)(iter + 3));
   }
   for (iter = 2; iter < 4900; iter++)
   {
      ASSERT_EQ(ts1.m_bufptr[iter], (uint8_t)(iter + 103));
   }
}


//Checks delete function with object occupied, delete point mid, and Dn=100.
TEST_F(LgBufUint8Test, DeleteOoDpmidDn100)
{
   LgBufUint8 ts1;
   size_t iter;

   //Form a buffer with reproducible contents for the first 5000 entries.
   for (iter = 0; iter < 5000; iter++)
   {
      ts1.AppendOne((uint8_t)(iter + 3));
   }

   ts1.Delete(2023, 100);

   ASSERT_EQ(ts1.m_n_used, 4900);
   ASSERT_EQ(ts1.m_errs, 0);
   ASSERT_TRUE(ts1.m_n_allocd >= ts1.m_n_used);
   ASSERT_TRUE(ts1.m_bufptr != nullptr);

   for (iter = 0; iter < 2023; iter++)
   {
      ASSERT_EQ(ts1.m_bufptr[iter], (uint8_t)(iter + 3));
   }
   for (iter = 2023; iter < 4900; iter++)
   {
      ASSERT_EQ(ts1.m_bufptr[iter], (uint8_t)(iter + 103));
   }
}


//Checks delete function with object occupied, delete point end minus 100, and Dn=100.
//This should delete elements 4899 through 4998, and leave element 4999, bumped down
//to location 4899.
TEST_F(LgBufUint8Test, DeleteOoDpe100Dn100)
{
   LgBufUint8 ts1;
   size_t iter;

   //Form a buffer with reproducible contents for the first 5000 entries.
   for (iter = 0; iter < 5000; iter++)
   {
      ts1.AppendOne((uint8_t)(iter + 3));
   }

   ts1.Delete(4899, 100);

   //4997 and 4998 should be missing, 4999 in place.
   ASSERT_EQ(ts1.m_n_used, 4900);
   ASSERT_EQ(ts1.m_errs, 0);
   ASSERT_TRUE(ts1.m_n_allocd >= ts1.m_n_used);
   ASSERT_TRUE(ts1.m_bufptr != nullptr);

   for (iter = 0; iter < 4899; iter++)
   {
      ASSERT_EQ(ts1.m_bufptr[iter], (uint8_t)(iter + 3));
   }
   for (iter = 4899; iter < 4900; iter++)
   {
      ASSERT_EQ(ts1.m_bufptr[iter], (uint8_t)(iter + 103));
   }
}


//Checks delete function with object occupied, delete point end minus 99, and Dn=100.
//This should delete elements 4900 through 4999, a truncation.
TEST_F(LgBufUint8Test, DeleteOoDpe99Dn100)
{
   LgBufUint8 ts1;
   size_t iter;

   //Form a buffer with reproducible contents for the first 5000 entries.
   for (iter = 0; iter < 5000; iter++)
   {
      ts1.AppendOne((uint8_t)(iter + 3));
   }

   ts1.Delete(4900, 100);

   //4997 and 4998 should be missing, 4999 in place.
   ASSERT_EQ(ts1.m_n_used, 4900);
   ASSERT_EQ(ts1.m_errs, 0);
   ASSERT_TRUE(ts1.m_n_allocd >= ts1.m_n_used);
   ASSERT_TRUE(ts1.m_bufptr != nullptr);

   for (iter = 0; iter < 4900; iter++)
   {
      ASSERT_EQ(ts1.m_bufptr[iter], (uint8_t)(iter + 3));
   }
}


//Checks delete function with object occupied, delete point end minus 98, and Dn=100.
//This should delete elements 4901 through 4999, only 99 elements.
TEST_F(LgBufUint8Test, DeleteOoDpe98Dn100)
{
   LgBufUint8 ts1;
   size_t iter;

   //Form a buffer with reproducible contents for the first 5000 entries.
   for (iter = 0; iter < 5000; iter++)
   {
      ts1.AppendOne((uint8_t)(iter + 3));
   }

   ts1.Delete(4901, 100);

   //4997 and 4998 should be missing, 4999 in place.
   ASSERT_EQ(ts1.m_n_used, 4901);
   ASSERT_EQ(ts1.m_errs, 0);
   ASSERT_TRUE(ts1.m_n_allocd >= ts1.m_n_used);
   ASSERT_TRUE(ts1.m_bufptr != nullptr);

   for (iter = 0; iter < 4901; iter++)
   {
      ASSERT_EQ(ts1.m_bufptr[iter], (uint8_t)(iter + 3));
   }
}


//Checks delete function with object occupied, delete point end minus 1, and Dn=100.
//This should delete elements 4998 and 4999 only.
TEST_F(LgBufUint8Test, DeleteOoDpe1Dn100)
{
   LgBufUint8 ts1;
   size_t iter;

   //Form a buffer with reproducible contents for the first 5000 entries.
   for (iter = 0; iter < 5000; iter++)
   {
      ts1.AppendOne((uint8_t)(iter + 3));
   }

   ts1.Delete(4998, 100);

   ASSERT_EQ(ts1.m_n_used, 4998);
   ASSERT_EQ(ts1.m_errs, 0);
   ASSERT_TRUE(ts1.m_n_allocd >= ts1.m_n_used);
   ASSERT_TRUE(ts1.m_bufptr != nullptr);

   for (iter = 0; iter < 4998; iter++)
   {
      ASSERT_EQ(ts1.m_bufptr[iter], (uint8_t)(iter + 3));
   }
}


//Checks delete function with object occupied, delete point end minus 0, and Dn=100.
//This should delete element 4999 only.
TEST_F(LgBufUint8Test, DeleteOoDpe0Dn100)
{
   LgBufUint8 ts1;
   size_t iter;

   //Form a buffer with reproducible contents for the first 5000 entries.
   for (iter = 0; iter < 5000; iter++)
   {
      ts1.AppendOne((uint8_t)(iter + 3));
   }

   ts1.Delete(4999, 100);

   ASSERT_EQ(ts1.m_n_used, 4999);
   ASSERT_EQ(ts1.m_errs, 0);
   ASSERT_TRUE(ts1.m_n_allocd >= ts1.m_n_used);
   ASSERT_TRUE(ts1.m_bufptr != nullptr);

   for (iter = 0; iter < 4999; iter++)
   {
      ASSERT_EQ(ts1.m_bufptr[iter], (uint8_t)(iter + 3));
   }
}


//Checks delete function with object occupied, delete point end plus 1, and Dn=100.
TEST_F(LgBufUint8Test, DeleteOoDpep1Dn100)
{
   LgBufUint8 ts1;
   size_t iter;

   //Form a buffer with reproducible contents for the first 5000 entries.
   for (iter = 0; iter < 5000; iter++)
   {
      ts1.AppendOne((uint8_t)(iter + 3));
   }

   ts1.Delete(5000, 100);

   ASSERT_EQ(ts1.m_n_used, 5000);
   ASSERT_EQ(ts1.m_errs, 0);
   ASSERT_TRUE(ts1.m_n_allocd >= ts1.m_n_used);
   ASSERT_TRUE(ts1.m_bufptr != nullptr);

   for (iter = 0; iter < 5000; iter++)
   {
      ASSERT_EQ(ts1.m_bufptr[iter], (uint8_t)(iter + 3));
   }
}


//Checks delete function with object occupied, delete point start, and Dn=10000.
//This should delete everything.
TEST_F(LgBufUint8Test, DeleteOoDpsDn10000)
{
   LgBufUint8 ts1;
   size_t iter;

   //Form a buffer with reproducible contents for the first 5000 entries.
   for (iter = 0; iter < 5000; iter++)
   {
      ts1.AppendOne((uint8_t)(iter + 3));
   }

   ts1.Delete(0, 10000);

   ASSERT_EQ(ts1.m_n_used, 0);
   ASSERT_EQ(ts1.m_errs, 0);
   ASSERT_TRUE(ts1.m_n_allocd >= ts1.m_n_used);
   ASSERT_TRUE(ts1.m_bufptr != nullptr);
}


//Checks read and delete one, happy path.  Minimal unit testing because the tested
//function uses functions that have been carefully unit tested.
TEST_F(LgBufUint8Test, ReadAndDeleteOneHappyPath)
{
   LgBufUint8 ts1;
   size_t iter;
   uint8_t rv;

   //Form a buffer with reproducible contents for the first 5000 entries.
   for (iter = 0; iter < 5000; iter++)
   {
      ts1.AppendOne((uint8_t)(iter + 17));
   }

   rv = ts1.ReadAndDeleteOne(20);

   ASSERT_EQ(rv, 20 + 17);

   ASSERT_EQ(ts1.m_n_used, 4999);
   ASSERT_EQ(ts1.m_errs, 0);
   ASSERT_TRUE(ts1.m_n_allocd >= ts1.m_n_used);
   ASSERT_TRUE(ts1.m_bufptr != nullptr);

   for (iter = 0; iter < 20; iter++)
   {
      ASSERT_EQ(ts1.m_bufptr[iter], (uint8_t)(iter + 17));
   }
   for (iter = 20; iter < 4999; iter++)
   {
      ASSERT_EQ(ts1.m_bufptr[iter], (uint8_t)(iter + 17 + 1));
   }
}


//Checks read and delete one, invalid index path.  Minimal unit testing because the tested
//function uses functions that have been carefully unit tested.
TEST_F(LgBufUint8Test, ReadAndDeleteOneInvalidSubscript)
{
   LgBufUint8 ts1;
   size_t iter;
   uint8_t rv;

   //Form a buffer with reproducible contents for the first 5000 entries.
   for (iter = 0; iter < 5000; iter++)
   {
      ts1.AppendOne((uint8_t)(iter + 17));
   }

   rv = ts1.ReadAndDeleteOne(5000);

   ASSERT_EQ(ts1.m_n_used, 5000);
   ASSERT_EQ(ts1.m_errs, 0);
   ASSERT_TRUE(ts1.m_n_allocd >= ts1.m_n_used);
   ASSERT_TRUE(ts1.m_bufptr != nullptr);

   for (iter = 0; iter < 5000; iter++)
   {
      ASSERT_EQ(ts1.m_bufptr[iter], (uint8_t)(iter + 17));
   }
}


//Checks read and delete happy path.  Minimal unit testing because the tested
//function uses functions that have been carefully unit tested.
TEST_F(LgBufUint8Test, ReadAndDeleteHappyPath)
{
   LgBufUint8 ts1;
   size_t iter;
   uint8_t buf[10];

   //Form a buffer with reproducible contents for the first 5000 entries.
   for (iter = 0; iter < 5000; iter++)
   {
      ts1.AppendOne((uint8_t)(iter + 19));
   }

   //Load up the buffer so we can determine what was assigned.
   memset(buf, 0xAC, sizeof(buf));

   ts1.ReadAndDelete(10, buf, 9);

   //Values read out via read call.
   for (iter = 0; iter < 9; iter++)
   {
      ASSERT_EQ(buf[iter], (uint8_t)(iter + 19 + 10));
   }

   //Final location in buffer should be untouched.
   ASSERT_EQ(buf[9], 0xAC);

   ASSERT_EQ(ts1.m_n_used, 4991);
   ASSERT_EQ(ts1.m_errs, 0);
   ASSERT_TRUE(ts1.m_n_allocd >= ts1.m_n_used);
   ASSERT_TRUE(ts1.m_bufptr != nullptr);

   for (iter = 0; iter < 10; iter++)
   {
      ASSERT_EQ(ts1.m_bufptr[iter], (uint8_t)(iter + 19));
   }
   for (iter = 10; iter < 4991; iter++)
   {
      ASSERT_EQ(ts1.m_bufptr[iter], (uint8_t)(iter + 19 + 9));
   }
}


//Checks read and delete invalid starting subscript.  Minimal unit testing because the tested
//function uses functions that have been carefully unit tested.
TEST_F(LgBufUint8Test, ReadAndDeleteInvalidPoint)
{
   LgBufUint8 ts1;
   size_t iter;
   uint8_t buf[10];

   //Form a buffer with reproducible contents for the first 5000 entries.
   for (iter = 0; iter < 5000; iter++)
   {
      ts1.AppendOne((uint8_t)(iter + 19));
   }

   //Load up the buffer so we can determine what was assigned.
   memset(buf, 0xAC, sizeof(buf));

   ts1.ReadAndDelete(10000, buf, 9);

   //0's assigned due to invalid subscript.
   for (iter = 0; iter < 9; iter++)
   {
      ASSERT_EQ(buf[iter], 0);
   }

   //Final location in buffer should be untouched.
   ASSERT_EQ(buf[9], 0xAC);

   ASSERT_EQ(ts1.m_n_used, 5000);
   ASSERT_EQ(ts1.m_errs, 0);
   ASSERT_TRUE(ts1.m_n_allocd >= ts1.m_n_used);
   ASSERT_TRUE(ts1.m_bufptr != nullptr);

   for (iter = 0; iter < 5000; iter++)
   {
      ASSERT_EQ(ts1.m_bufptr[iter], (uint8_t)(iter + 19));
   }
}


//Checks read and delete partial overhang path.  Minimal unit testing because the tested
//function uses functions that have been carefully unit tested.
TEST_F(LgBufUint8Test, ReadAndDeletePartialOverhang)
{
   LgBufUint8 ts1;
   size_t iter;
   uint8_t buf[10];

   //Form a buffer with reproducible contents for the first 5000 entries.
   for (iter = 0; iter < 5000; iter++)
   {
      ts1.AppendOne((uint8_t)(iter + 19));
   }

   //Load up the buffer so we can determine what was assigned.
   memset(buf, 0xAC, sizeof(buf));

   //4994, 4995, 4996, 4997, 4998, 4999 should be read and deleted.
   ts1.ReadAndDelete(4994, buf, 9);

   //Values read out via read call.
   for (iter = 0; iter < 6; iter++)
   {
      ASSERT_EQ(buf[iter], (uint8_t)(iter + 19 + 4994));
   }

   //Unused portion of buffer should be 0.
   for (iter = 6; iter < 9; iter++)
   {
      ASSERT_EQ(buf[iter], 0);
   }

   //Final blockfill should be untouched.
   for (iter = 9; iter < 10; iter++)
   {
      ASSERT_EQ(buf[iter], 0xAC);
   }

   ASSERT_EQ(ts1.m_n_used, 4994);
   ASSERT_EQ(ts1.m_errs, 0);
   ASSERT_TRUE(ts1.m_n_allocd >= ts1.m_n_used);
   ASSERT_TRUE(ts1.m_bufptr != nullptr);

   for (iter = 0; iter < 4994; iter++)
   {
      ASSERT_EQ(ts1.m_bufptr[iter], (uint8_t)(iter + 19));
   }
}

//void LgBufUint8::DebugWrite(std::ostream& outstream, const std::string& title, const bool include_ending_hbar) const

//Checks read and delete partial overhang path.  Minimal unit testing because the tested
//function uses functions that have been carefully unit tested.
TEST_F(LgBufUint8Test, DebugWriteGeneralTest)
{
   //Generally disabled because it adds a lot of noise to the test report.
#if 0
   LgBufUint8 ts1;
   size_t iter;

   //Empty object.
   ts1.DebugWrite(std::cout, "Empty Object", true);

   //Populated object.
   for (iter = 0; iter < 5000; iter++)
   {
      ts1.AppendOne((uint8_t)(iter + 19));
   }
   ts1.DebugWrite(std::cout, "Populated Object", true);
#endif
}
