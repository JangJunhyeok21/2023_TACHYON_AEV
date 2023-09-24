#ifndef ODOMETER_H
#define ODOMETER_H
#include "ros/ros.h"
#include "odometer/speed_msg.h"
#include <wiringPi.h>
#define SENSOR_PIN 23
#define PPR 32 //Hall 8 * ratio 4

static int g_count;

class odo
{
private:
    float speed;
    int total_count;
    ros::Publisher speed_pub;

public:
    ros::Timer timer;
    odo(ros::NodeHandle *nh);
    double calculate_speed();
    void publish_speed(const ros::TimerEvent&);
};

#endif // ODOMETER_H
