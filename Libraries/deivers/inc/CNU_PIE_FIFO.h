/*********************************************************************************************************************
 *     COPYRIGHT NOTICE
 *     Copyright (c) 2023,CNU_W.PIE
 *     All rights reserved.
 *     本库函数参考逐飞科技开源的STC函数库
 *     除注明出处外，以下所有内容版权均属胖胖个人所有，未经允许，不得用于商业用途，
 *     修改内容时必须保留PP的版权声明。
 *     Except where indicated, the copyright of all the contents below is owned by PP 
 *     and can not be used for commercial purposes without permission. 
 *     The copyright notice of PP must be preserved when modifying the content.
 *
 * @file       CNU_PIE_FIFO.h
 * @brief      FIFO
 * @author     胖胖
 * @version    v1.0
 * @note       NULL
 * @date       2023-07-26
 ********************************************************************************************************************/
 
#ifndef __CNU_PIE_FIFO_H_
#define __CNU_PIE_FIFO_H_

#include "common.h"

typedef struct
{
    uint8_t     *buffer;                                                          // 缓存指针
    uint32_t    head;                                                             // 缓存头指针 总是指向空的缓存
    uint32_t    end;                                                              // 缓存尾指针 总是指向非空缓存（缓存全空除外）
    uint32_t    size;                                                             // 缓存剩余大小
    uint32_t    max;                                                              // 缓存总大小
}fifo_struct;

typedef enum
{
    FIFO_SUCCESS,

    FIFO_BUFFER_NULL,
    FIFO_SPACE_NO_ENOUGH,
    FIFO_DATA_NO_ENOUGH,
}fifo_state_enum;

typedef enum
{
    FIFO_READ_AND_CLEAN,
    FIFO_READ_ONLY,
}fifo_operation_enum;

fifo_state_enum   fifo_init           (fifo_struct *fifo, uint8_t *buffer_addr, uint32_t size);
void              fifo_head_offset    (fifo_struct *fifo, uint32_t offset);
void              fifo_end_offset     (fifo_struct *fifo, uint32_t offset);
void              fifo_clear          (fifo_struct *fifo);
uint32_t          fifo_used           (fifo_struct *fifo);

fifo_state_enum fifo_read_buffer    (fifo_struct *fifo, uint8_t *dat, uint32_t *length, fifo_operation_enum flag);
fifo_state_enum fifo_write_buffer   (fifo_struct *fifo, uint8_t *dat, uint32_t length);

#endif
