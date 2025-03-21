/*================================================================
  Copyright (c) 2021, Quectel Wireless Solutions Co., Ltd. All rights reserved.
  Quectel Wireless Solutions Proprietary and Confidential.
=================================================================*/
#ifndef QL_RINGBUFFER_H
#define  QL_RINGBUFFER_H

#include "ql_osi.h"

typedef enum{
	NO_PTR = -3, /**< Returned if null pointer was given as parameter. */
	NO_RW_DATA = -2, /**< Returned if there was no data to read in buffer. */
	NO_PLACE = -1, /**< Returned if there was no place to write data in buffer. */
	OK = 1 /**< Returned when write/read was succesfull/ */
} RingStatus_t;

/**
 * @brief Buffer handler structure.
 *
 */
typedef struct{
	size_t      size; /**< Size of buffer given in elements. */
	size_t      elementSize; /**< Size of one buffer element. */
	size_t      sizeB; /**< Size of buffer given in bytes. */
	size_t      elementsInBuffer; /**< Current count of elements in buffer. */
	UINT32      writePtr; /**< Buffer next write pointer. */
	UINT32      readPtr; /**< Buffer next read pointer. */
	UINT32      place; /**< Place available in buffer. */
	void*       buffer; /**< Pointer to array holding ring buffer. */
    ql_mutex_t  lock;
} RingBuffer_t;

/**
 * Function that returns size of whole ring buffer.
 *
 * @param buffer 	Pointer to buffer structure, which size has to be returned.
 * @retval 			Size of pointed ring buffer.
 */
extern UINT32 RingGetElementsCapacity (RingBuffer_t* buffer);
/**
 * @brief Function that returns available space in selected buffer.
 *
 * @param buffer Pointer to buffer structure, which size has to be returned.
 * @return uint32_t Available size in provided buffer.
 */
extern UINT32 RingGetSpace (RingBuffer_t* buffer);
/**
 * @brief Function that returns count of data available in buffer.
 *
 * @param buffer Pointer to buffer structure
 * @return uint32_t Data count in buffer.
 */
extern UINT32 RingGetDataCnt (RingBuffer_t* buffer);
/**
 * @brief Function to initialize selected ring buffer.
 *
 * @param buffer 	Pointer to buffer structure that has to be initialized.
 * @param arrayBuffer Pointer to array used by buffer
 * @param bufferSize Size of buffer given in elements
 * @param elementSize Size of one element
 * @return RingStatus_t
 */
extern RingStatus_t RingInit (RingBuffer_t* buffer, void* arrayBuffer, size_t bufferSize, size_t elementSize);
/**
 * @brief Function to initialize ring buffer with memory allocation
 *
 * @param buffer Pointer to buffer structure that has to be initialized.
 * @param bufferSize Size of buffer given in elements
 * @param elementSize Size of one element
 * @return RingStatus_t
 */
extern RingStatus_t RingInitAlloc (RingBuffer_t* buffer, size_t bufferSize, size_t elementSize);

extern RingStatus_t RingDeInit(RingBuffer_t* buffer);

/**
 * @brief Addes one byte to the end of buffer.
 *
 * @param buffer Pointer to buffer to write.
 * @param data Data to write.
 * @return RingStatus_t Status of write process.
 */
extern RingStatus_t RingWriteElement (RingBuffer_t* buffer, void* data);
/**
 * @brief Writes multiple bytes to buffer.
 *
 * @param buffer Buffer to write data.
 * @param data Data pointer to write.
 * @param len Length of data.
 * @return RingStatus_t Write status.
 */
extern RingStatus_t RingWriteElements (RingBuffer_t* buffer, void* data, size_t len);


extern RingStatus_t RingPeekElement (RingBuffer_t* buffer, void* data);
/**
 * @brief Reads one byte from buffer.
 *
 * @param buffer Buffer to read.
 * @param data Pointer to save data.
 * @return RingStatus_t Read status.
 */
extern RingStatus_t RingReadElement (RingBuffer_t* buffer, void* data);
/**
 * @brief Reads multiple bytes from buffer.
 *
 * @param buffer Buffer to read data.
 * @param data Pointer to write data.
 * @param len Length of data to read.
 * @return RingStatus_t Read status.
 */
extern RingStatus_t RingReadElements (RingBuffer_t* buffer, void* data, size_t len);
/**
 * @brief Returns write pointer of buffer.
 *
 * @param buffer Buffer to get pointer.
 * @return uint32_t Write pointer.
 */
extern UINT32 RingGetHead (RingBuffer_t* buffer);
/**
 * @brief Returns read pointer of buffer.
 *
 * @param buffer Buffer to get pointer.
 * @return uint32_t Read pointer.
 */
extern UINT32 RingGetTail (RingBuffer_t* buffer);
/**
 * @brief Gets last element from buffer without taking it from buffer.
 *
 * @param buffer Buffer to read.
 * @return uint32_t Last byte in buffer.
 */
extern RingStatus_t RingGetLastElement(RingBuffer_t* buffer, void* element);

#endif//QL_RINGBUFFER_H


