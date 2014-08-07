class Follow
{
  public:
  static void update();
  static void doTurn(int16_t angle_degrees);
  static void doFollow(uint32_t follow_min_distance);
  static uint8_t sensors[5];
  static int16_t pos;
  static uint8_t detected_left, detected_straight, detected_right, detected_end;
  
  static const uint8_t STATE_NONE = -1;  
  static const uint8_t STATE_WAITING = 0;
  static const uint8_t STATE_FOLLOWING = 1;
  static const uint8_t STATE_TURNING = 2;
  static const uint8_t STATE_SNAPPING = 3;
  static const uint8_t STATE_FOLLOWING_MORE = 4;
  
  static uint8_t state;
  
  private:
  static void readSensors();
  static void checkForEnd();
  static void follow();
  static void follow_more();
  static void turn();
  static void wait();
  static void snap();
  static void do_pid(uint16_t speed);
  static void check_for_intersections();
};
