static unsigned char CMD_STATUS=0;
static unsigned char CMD_NUMBER=1;
static unsigned char CMD_STATUS_LOCK=2;
static unsigned char CMD_STATUS_CALL=0;
static unsigned char CMD_STATUS_RETURN=1;

class RPiSlave
{
  public:
  /* Initialize slave; do not respond to general calls. */
  static void init(unsigned char address);
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
