#ifndef ODOMETER_H
#define ODOMETER_H
#include "ros/ros.h"
#include "odometer/speed_msg.h"
#include <wiringPi.h>
#define SENSOR_PIN 23
#define PPR 8
#define CIRCUL 1.527
#define ratio 55/12

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
