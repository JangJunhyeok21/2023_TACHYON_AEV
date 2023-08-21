#ifndef STEERING_H
#define STEERING_H
#include "ros/ros.h"
#include "std_msgs/Int32.h"
#include "steerControl/steering_msg.h"
#include <wiringPi.h>

#define value 11
#define right 22
#define left 33
#define IN_A 44
#define IN_B 55
#define kp 55

static int g_tick;

class steer
{
private:
    int target_angle, angle;
public:
    steer(ros::NodeHandle *nh);
    ros::Subscriber target_sub;
    void targetSub(const std_msgs::Int32& msg);
    int steerControl(int angle, int target);
}

#endif //STEERING_H