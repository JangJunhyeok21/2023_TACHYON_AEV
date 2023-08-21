#include "steering.h"

steer::steer(ros::NodeHandle &nh){
    target_sub=nh->subscribe("/target_angle",10,&steer::targetSub, this);
}

void steer::targetSub(const std_msgs::Int32& msg){
    target_angle=msg.angle;
    steerControl(angle,target_angle);
}

int steer::steerControl(int angle, int target){
    //조향은 PID제어
    float err=target-angle;
    digitalWrite(err>=0?right:left,HIGH);
    digitalWrite(err>=0?left:right,LOW);
    pwmWrite(value,abs(err*kp));
}
void setupWiringPi(){
    wiringPiSetup();
    pinMode(value,PWM_OUTPUT);
    pinMode(IN_A,INPUT);
    pinMode(IN_B,INPUT);
    pinMode(right,OUTPUT);
    pinMode(left,OUTPUT);
}
int main(int argc, char**argv){
    setupWiringPi();
    //핀설정
    ros::init(argc,argv,"steeringControl_node");
    ros::NodeHandle nh;
    steer steerCon(&nh);
    //인터럽트 설정
    ros::spin();
}