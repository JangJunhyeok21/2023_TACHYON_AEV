#ifndef STEERING_H
#define STEERING_H
#include "ros/ros.h"
#include "std_msgs/Int32.h"
#include <wiringPi.h>

#define value 11
#define right 22
#define left 33
#define kp 1
#define ki 1
#define kd 1
#define dt 0.01
#define steerRatio 4.0

class steer
{
private:
    int target_angle, angle;
public:
    ros::Timer timer;
    steer(ros::NodeHandle *nh);
    ros::Subscriber target_sub;
    ros::Subscriber encode_sub;
    void targetSub(const std_msgs::Int32& msg);
    void encodeSub(const std_msgs::Int32& msg);
    void steerControl(int angle, int target);
};

#endif //STEERING_H