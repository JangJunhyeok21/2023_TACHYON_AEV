#define forwardrx 12
#define backwardrx 11
#define velvaluerx A1
#define brakerx 4
#define brakeIN1 9
#define brakeIN2 10
#define accelPin 3
#define decelPin 6
#define steervaluerx A2
#define steervalue 5

void setup() {
  pinMode(steervaluerx, INPUT);
  pinMode(steervalue, OUTPUT);

  pinMode(brakerx, INPUT_PULLUP);
  pinMode(brakeIN1, OUTPUT);
  pinMode(brakeIN2, OUTPUT);

  pinMode(accelPin, OUTPUT);
  pinMode(decelPin, OUTPUT);
  pinMode(forwardrx, INPUT_PULLUP);
  pinMode(backwardrx, INPUT_PULLUP);
  pinMode(velvaluerx, INPUT);


  analogReference(EXTERNAL);
  digitalWrite(brakeIN1, LOW);
  digitalWrite(brakeIN2, LOW);
  analogWrite(accelPin, 0);
  analogWrite(decelPin, 0);
  analogWrite(steervalue, 0);
}

void loop() {
  const bool brake_flag = 0;

  analogWrite(steervalue, analogRead(steervaluerx));

  if (digitalRead(brakerx)) {
    if (!brake_flag) {
      digitalWrite(brakeIN1, HIGH);
      delay(1500);
      digitalWrite(brakeIN2, HIGH);
    }
  } else {
    if (brake_flag) {
      digitalWrite(brakeIN1, LOW);
      delay(1500);
      digitalWrite(brakeIN2, LOW);
    }
  }
  if (digitalRead(forwardrx)) {
    analogWrite(accelPin, analogRead(velvaluerx));
  }else if (digitalRead(backwardrx)) {
    analogWrite(decelPin, analogRead(velvaluerx));
  }
}
