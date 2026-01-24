#include "gtest/gtest.h"

#include "lg_verbosity.hpp"


class LgVerbosity : public testing::Test {
  protected:
  };


//Ensures that the default value is 3.
TEST_F(LgVerbosity, DefaultValue3)
{
    unsigned retrieved_verbosity = 99;

    retrieved_verbosity = LgVerbosity_VerbosityGet();

    ASSERT_EQ(retrieved_verbosity, 3);
}


//Ensures that the value of 0 sticks.
TEST_F(LgVerbosity, SetAndRetrieve0)
{
   unsigned retrieved_verbosity = 99;

   LgVerbosity_VerbositySet(0);

   retrieved_verbosity = LgVerbosity_VerbosityGet();

   ASSERT_EQ(retrieved_verbosity, 0);
}


//Ensures that the value of 1 sticks.
TEST_F(LgVerbosity, SetAndRetrieve1)
{
    unsigned retrieved_verbosity = 99;

    LgVerbosity_VerbositySet(1);

    retrieved_verbosity = LgVerbosity_VerbosityGet();

    ASSERT_EQ(retrieved_verbosity, 1);
}


//Ensures that the value of 2 sticks.
TEST_F(LgVerbosity, SetAndRetrieve2)
{
    unsigned retrieved_verbosity = 99;

    LgVerbosity_VerbositySet(2);

    retrieved_verbosity = LgVerbosity_VerbosityGet();

    ASSERT_EQ(retrieved_verbosity, 2);
}


//Ensures that the value of 5 sticks.
TEST_F(LgVerbosity, SetAndRetrieve5)
{
    unsigned retrieved_verbosity = 99;

    LgVerbosity_VerbositySet(5);

    retrieved_verbosity = LgVerbosity_VerbosityGet();

    ASSERT_EQ(retrieved_verbosity, 5);
}


//Ensures that the value of 8 sticks.
TEST_F(LgVerbosity, SetAndRetrieve8)
{
    unsigned retrieved_verbosity = 99;

    LgVerbosity_VerbositySet(8);

    retrieved_verbosity = LgVerbosity_VerbosityGet();

    ASSERT_EQ(retrieved_verbosity, 8);
}


//Ensures that the value of 9 sticks.
TEST_F(LgVerbosity, SetAndRetrieve9)
{
    unsigned retrieved_verbosity = 99;

    LgVerbosity_VerbositySet(9);

    retrieved_verbosity = LgVerbosity_VerbosityGet();

    ASSERT_EQ(retrieved_verbosity, 9);
}


//Ensures that the value of 10 is clipped.
TEST_F(LgVerbosity, SetAndRetrieve10)
{
    unsigned retrieved_verbosity = 99;

    LgVerbosity_VerbositySet(10);

    retrieved_verbosity = LgVerbosity_VerbosityGet();

    ASSERT_EQ(retrieved_verbosity, 9);
}


//Ensures that the value of 257 is clipped.
TEST_F(LgVerbosity, SetAndRetrieve257)
{
    unsigned retrieved_verbosity = 99;

    LgVerbosity_VerbositySet(3);
    LgVerbosity_VerbositySet(257);

    retrieved_verbosity = LgVerbosity_VerbosityGet();

    ASSERT_EQ(retrieved_verbosity, 9);
}
