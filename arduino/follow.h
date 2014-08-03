class Follow
{
  public:
  static void update();
  static void doTurn(int16_t angle_degrees);
  static void doFollow();
  static uint8_t sensors[5];
  static int16_t pos;
  static uint8_t detected_left, detected_straight, detected_right;
  
  static const uint8_t STATE_NONE = -1;  
  static const uint8_t STATE_WAITING = 0;
  static const uint8_t STATE_FOLLOWING = 1;
  static const uint8_t STATE_TURNING = 2;
  static const uint8_t STATE_SNAPPING = 3;
  
  static uint8_t state;
  
  private:
  static void readSensors();
  static void follow();
  static void turn();
  static void wait();
  static void snap();
};
