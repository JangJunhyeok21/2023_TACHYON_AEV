#include "steering.h"

steer::steer(ros::NodeHandle *nh){
    target_sub=nh->subscribe("/target_angle",10,&steer::targetSub, this);
    encode_sub=nh->subscribe("/encoder",10,&steer::encodeSub, this);
    timer=nh->createTimer(ros::Duration(0.1),&steer::steerControl,this);
}

void steer::targetSub(const std_msgs::Int32& msg){
    target_angle=msg.data;
}

void steer::encodeSub(const std_msgs::Int32& msg){
    angle=msg.data*6/steerRatio;
}

void steer::steerControl(const ros::TimerEvent& ){
    //조향은 PID제어
    float err=target_angle-angle;
    static float prev_err=0;
    float dErr=err-prev_err;
    static float I=0;
    I+=ki*err*dt;
    //세츄
    prev_err=err;
    digitalWrite(err>=0?right:left,HIGH);
    digitalWrite(err>=0?left:right,LOW);
    pwmWrite(value,abs(err*kp+I+kd*(dErr/dt)));
}

void setupWiringPi(){
    wiringPiSetupPhys();
    pinMode(value,PWM_OUTPUT);
    pinMode(right,OUTPUT);
    pinMode(left,OUTPUT);
}

int main(int argc, char**argv){
    setupWiringPi();
    //핀설정
    ros::init(argc,argv,"steeringControl_node");
    ros::NodeHandle nh;
    steer steerCon(&nh);
    ros::spin();
}