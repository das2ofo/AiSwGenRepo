#ifndef NVMMNGR_WRITEREQUEST__H
#define NVMMNGR_WRITEREQUEST__H

#include <stdint.h>
#include <stdbool.h>

#define RESET_REASON_ADR 0x12005050

/** AUTOSAR-like types for clarity */
typedef int16_t  sint16;  /**< Signed 16-bit integer.  */
typedef int8_t   sint8;   /**< Signed 8-bit integer.   */
typedef uint8_t  uint8;   /**< Unsigned 8-bit integer. */
typedef uint16_t uint16;  /**< Unsigned 16-bit integer */
typedef uint32_t uint32;  /**< Unsigned 32-bit integer */


/**
 * @file NvmMngr_WriteRequest_.h
 * @brief API and structures for handling NVM write requests.
 *
 * This header defines the data structures and API used to prepare a write
 * request to Non-Volatile Memory (NVM). The function
 * @ref NvmMngr_WriteRequest_ copies user data into the RAM buffer
 * representing an NVM page, marks the page for writing, and (if needed)
 * assigns the page to the FIFO list of pages pending programming.
 *
 * **Overall goal**
 * - Determine which NVM page contains the logical data block.
 * - Compute the relative offset of the block inside that page.
 * - Copy the provided user data into the correct position in the
 *   corresponding RAM page buffer.
 * - Mark the page as needing to be written and ensure it is added to the
 *   FIFO schedule for later NVM programming.
 */

/*======================================================================*/
/*   DATA POSITION ENUMERATION                                           */
/*======================================================================*/

/**
 * @brief Logical identifier of a data block managed by the NVM manager.
 *
 * Each entry represents a logical data block mapped into a specific NVM page.
 */
typedef enum NvmMngr_DataPosition_e
{
  RESET_REASON_0 = 0,   /**< Reset reason block.      */
  CALIBRATION_1         /**< Calibration data block.  */
} NvmMngr_DataPosition_t;

/*======================================================================*/
/*   PAGE IDENTIFIER ENUMERATION                                        */
/*======================================================================*/

/**
 * @brief Logical identifier of an NVM page.
 *
 * Each page stores one or more data blocks. This enum declares the page
 * identifiers used by the NVM manager.
 */
typedef enum PageAddr_e
{
  RESET_REASON_PAGE = 0,  /**< Page containing reset reason data.      */
  CALIBRATION_PAGE,       /**< Page containing calibration data.       */
  N_PAGE_COPY             /**< Number of NVM pages managed.            */
} PageAddr_t;

/*======================================================================*/
/*   BLOCK DESCRIPTOR                                                   */
/*======================================================================*/

/**
 * @brief Descriptor of a logical NVM block.
 *
 * Each entry of @ref NvmBlock_ defines:
 * - the data length in bytes,
 * - the page to which the block belongs,
 * - the absolute NVM address of the block.
 */
typedef struct NvmBlock_s
{
  const uint8        dataLen_u8;     /**< Length of the block in bytes.            */
  const PageAddr_t   belongPage_;    /**< Page containing this block.              */
  const uint32       addrData_u32;   /**< Absolute NVM address of block start.     */
} NvmBlock_t;

/*======================================================================*/
/*   PAGE COPY STRUCTURE                                                */
/*======================================================================*/

/**
 * @brief RAM copy of an NVM page used for staging updates.
 */
typedef struct NvmPageCopy_s
{
  uint8  pageCopy_u8[128];      /**< Local copy of the page content.            */
  uint8  posListFifo_u8;        /**< Logical FIFO position for pending writes.  */
  bool   writeReq_b;            /**< Write request flag for this page.          */
  bool   writingDone_b;         /**< Indicates that NVM write has completed.    */
  uint32 startAddrPage_u32;     /**< Absolute NVM start address of the page.    */
} NvmPageCopy_t;

/* External configuration tables */


NvmBlock_t NvmBlock_[] = 
{
  /* Reset reason */
  {
    .dataLen_u8     = 5,
    .belongPage_    = RESET_REASON_PAGE,
    .addrData_u32   = RESET_REASON_ADR
  },
  /* Calibration */
  {
    .dataLen_u8     = 5,
    .belongPage_    = CALIBRATION_PAGE,
    .addrData_u32   = RESET_REASON_ADR
  }
};

NvmPageCopy_t NvmPageCopy_[] =
{
  {
    .pageCopy_u8 = {0},
    .posListFifo_u8 = 0xFF,
    .writeReq_b = false,
    .startAddrPage_u32 = RESET_REASON_ADR
  },
  {
    .pageCopy_u8 = {0},
    .posListFifo_u8 = 0xFF,
    .writeReq_b = false,
    .startAddrPage_u32 = RESET_REASON_ADR
  }
};

