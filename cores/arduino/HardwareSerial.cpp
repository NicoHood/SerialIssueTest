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

size_t HardwareSerial::write(uint8_t c)
{
  // If the buffer and the data register is empty, just write the byte
  // to the data register and be done. This shortcut helps
  // significantly improve the effective datarate at high (>
  // 500kbit/s) bitrates, where interrupt overhead becomes a slowdown.
  if (_tx_buffer_head == _tx_buffer_tail && bit_is_set(*_ucsra, UDRE0)) {
    *_udr = c;
    sbi(*_ucsra, TXC0);
    return 1;
  }

  tx_buffer_index_t i = (_tx_buffer_head + 1) % SERIAL_TX_BUFFER_SIZE;
	
  // If the output buffer is full, there's nothing for it other than to 
  // wait for the interrupt handler to empty it a bit
  while (i == _tx_buffer_tail) {
    if (bit_is_clear(SREG, SREG_I)) {
      // Interrupts are disabled, so we'll have to poll the data
      // register empty flag ourselves. If it is set, pretend an
      // interrupt has happened and call the handler to free up
      // space for us.
      if(bit_is_set(*_ucsra, UDRE0))
	_tx_udr_empty_irq();
    } else {
      // nop, the interrupt handler will free up space for us
    }
  }

  _tx_buffer[_tx_buffer_head] = c;
  _tx_buffer_head = i;
	
  sbi(*_ucsrb, UDRIE0);
  
  return 1;
}
  HardwareSerial Serial( &UCSR0A, &UCSR0B, &UDR0);




