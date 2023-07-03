#ifndef ODOMETER_H
#define ODOMETER_H
#include "ros/ros.h"
#include "odometer/speed_msg.h"
#include <wiringPi.h>

class odo
{
private:
    double speed;
    int count, total_count;
    ros::Publisher speed_pub;

public:
    ros::Timer timer;
    odo(ros::NodeHandle *nh);
    double calculate_speed();
    void publish_speed(const ros::TimerEvent&);
};

#endif // ODOMETER_H
