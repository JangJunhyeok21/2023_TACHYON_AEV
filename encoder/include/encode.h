#ifndef ENCODE_H
#define ENCODE_H
#include "ros/ros.h"
#include "std_msgs/Int32.h"
#include <wiringPi.h>

#define IN_A 31
#define IN_B 35

int g_tick=0;

class encoder
{
private:
    ros::Publisher encode_pub;

public:
    ros::Timer timer;
    encoder(ros::NodeHandle *nh);
    void publish_encode(const ros::TimerEvent&);
};

#endif // ENCODE_H
