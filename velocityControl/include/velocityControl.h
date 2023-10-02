#ifndef VELOCITY_H
#define VELOCITY_H
#include "ros/ros.h"
#include "velocityControl/velocity_msg.h"
#include "odometer/speed_msg.h"
#include <wiringPi.h>
#include <stdlib.h>

#define accel 32
#define brake1 16
#define AS_SW 13
#define brake2 18
#define regenPin 24
#define estopPin 11
#define kp 1
#define ki 1
#define dt 0.1
#define amp 1

bool AS_SW_flag = false; // 0:m 1:a
bool estop_flag = false; // 0:no estop 1:estop

class velocity
{
private:
    double kph, target_speed;
    bool estopEng = false;

public:
    ros::Subscriber speed_sub;
    ros::Subscriber target_sub;
    velocity(ros::NodeHandle *nh);
    void estop();
    void release();
    void speed_control(double kph, double target);
    void speedSub(const odometer::speed_msg &msg);
    void targetSub(const velocityControl::velocity_msg &msg);
};

#endif // VELOCITY_H
