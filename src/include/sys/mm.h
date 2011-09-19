//  IBM_PROLOG_BEGIN_TAG
//  This is an automatically generated prolog.
//
//  $Source: src/include/sys/mm.h $
//
//  IBM CONFIDENTIAL
//
//  COPYRIGHT International Business Machines Corp. 2011
//
//  p1
//
//  Object Code Only (OCO) source materials
//  Licensed Internal Code Source Materials
//  IBM HostBoot Licensed Internal Code
//
//  The source code for this program is not published or other-
//  wise divested of its trade secrets, irrespective of what has
//  been deposited with the U.S. Copyright Office.
//
//  Origin: 30
//
//  IBM_PROLOG_END
#ifndef __SYS_MM_H
#define __SYS_MM_H

#include <stdint.h>
#include <limits.h>
#include <sys/msg.h>

#ifdef __cplusplus
extern "C"
{
#endif

/**
 * Page removal operations
 */
enum PAGE_REMOVAL_OPS
{
    RELEASE,
    FLUSH,
};

/**
 * Page permission types mapped to the same bits in the Shadow PTE
 */
enum PAGE_PERMISSIONS
{
  READ_ONLY = 0x00000001,
  WRITABLE = 0x000000002,
  EXECUTABLE = 0x00000004,
  WRITE_TRACKED = 0x00000008,
  ALLOCATE_FROM_ZERO = 0x00000010,
  NO_ALLOCATE_FROM_ZERO = 0x00000020,
  NO_ACCESS = 0x00000040,
};


/** @fn mm_alloc_block()
 *  @brief System call to allocate virtual memory block in the base segment
 *
 *  @param[in] mq - Message queue to be associated with the block
 *  @param[in] va - Base virtual address of the block to be allocated
 *  @param[in] size - Requested virtual memory size of the block
 *
 *  @return int - 0 for successful block allocation, non-zero otherwise
 */
int mm_alloc_block(msg_q_t mq,void* va,uint64_t size);

/** @fn mm_remove_pages()
 *  @brief System call to remove pages by a specified operation
 *
 *  @param[in] i_op - Page removal operation to perform
 *  @param[in] i_vaddr - Virtual address associated to page(s)
 *  @param[in] size - Size of memory to perform page removal on
 *
 *  @return int - 0 for successful page removal, non-zero otherwise
 */
int mm_remove_pages(PAGE_REMOVAL_OPS i_op, void* i_vaddr, uint64_t i_size);

/** @fn mm_set_permission()
 *  @brief System call to set the page permissions
 *
 *  @param[in] va - virtual address of the pages(s) to update permission
 *  @param[in] size - requested size of memory in bytes
 *          if size = 0 then only a single page will be updated.
 *  @param[in] access_type - Type of permission to be given to the page(s)
 *
 *  @return int - 0 for successful update of permission, non-zero otherwise
 */
int mm_set_permission(void* va, uint64_t size, uint64_t access_type);

#ifdef __cplusplus
}
#endif

/** @fs mm_icache_invalidate()
 *  @brief Invalidate the ICACHE for the given memory
 *
 *  @param[in] i_addr - Destination address
 *  @param[in] i_cpu_word_count - number of CPU_WORDs (uint64_t)
 */
void mm_icache_invalidate(void * i_addr, size_t i_cpu_word_count);


#endif
