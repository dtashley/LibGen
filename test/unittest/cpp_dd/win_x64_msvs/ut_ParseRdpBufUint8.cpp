#include "gtest/gtest.h"

#include "lg_parse_rdp_buf_uint8.hpp"


class LgParseRdpBufUint8Test : public testing::Test {
protected:
};


//Checks behavior, empty buffer.
TEST_F(LgParseRdpBufUint8Test, LgPrdpBu8ParseSpcTbNoEols_EmptyBufZero)
{
   LgBufUint8 ts;
   bool fn_rv;
   bool pl_rv;
   size_t out_end = 100;

   //Be sure that buffer was properly initialized, although this is an unnecessary check, given other unit tests.
   ASSERT_EQ(ts.m_n_allocd, 0);
   ASSERT_EQ(ts.m_n_used,   0);
   ASSERT_EQ(ts.m_bufptr,   nullptr);

   fn_rv = LgPrdpBu8ParseSpcTbNoEols(ts, 0, out_end, &pl_rv);

   ASSERT_FALSE(fn_rv);
   ASSERT_FALSE(pl_rv);
   ASSERT_EQ(out_end, 0);
}
