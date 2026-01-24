#include "gtest/gtest.h"

#include "lg_cpp_dd_fbuf_uint8.hpp"


class LgFbufUint8Test : public testing::Test {
protected:
};


//Check that the constructor seems to function.
TEST_F(LgFbufUint8Test, ConstructorCheck01)
{
    {
        //Try to get the compiler to overlay space, to better detect issues in constructor
        //being called;
        char buf[100];

        memset(buf, 0xFF, sizeof(buf));
    }
    {
       LgFbufUint8 ts1;

       //Constructor should have run at this point.
       ASSERT_EQ(ts1.m_state, LgFbufUint8State::Unocccupied);
       ASSERT_EQ(ts1.m_errs, 0);
       ASSERT_EQ(ts1.m_fname, "");

       //As part of this process, the sub-constructor should have been called.
       ASSERT_EQ(ts1.m_buf.m_n_used, 0);
       ASSERT_EQ(ts1.m_buf.m_n_allocd, 0);
       ASSERT_EQ(ts1.m_buf.m_bufptr, nullptr);
    }
}   


//Check that the copy constructor seems to function.
TEST_F(LgFbufUint8Test, CopyConstructorCheck01)
{
    LgFbufUint8 ts0;

    ts0.AppendOne(39);
    ts0.AppendOne(37);

    {
        //Try to get the compiler to overlay space, to better detect issues in copy constructor
        //being called;
        char buf[100];

        memset(buf, 0xFF, sizeof(buf));
    }
    {
        LgFbufUint8 ts1 = ts0;

        ASSERT_EQ(ts0.m_state, LgFbufUint8State::Unocccupied);
        ASSERT_EQ(ts1.m_state, LgFbufUint8State::Unocccupied);
        ASSERT_EQ(ts0.m_errs, 0);
        ASSERT_EQ(ts1.m_errs, 0);
        ASSERT_FALSE(ts0.m_buf.m_bufptr == nullptr);
        ASSERT_FALSE(ts1.m_buf.m_bufptr == nullptr);
        ASSERT_TRUE(ts0.m_buf.m_n_allocd >= 2);
        ASSERT_TRUE(ts1.m_buf.m_n_allocd >= 2);
        ASSERT_EQ(ts0.m_buf.m_n_used, 2);
        ASSERT_EQ(ts1.m_buf.m_n_used, 2);
        ASSERT_EQ(ts0.m_buf.m_bufptr[0], 39);
        ASSERT_EQ(ts1.m_buf.m_bufptr[0], 39);
        ASSERT_EQ(ts0.m_buf.m_bufptr[1], 37);
        ASSERT_EQ(ts1.m_buf.m_bufptr[1], 37);
    }
}


//Check that the copy assignment operator seems to function.
TEST_F(LgFbufUint8Test, CopyAssignmentCheck01)
{
    const char* fname = "ut_LgVerbosity.cpp";
    std::string fname_extracted;
    LgFbufUint8 fts1, fts2;
    bool bresult = false;

    //Read a known existing file into ts0.
    bresult = fts1.Fread(fname);
    ASSERT_TRUE(bresult);

    //Double-check the important parameters of the object.
    ASSERT_EQ(fts1.m_state, LgFbufUint8State::Occupied);
    ASSERT_EQ(fts1.m_errs, 0);
    ASSERT_TRUE(fts1.m_buf.m_n_used > 0);
    ASSERT_TRUE(fts1.m_buf.m_n_allocd >= fts1.m_buf.m_n_used);
    fname_extracted = fts1.GetFilename();
    ASSERT_EQ(fname_extracted, fname);

    //Assign the object to a second object.  This should invoke the copy assignment operator.
    fts2 = fts1;

    //Double-check the fields of the second object.  They should be identical to the first object.
    ASSERT_EQ(fts2.m_state, LgFbufUint8State::Occupied);
    ASSERT_EQ(fts2.m_errs, 0);
    ASSERT_TRUE(fts2.m_buf.m_n_used > 0);
    ASSERT_TRUE(fts2.m_buf.m_n_allocd >= fts2.m_buf.m_n_used);
    fname_extracted = fts2.GetFilename();
    ASSERT_EQ(fname_extracted, fname);
}


//Check that the move constructor seems to function.
TEST_F(LgFbufUint8Test, MoveConstructorCheck01)
{
    //Unsure how to check this.
}


//Check that the move assignment operator seems to function.
TEST_F(LgFbufUint8Test, MoveAssignmentOperatorCheck01)
{
    //Unsure how to check this.
}


