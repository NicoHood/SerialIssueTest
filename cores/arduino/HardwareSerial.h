/*
  HardwareSerial.h - Hardware serial library for Wiring
  Copyright (c) 2006 Nicholas Zambetti.  All right reserved.

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA

  Modified 28 September 2010 by Mark Sproul
  Modified 14 August 2012 by Alarus
  Modified 3 December 2013 by Matthijs Kooijman
*/

#ifndef HardwareSerial_h
#define HardwareSerial_h

#include <inttypes.h>
#include <stdlib.h>
#include <avr/io.h>

#define SERIAL_TX_BUFFER_SIZE 64

typedef uint8_t tx_buffer_index_t;


class HardwareSerial //: public Stream
{
  protected:
    volatile uint8_t * const _ucsra;
    volatile uint8_t * const _ucsrb;
    volatile uint8_t * const _udr;

    volatile tx_buffer_index_t _tx_buffer_head;
    volatile tx_buffer_index_t _tx_buffer_tail;

    // Don't put any members after these buffers, since only the first
    // 32 bytes of this struct can be accessed quickly using the ldd
    // instruction.

    unsigned char _tx_buffer[SERIAL_TX_BUFFER_SIZE];

  public:
    inline HardwareSerial(
  
      volatile uint8_t *ucsra, volatile uint8_t *ucsrb,
     volatile uint8_t *udr):
    _ucsra(ucsra), _ucsrb(ucsrb), 
    _udr(udr),
    _tx_buffer_head(0), _tx_buffer_tail(0)
{
}

  size_t write(uint8_t);
   void _tx_udr_empty_irq(void);
};
extern HardwareSerial Serial;


#endif
