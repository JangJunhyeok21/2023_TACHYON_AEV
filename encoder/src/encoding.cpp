#include "encode.h"


encoder::encoder(ros::NodeHandle *nh){
    encode_pub=nh->advertise<std_msgs::Int32>("/encoder",10);
    timer=nh->createTimer(ros::Duration(0.01),&encoder::publish_encode,this);
}

void encoder::publish_encode(const ros::TimerEvent& ){
    std_msgs::Int32 msg;
    msg.data=g_tick;
    encode_pub.publish(msg);
}

void ticking(){
    if(digitalRead(IN_A)){
        //cw
        g_tick++;
    }else{
        //ccw
        g_tick--;
    }
}

void setupWiringPi(){
    wiringPiSetupPhys();
    pinMode(IN_A,INPUT);
    pinMode(IN_B,INPUT);
}

int main(int argc, char**argv){
    setupWiringPi();
    ros::init(argc,argv,"encode_node");
    ros::NodeHandle nh;
    encoder encoder(&nh);
    wiringPiISR(IN_B,INT_EDGE_RISING,&ticking); //sensing
    ros::spin();
}
