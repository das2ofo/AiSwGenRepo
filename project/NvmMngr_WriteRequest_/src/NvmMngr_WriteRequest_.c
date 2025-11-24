#include <stdint.h>
#include <stdbool.h>
#include "NvmMngr_WriteRequest_.h"

static uint8 fifoPrelation_u8 = 0;

void NvmMngr_WriteRequest_(NvmMngr_DataPosition_t dataToWrite_,uint8 * const data_pu8)
{
  uint32 l_pageToWrite_u32 = 0;
  uint32 l_pageStartAdd_u32 = 0;
  uint32 l_dataStartAdd_u32 = 0;
  uint32  l_relativeArrPos_u32 = 0;
  uint32 l_dataLen_u32 = 0;
  uint8* l_copy_data_pu8 = data_pu8;
  l_pageToWrite_u32 = (uint32)NvmBlock_[dataToWrite_].belongPage_;
  l_pageStartAdd_u32 = NvmPageCopy_[l_pageToWrite_u32].startAddrPage_u32;
  l_dataStartAdd_u32 = NvmBlock_[dataToWrite_].addrData_u32;
  /* Position used to populate the array that rapresents the page*/
  l_relativeArrPos_u32 = l_dataStartAdd_u32-l_pageStartAdd_u32;
  l_dataLen_u32 = NvmBlock_[dataToWrite_].dataLen_u8;
  NvmPageCopy_[l_pageToWrite_u32].writeReq_b = true;
  NvmPageCopy_[l_pageToWrite_u32].writingDone_b = false;

  /* Copy the data in the pageCopy_u8 the relative position */
  for(uint32 l_iterator_u32=0;l_iterator_u32<l_dataLen_u32 ;l_iterator_u32++)
  {
    /* Write the data in the copyPage contained in ram */
    NvmPageCopy_[l_pageToWrite_u32].pageCopy_u8[l_relativeArrPos_u32+l_iterator_u32] = *l_copy_data_pu8;
    l_copy_data_pu8++;      
  }
  /* A position in the list is assigned only the first time after the page it has been copied*/
  if((uint32)0xFF==NvmPageCopy_[l_pageToWrite_u32].posListFifo_u8 )
  {
    NvmPageCopy_[l_pageToWrite_u32].posListFifo_u8=fifoPrelation_u8;
    /* Increment the position for the next page that will enter */
    fifoPrelation_u8++;
  }  
}

uint8 vectorData_u8[10] = {0};

int main()
{
  fifoPrelation_u8=0;
  NvmMngr_WriteRequest_(RESET_REASON_0, &vectorData_u8[0]);
  return 0;
}