#define steervaluerx A2
#define steervalue 5

void setup() {
  pinMode(steervaluerx, INPUT);
  pinMode(steervalue, OUTPUT);

  analogReference(EXTERNAL);
  analogWrite(steervalue, 0);
}

void loop() {
  analogWrite(steervalue, analogRead(steervaluerx));
}
