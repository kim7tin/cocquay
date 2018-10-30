
int lightValue = 0;
void setup() {
  pinMode(8, INPUT_PULLUP);
  pinMode(8, INPUT_PULLUP);
  pinMode(10, OUTPUT);
  pinMode(11, OUTPUT);
  digitalWrite(10, LOW);
  digitalWrite(11, LOW);

  Serial.begin(9600);
}

void loop() {
  lightValue = analogRead(A0);
  Serial.print("sensor = ");
  Serial.println(lightValue);
  delay(1000);
}
