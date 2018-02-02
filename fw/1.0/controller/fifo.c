///////////////////////////////////////////////////////////////////////////////
//
// Simple Fifo implementation
//
// by Alvaro Prieto
//
///////////////////////////////////////////////////////////////////////////////
#include "fifo.h"
#include "stm32f0xx.h"

//
// Set up fifo struct (Buffer size must be a power of 2)
//
int32_t FifoInit(fifo_t *fifo, uint32_t size, void *buff) {
  if (!(size && !(size & (size - 1)))) {
    // ERROR - Fifo size not a power of 2
    return -1;
  }

  fifo->sizeMask = size - 1;
  fifo->buff = (uint8_t *)buff;
  fifo->start = 0;
  fifo->end = 0;

  return 0;
}

//
// Return number of bytes in fifo
//
uint32_t FifoSize(fifo_t *fifo) {
  return (fifo->end - fifo->start) & fifo->sizeMask;
}

//
// Push character into fifo
//
uint8_t FifoPush(fifo_t *fifo, uint8_t byte) {
  uint8_t rval = 0;

  __disable_irq();

  fifo->buff[fifo->end++] = byte;
  fifo->end &= fifo->sizeMask;

  // If start==end, we've looped around and will start dropping characters
  if (fifo->start == fifo->end) {
    fifo->start = (fifo->start + 1) & fifo->sizeMask;
    rval = 1;
  }

  __enable_irq();

  return rval;
}

//
// Pop character from fifo
//
uint8_t FifoPop(fifo_t *fifo) {
  uint8_t byte = 0;

  __disable_irq();

  // Make sure the fifo is not empty
  if (fifo->start != fifo->end) {
    byte = fifo->buff[fifo->start++];
    fifo->start &= fifo->sizeMask;
  }

  __enable_irq();

  return byte;
}

//
// Peek into the fifo
//
uint8_t FifoPeek(fifo_t *fifo, uint32_t byte) {
  return fifo->buff[(fifo->start + byte) & fifo->sizeMask];
}
