#include "velocityControl.h"

velocity::velocity(ros::NodeHandle *nh)
{
    kph = 0;
    target_speed = 0;
    speed_sub = nh->subscribe("/speed", 10, &velocity::speedSub, this);
    target_sub = nh->subscribe("/target_velocity", 10, &velocity::targetSub, this);
}
void velocity::speed_control(double speed, double target)
{ // 목표속도 P제어
    // P control
    float err = target - speed;
    if (err >= 0)
    {
        digitalWrite(accelPin, HIGH);
        digitalWrite(regenPin, LOW);
        pwmWrite(value, abs(err * kp) + 200); // 기본 200깔고 가기
    }
    else
    {
        digitalWrite(accelPin, LOW);
        digitalWrite(regenPin, HIGH);
        pwmWrite(value, abs(err * kp) / 5); // 회생제동량은 가속에 비해 10%
    }
}

void release()
{
    digitalWrite(brakePin, LOW);
    digitalWrite(accelPin, LOW);
    digitalWrite(regenPin, LOW);
    pwmWrite(value, 0);
}
void estop()
{
    ROS_INFO("E-STOP!!");
    digitalWrite(brakePin, HIGH);
    digitalWrite(accelPin, LOW);
    digitalWrite(regenPin, HIGH);
    pwmWrite(value, 1024);
}
// 미션에 따른 종방향제어는 상위에 맡기고 속도와 긴급정지만 판단한다.
void velocity::speedSub(const odometer::speed_msg &msg)
{
    ROS_INFO("KPH: %fkm/h, Trip: %fm", msg.kph, msg.odo);
    kph = msg.kph;
    if (AS_SW_flag && !estop_flag)
    { // 자율주행 모드가 켜지고 Estop이 해제되어 있을 때
        speed_control(kph, target_speed);
    }
    else if (estop_flag)
    {   //상위제어 혹은 스위치로 estop 활성화 되어있을 때
        estop();
    }
    else
    {   //이도 저도 아닐때
        release();
    }
}
void velocity::targetSub(const velocityControl::velocity_msg &msg)
{
    target_speed = msg.targetKph;
    estop_flag = msg.estopEng;
}

void estop_switching()
{
    estop_flag = digitalRead(estopPin);
}
void AS_switching()
{
    AS_SW_flag = digitalRead(AS_SW);
}

void setupWiringPi()
{
    wiringPiSetupPhys();
    pinMode(value, PWM_OUTPUT);
    pinMode(accelPin, OUTPUT);
    pinMode(regenPin, OUTPUT);
    pinMode(brakePin, OUTPUT);
    pinMode(AS_SW, INPUT);
    pinMode(estopPin, INPUT);
}

int main(int argc, char **argv)
{
    setupWiringPi();
    ros::init(argc, argv, "velCon_node");
    ros::NodeHandle nh;
    velocity velCon(&nh);
    wiringPiISR(estopPin, INT_EDGE_BOTH, &estop_switching);
    wiringPiISR(AS_SW, INT_EDGE_BOTH, &AS_switching);
    ros::spin();
}
