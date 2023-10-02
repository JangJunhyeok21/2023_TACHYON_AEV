#include <ros.h>
#include <std_msgs/Int16.h>
#define steervalue 5
#define steervaluerx A2
#define IN_A 2
#define IN_B 3

uint16_t tick = 0;

ros::NodeHandle  nh;

std_msgs::Int16 msg;
ros::Publisher encode_pub("/encoder", &msg);

void ticking() {
  if (digitalRead(IN_A)) {
    tick++;
  } else {
    tick--;
  }
}

void setup()
{
  pinMode(steervalue, OUTPUT);
  pinMode(IN_A, INPUT_PULLUP);
  pinMode(IN_B, INPUT_PULLUP);
  pinMode(steervaluerx, INPUT_PULLUP);
  analogReference(EXTERNAL);
  analogWrite(steervalue, 0);
  attachInterrupt(digitalPinToInterrupt(IN_B), ticking, RISING);
  nh.initNode();
  nh.advertise(encode_pub);
}

void loop()
{
  msg.data=tick;
  encode_pub.publish(&msg);
  analogWrite(steervalue, analogRead(steervaluerx));
  nh.spinOnce();
}
