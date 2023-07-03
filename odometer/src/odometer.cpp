#include "odometer.h"

odo::odo(ros::NodeHandle *nh){
    speed=0;
    count=0;
    total_count=0;
    speed_pub=nh->advertise<odometer::speed_msg>("/speed",10);
    timer=nh->createTimer(ros::Duration(0.5),&odo::publish_speed,this);
}

double odo::calculate_speed(){
    total_count+=count;
    count=0;
    return count*0.7205;
}

void odo::publish_speed(const ros::TimerEvent& ){
    //std_msgs::Float64 msg;
    odometer::speed_msg msg;
    msg.kph=calculate_speed();
    msg.odo=total_count/36*1.441;
    speed_pub.publish(msg);
}

int main(int argc, char**argv){
    wiringPiSetup();
    pinMode(26,OUTPUT);
    digitalWrite(26,HIGH);
    bool flag=0;
    ros::init(argc,argv,"odo_node");
    ros::NodeHandle nh;
    odo odometer(&nh);
    /*while (1) {
        if(){
            odometer.count++;
            flag=!flag;
        }
        else if(){
            flag=!flag;
        }
    }*/
    ros::spin();
}