/*======================================================================*/
/*   MAIN API                                                           */
/*======================================================================*/

/**
 * @brief Prepare a write request for a logical NVM block.
 *
 * @details
 * **Purpose of the function**
 *
 * This function prepares the write of a logical NVM block by modifying the
 * RAM copy of the page that contains the block. No physical write to NVM is
 * performed here. Instead, the page is marked as "pending write", and if the
 * page has not yet been added to the FIFO queue of pending pages, it is
 * assigned the next available FIFO position.
 *
 *
 * @par Interface summary
 *
* | Interface                         | In | Out | Data type             | Param | Data factor | Data offset | Data size | Data range        | Data unit |
* |-----------------------------------|:--:|:---:|-----------------------|:-----:|------------:|------------:|----------:|-------------------|-----------|
* | NvmBlock_[].belongPage_           | ✓  |     | PageAddr_t            |   -   |           1 |           0 |         1 | [0, N_PAGE_COPY]  | [-]       |
* | NvmPageCopy_[].startAddrPage_u32  | ✓  |     | uint32                |   -   |           1 |           0 |         1 | [0, UINT32_MAX]   | [-]       |
* | NvmBlock_[].addrData_u32          | ✓  |     | uint32                |   -   |           1 |           0 |         1 | [0, UINT32_MAX]   | [-]       |
* | data_pu8                          | ✓  |     | uint8*                |   -   |           1 |           0 |         1 | [0, UINT8_MAX]    | [-]       |
* | NvmBlock_[].dataLen_u8            | ✓  |     | uint8                 |   -   |           1 |           0 |         1 | [0, UINT8_MAX]    | [-]       |
* | dataToWrite_                      | ✓  |     | NvmMngr_DataPosition_t|   -   |           1 |           0 |         1 | [0, 20]           | [-]       |
* | NvmPageCopy_[].writeReq_b         |    | ✓   | bool                  |   -   |           1 |           0 |         1 | [0, 1]            | [-]       |
* | NvmPageCopy_[].posListFifo_u8     |    | ✓   | uint8                 |   -   |           1 |           0 |         1 | [0, UINT8_MAX]    | [-]       |
* | NvmPageCopy_[].pageCopy_u8        |    | ✓   | uint8                 |   -   |           1 |           0 |       128 | [0, UINT8_MAX]    | [-]       |

 *
 * @par Activity diagram (PlantUML)
 *
 * @startuml
 * start
 *
 * :Determine the NVM page associated\nwith the selected block.\n
 * NVM_PAGE = NvmBlock_[dataToWrite_].belongPage_;
 *
 * :Read the start address of NVM_PAGE\nfrom the RAM page copy structure;\n
 * PAGE_START_ADDRESS = NvmPageCopy_[NVM_PAGE].startAddrPage_u32;
 *
 * :Read the absolute NVM address of the block\nfrom the block descriptor;\n
 * BLOCK_START_ADDRESS = NvmBlock_[dataToWrite_].addrData_u32;
 *
 * :Compute the relative offset inside the page:\n
 * REL_OFFSET = BLOCK_START_ADDRESS - PAGE_START_ADDRESS;
 *
 * :Read the block data length;\n
 * LEN = NvmBlock_[dataToWrite_].dataLen_u8;
 *
 * :Mark the page as pending write;\n
 * NvmPageCopy_[NVM_PAGE].writeReq_b = true;\n
 * NvmPageCopy_[NVM_PAGE].writingDone_b = false;
 *
 * :Copy user data into the RAM page buffer:\n
 * write LEN bytes starting at REL_OFFSET\ninside NvmPageCopy_[NVM_PAGE].pageCopy_u8[];
 * if (0xFF==NvmPageCopy_[NVM_PAGE].posListFifo_u8?) then (yes)
 *   :Assign next available FIFO slot to the page;\n
 *   posListFifo_u8 = fifoPrelation_u8;\n
 *   fifoPrelation_u8++;
 * endif
 *
 * stop
 * @enduml
 *
 * @param[in] dataToWrite_  Logical block identifier to be updated.
 * @param[in] data_pu8      Pointer to the user data to write into the NVM page buffer.
 *
 * @note This function only updates the RAM shadow copy of the NVM page.
 *       The actual write to physical NVM is handled elsewhere.
 */
void NvmMngr_WriteRequest_(NvmMngr_DataPosition_t dataToWrite_, uint8 * const data_pu8);

#endif /* NVMMNGR_WRITEREQUEST__H */
