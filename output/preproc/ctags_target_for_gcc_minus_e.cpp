# 1 "/Users/ductin/Project/cocquay/sketch_oct30a/sketch_oct30a.ino"
# 1 "/Users/ductin/Project/cocquay/sketch_oct30a/sketch_oct30a.ino"
# 2 "/Users/ductin/Project/cocquay/sketch_oct30a/sketch_oct30a.ino" 2
# 3 "/Users/ductin/Project/cocquay/sketch_oct30a/sketch_oct30a.ino" 2
SimpleKalmanFilter bo_loc(2, 2, 1);
# 14 "/Users/ductin/Project/cocquay/sketch_oct30a/sketch_oct30a.ino"
volatile int light = 0, upTime = 0;

void run(bool direction)
{
  // Serial.print("Run");
  // Serial.println(direction);
  if (direction)
  {
    digitalWrite(10, 0x0);
    digitalWrite(11, 0x1);
  }
  else
  {
    digitalWrite(10, 0x1);
    digitalWrite(11, 0x0);
  }
}

void stop()
{
  digitalWrite(10, 0x0);
  digitalWrite(11, 0x0);
}

void setup()
{
  pinMode(8, 0x2);
  pinMode(9, 0x2);
  pinMode(10, 0x1);
  pinMode(11, 0x1);
  stop();

  Serial.begin(9600);

  __asm__ __volatile__ ("cli" ::: "memory"); // tắt ngắt toàn cục
  /* Reset Timer/Counter1 */
  (*(volatile uint8_t *)(0x80)) = 0;
  (*(volatile uint8_t *)(0x81)) = 0;
  (*(volatile uint8_t *)(0x6F)) = 0;

  /* Setup Timer/Counter1 */
  (*(volatile uint8_t *)(0x81)) |= (1 << 1) | (1 << 0); // prescale = 64
  (*(volatile uint16_t *)(0x84)) = 3036;
  (*(volatile uint8_t *)(0x6F)) = (1 << 0); // Overflow interrupt enable
  __asm__ __volatile__ ("sei" ::: "memory"); // cho phép ngắt toàn cục
}

void loop()
{
  if (light > 900)
  {
    if (digitalRead(8))
    {
      stop();
    }
    else
    {
      run(true);
    }
  }
  else
  {
    if (digitalRead(9))
    {
      stop();
    }
    else
    {
      run(false);
    }
  }
}

extern "C" void __vector_13 /* Timer/Counter1 Overflow */ (void) __attribute__ ((signal,used, externally_visible)) ; void __vector_13 /* Timer/Counter1 Overflow */ (void)
{
  upTime++;
  if (upTime > 2)
  {
    upTime = 0;
    (*(volatile uint16_t *)(0x84)) = 3036;
    light = bo_loc.updateEstimate(analogRead(A0));
    Serial.print("sensor = ");
    Serial.println(light);
  }
}