//Check that the comparison function seems to function.
TEST_F(LgFbufUint8Test, ComparisonCheck01)
{
    const char* fname = "ut_LgVerbosity.cpp";
    LgFbufUint8 fts1, fts2;
    bool bresult = false;
    std::string fname_extracted;

    //Read a known existing file into ts0.
    bresult = fts1.Fread(fname);
    ASSERT_TRUE(bresult);

    //Copy to a second object.
    fts2 = fts1;

    //Double-check important parameters of the first object.  The second object is assumed identical.
    ASSERT_EQ(fts1.m_state, LgFbufUint8State::Occupied);
    ASSERT_EQ(fts1.m_errs, 0);
    ASSERT_TRUE(fts1.m_buf.m_n_used > 0);
    ASSERT_TRUE(fts1.m_buf.m_n_allocd >= fts1.m_buf.m_n_used);
    fname_extracted = fts1.GetFilename();
    ASSERT_EQ(fname_extracted, fname);

    //The objects should be identical.
    ASSERT_EQ(fts1.Cmp(fts2), 0);

    //Delete one character from the first object.  It should now not be equal to the second object.
    fts1.DeleteOne(0);
    ASSERT_TRUE(fts1.Cmp(fts2) != 0);
}


//Check that the less than operator seems to function.
TEST_F(LgFbufUint8Test, LessThanOperatorCheck01)
{
    const char* fname = "ut_LgVerbosity.cpp";
    LgFbufUint8 fts1, fts2;
    bool bresult = false;
    std::string fname_extracted;

    //Read a known existing file into ts0.
    bresult = fts1.Fread(fname);
    ASSERT_TRUE(bresult);

    //Copy to a second object.
    fts2 = fts1;

    //Double-check important parameters of the first object.  The second object is assumed identical.
    ASSERT_EQ(fts1.m_state, LgFbufUint8State::Occupied);
    ASSERT_EQ(fts1.m_errs, 0);
    ASSERT_TRUE(fts1.m_buf.m_n_used > 0);
    ASSERT_TRUE(fts1.m_buf.m_n_allocd >= fts1.m_buf.m_n_used);
    fname_extracted = fts1.GetFilename();
    ASSERT_EQ(fname_extracted, fname);

    //The objects should be identical.
    ASSERT_TRUE(fts1 == fts2);

    //fts1 should now not be less than fts2.  This verifies the false case.
    ASSERT_FALSE(fts1 < fts2);

    //In order to get the true case, empty fts1 and re-test.
    while(fts1.m_buf.Nelem())
    {
       fts1.m_buf.DeleteOne(0);
    }
    ASSERT_TRUE(fts1 < fts2);
}



//Checks the error getting and clearing functions.
TEST_F(LgFbufUint8Test, ErrGetAndClear)
{
   LgFbufUint8 ts1;
   unsigned result;

   //Attempt to read a file that presumably does not exist.
   ts1.Fread("xxqqzz98_5576.noexist");

   result = ts1.ErrsGet();

   ASSERT_EQ(result, LG_CPP_DD_FBUF_UINT8_ERR_FOPEN);

   ts1.ErrsClear();

   result = ts1.ErrsGet();

   ASSERT_EQ(result, 0);
}


//Checks file reading and writing, but happy path only.
TEST_F(LgFbufUint8Test, FreadFwriteHappyPathOnly)
{
   const char* s = "0123456789abcdefghijklmnopqrstuvwxyz-+/*ABCDEFGHIJKLMNOPQRSTUVWXYZ";
   LgBufUint8 ts1, ts2;
   LgFbufUint8 fts1, fts2;

   const size_t slen = strlen(s);
   unsigned prng = 0;
   bool bresult = false;
   size_t iter;
   //unsigned result;

   //Form a buffer with known reproducible contents.
   for (iter = 0; iter < 5000; iter++)
   {
      fts1.AppendOne(s[prng]);
      prng = (prng + 61) % slen;
   }
   
   //Dump to a file.
   bresult = fts1.Fwrite("ut_temp_FreadFwriteHappyPathOnly.txt");
   ASSERT_TRUE(bresult);

   //Read from the file to the other variable.
   bresult = fts2.Fread("ut_temp_FreadFwriteHappyPathOnly.txt");
   ASSERT_TRUE(bresult);

   ASSERT_TRUE(ts1 == ts2);
}


//Checks file reading and writing, but happy path only.
TEST_F(LgFbufUint8Test, TextFileRead)
{
    const char* fname = "ut_LgVerbosity.cpp";
    LgFbufUint8 fts1, fts2;
    bool bresult = false;

    //Read from the file to the other variable.
    bresult = fts1.Fread(fname);
    ASSERT_TRUE(bresult);

    //Do a debug dump.
    //fts1.DebugWrite(std::cout, "fts1", true);

    //Dump to a file.
    bresult = fts1.Fwrite("ut_temp_FreadFwriteHappyPathOnly.txt");
    ASSERT_TRUE(bresult);

    //Assign.
    fts2 = fts1;

    ASSERT_TRUE(fts1 == fts2);
}
