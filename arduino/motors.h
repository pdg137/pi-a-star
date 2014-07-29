class Motors
{
  static uint8_t left_dir_pin;
  static uint8_t right_dir_pin;
  
  public:
  static void set(int left, int right);
  static void setup(uint8_t motor_a_dir_pin, uint8_t motor_b_dir_pin);
};
