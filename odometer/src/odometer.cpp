#include "odometer.h"

odo::odo(ros::NodeHandle *nh){
    speed=0;
    g_count=0;
    total_count=0;
    speed_pub=nh->advertise<odometer::speed_msg>("/speed",10);
    timer=nh->createTimer(ros::Duration(0.5),&odo::publish_speed,this);
}

double odo::calculate_speed(){
    total_count+=g_count; //refresh odom
    speed=g_count*0.7205; //count to KPH
    g_count=0;    
    return speed;
}

void odo::publish_speed(const ros::TimerEvent& ){
    odometer::speed_msg msg;
    msg.kph=calculate_speed();
    msg.odo=total_count/36.0*1.441;
    speed_pub.publish(msg);
}
void count_up(){
    g_count++;
}

int main(int argc, char**argv){
    wiringPiSetup();
    pinMode(SENSOR_PIN,INPUT);
    ros::init(argc,argv,"odo_node");
    ros::NodeHandle nh;
    odo odometer(&nh);
    wiringPiISR(SENSOR_PIN,INT_EDGE_RISING,&count_up); //sensing
    ros::spin();
}
