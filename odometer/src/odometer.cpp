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
    speed=count*0.7205;
    count=0;    
    return speed;
}

void odo::publish_speed(const ros::TimerEvent& ){
    //std_msgs::Float64 msg;
    odometer::speed_msg msg;
    msg.kph=calculate_speed();
    msg.odo=total_count/36.0*1.441;
    speed_pub.publish(msg);
}
int odo::count_up(){
    count++;
}

int main(int argc, char**argv){
    wiringPiSetup();
    pinMode(SENSOR_PIN,INPUT);
    ros::init(argc,argv,"odo_node");
    ros::NodeHandle nh;
    odo odometer(&nh);
    wiringPiISR(SENSOR_PIN,INT_EDGE_RISING,&odometer.count_up)
    ros::spin();
}
