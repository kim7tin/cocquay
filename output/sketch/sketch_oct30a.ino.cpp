#include <Arduino.h>
#line 1 "/Users/ductin/Project/cocquay/sketch_oct30a/sketch_oct30a.ino"
#line 1 "/Users/ductin/Project/cocquay/sketch_oct30a/sketch_oct30a.ino"
#include <avr/interrupt.h>
#include <SimpleKalmanFilter.h>
SimpleKalmanFilter bo_loc(2, 2, 1);

#define THRESHOLD 900
#define SCALETIMER 2

#define POSA 8
#define POSB 9

#define MOTORA 10
#define MOTORB 11

volatile int light = 0, upTime = 0;

#line 16 "/Users/ductin/Project/cocquay/sketch_oct30a/sketch_oct30a.ino"
void run(bool direction);
#line 32 "/Users/ductin/Project/cocquay/sketch_oct30a/sketch_oct30a.ino"
void stop();
#line 38 "/Users/ductin/Project/cocquay/sketch_oct30a/sketch_oct30a.ino"
void setup();
#line 61 "/Users/ductin/Project/cocquay/sketch_oct30a/sketch_oct30a.ino"
void loop();
#line 16 "/Users/ductin/Project/cocquay/sketch_oct30a/sketch_oct30a.ino"
void run(bool direction)
{
  // Serial.print("Run");
  // Serial.println(direction);
  if (direction)
  {
    digitalWrite(MOTORA, LOW);
    digitalWrite(MOTORB, HIGH);
  }
  else
  {
    digitalWrite(MOTORA, HIGH);
    digitalWrite(MOTORB, LOW);
  }
}

void stop()
{
  digitalWrite(MOTORA, LOW);
  digitalWrite(MOTORB, LOW);
}

void setup()
{
  pinMode(POSA, INPUT_PULLUP);
  pinMode(POSB, INPUT_PULLUP);
  pinMode(MOTORA, OUTPUT);
  pinMode(MOTORB, OUTPUT);
  stop();

  Serial.begin(9600);

  cli(); // tắt ngắt toàn cục
  /* Reset Timer/Counter1 */
  TCCR1A = 0;
  TCCR1B = 0;
  TIMSK1 = 0;

  /* Setup Timer/Counter1 */
  TCCR1B |= (1 << CS11) | (1 << CS10); // prescale = 64
  TCNT1 = 3036;
  TIMSK1 = (1 << TOIE1); // Overflow interrupt enable
  sei();                 // cho phép ngắt toàn cục
}

void loop()
{
  if (light > THRESHOLD)
  {
    if (digitalRead(POSA))
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
    if (digitalRead(POSB))
    {
      stop();
    }
    else
    {
      run(false);
    }
  }
}

ISR(TIMER1_OVF_vect)
{
  upTime++;
  if (upTime > SCALETIMER)
  {
    upTime = 0;
    TCNT1 = 3036;
    light = bo_loc.updateEstimate(analogRead(A0));
    Serial.print("sensor = ");
    Serial.println(light);
  }
}

