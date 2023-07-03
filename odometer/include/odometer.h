#ifndef ODOMETER_H
#define ODOMETER_H
#include "ros/ros.h"
#include "odometer/speed_msg.h"

class odo
{
private:
    double speed;
    int count, total_count;
    ros::Publisher speed_pub;

public:
    odo(ros::nodeHandle *nh);
    double calculate_speed();
    void publish_speed();
};

#endif // ODOMETER_H
