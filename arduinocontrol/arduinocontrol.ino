#define steervaluerx A2
#define steervalue 5

void setup() {
  pinMode(steervaluerx, INPUT);
  pinMode(steervalue, OUTPUT);
<<<<<<< HEAD
=======

>>>>>>> 051f893260dbc37a2c8f755e7bf9ecb6db10a398
  analogReference(EXTERNAL);
  analogWrite(steervalue, 0);
}

void loop() {
  analogWrite(steervalue, analogRead(steervaluerx));
}
