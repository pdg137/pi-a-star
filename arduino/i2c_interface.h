class I2cInterface
{
  static uint8_t *raw_data;
  static uint8_t raw_data_size;
  static void receive(int numBytes);

  public:
  static void setup(uint8_t address, uint8_t *data, uint8_t size);
};

