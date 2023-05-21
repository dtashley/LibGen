// unittest_lu_ccm_fio.cpp : Unit tests for functions in "lu_ccm_fio.c".
//
#include <stdio.h>

#include "lu_ccm_fio.h"


int unittest_lu_ccm_fio(void)
{
   LU_CCM_stream_repchar(stdout, 'X', 42);

   return 0;
}