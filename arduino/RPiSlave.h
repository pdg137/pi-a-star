class RPiSlave
{
  public:
  /* Initialize slave; do not respond to general calls. */
  static void init(unsigned char address);
  static void ack();
  static void nack();
  static unsigned char handle_event(unsigned char event, unsigned char data);
};
