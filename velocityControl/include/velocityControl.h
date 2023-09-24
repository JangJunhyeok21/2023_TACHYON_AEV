#ifndef VELOCITY_H
#define VELOCITY_H
#include "ros/ros.h"
#include "velocityControl/velocity_msg.h"
#include "odometer/speed_msg.h"
#include <wiringPi.h>
#include <stdlib.h>

#define value 32
#define brakePin 16 //arduino boolean signal 0:release 1:engaged
#define AS_SW 13
#define accelPin 18
#define regenPin 24
#define estopPin 11
//#define steer_relay 66 //조향 모터드라이브 릴레이 신호선 -> as-sw에 연결하여 자율주행 모드일 때 자동으로 붙음.
#define kp 1
#define amp 100

bool AS_SW_flag=false; //0:m 1:a
bool estop_flag=false; //0:no estop 1:estop

class velocity
{
private:
    double kph, target_speed, accel;
    bool estopEng=0;
public:
    ros::Subscriber speed_sub;
    ros::Subscriber target_sub;
    velocity(ros::NodeHandle *nh);
    void speed_control(double kph, double target);
    void speedSub(const odometer::speed_msg& msg);
    void targetSub(const velocityControl::velocity_msg& msg);
};

#endif // VELOCITY_H
