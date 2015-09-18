#include "HardwareSerial.h"

#ifndef cbi
#define cbi(sfr, bit) (_SFR_BYTE(sfr) &= ~_BV(bit))
#endif
#ifndef sbi
#define sbi(sfr, bit) (_SFR_BYTE(sfr) |= _BV(bit))
#endif


// Actual interrupt handlers //////////////////////////////////////////////////////////////

void HardwareSerial::_tx_udr_empty_irq(void)
{
 sbi(*_ucsra, TXC0);
}

void HardwareSerial::write(uint8_t c)
{
  // If the buffer and the data register is empty, just write the byte
  // to the data register and be done. This shortcut helps
  // significantly improve the effective datarate at high (>
  // 500kbit/s) bitrates, where interrupt overhead becomes a slowdown.
  if (_tx_buffer_head == _tx_buffer_tail && bit_is_set(*_ucsra, UDRE0)) {
    *_udr = c;
    sbi(*_ucsra, TXC0);
    return;
  }

  while (0== _tx_buffer_tail) {
    _tx_udr_empty_irq();
  }
  _tx_buffer[_tx_buffer_head] = c;
  
  return;
}

HardwareSerial Serial( &UCSR0A, &UCSR0B, &UDR0);




