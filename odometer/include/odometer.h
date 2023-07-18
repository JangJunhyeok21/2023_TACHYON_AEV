#ifndef ODOMETER_H
#define ODOMETER_H
#include "ros/ros.h"
#include "odometer/speed_msg.h"
#include <wiringPi.h>
#define SENSOR_PIN 23

class odo
{
private:
    float speed;
    int count, total_count;
    ros::Publisher speed_pub;

public:
    ros::Timer timer;
    odo(ros::NodeHandle *nh);
    double calculate_speed();
    void publish_speed(const ros::TimerEvent&);
    int count_up();
};

#endif // ODOMETER_H
