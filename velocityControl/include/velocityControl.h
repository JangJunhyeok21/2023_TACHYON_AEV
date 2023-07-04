#ifndef VELOCITY_H
#define VELOCITY_H
#include "ros/ros.h"
#include "velocityControl/velocity_msg.h"
#include "velocityControl/misson_msg.h"
#include "odometer/speed_msg.h"
#include <wiringPi.h>

class velocity
{
private:
    double kph, target_speed;
    char misson;
public:
    ros::Subscriber speed_sub;
    ros::Subscriber misson_sub;
    ros::Subscriber target_sub;
    velocity(ros::NodeHandle *nh);
    void speed_control(double kph, double target);
    void speedSub(const odometer::speed_msg& msg);
    void missonSub(const velocityControl::misson_msg& msg);
    void targetSub(const velocityControl::velocity_msg& msg);
};

#endif // ODOMETER_H