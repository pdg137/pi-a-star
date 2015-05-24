typedef void (*callback_taking_byte)(unsigned char b);
typedef unsigned char (*callback_returning_byte)();
typedef void (*callback_no_data)();


class FastSlaveTWI
{
  public:
  /* Initialize slave; do not respond to general calls. */
  static void init(unsigned char address, unsigned char rpi_delay_us,
    callback_taking_byte slave_receive_byte,
    callback_returning_byte slave_transmit_byte,
    callback_no_data slave_start,
    callback_no_data slave_stop);
  static void ack();
  static void nack();
  static unsigned char handle_event(unsigned char event);
  static unsigned char getByte(unsigned char index);
  static void setByte(unsigned char index, unsigned char value);
  
  /* Check for a command and return the command # if we are being called. */
  static unsigned char checkForCommand();
  
  /* Indicate that we are done processing a command. */
  static void commandReturn();
};
