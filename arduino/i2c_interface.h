class I2cInterface
{
  static uint8_t *raw_data;
  static uint8_t raw_data_size;
  static uint8_t *raw_report;
  static uint8_t raw_report_size;
  static void receive(int numBytes);
  static void request();

  public:
  static void setup(uint8_t address, uint8_t *data, uint8_t data_size,
    uint8_t * report, uint8_t report_size);
};

