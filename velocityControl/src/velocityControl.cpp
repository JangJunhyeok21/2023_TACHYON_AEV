#include "velocityControl.h"
#include <iostream>

using namespace std;


velocity::velocity(ros::NodeHandle *nh){
    kph=0;
    target_speed=0;
    speed_sub=nh->subscribe("/speed",10,&velocity::speedSub, this);
    target_sub=nh->subscribe("/target_velocity",10,&velocity::targetSub, this);
}
void velocity::speed_control(double speed, double target){ //목표속도 P제어
    //P control
    float err=target-speed;
    if(err>=0){
        digitalWrite(accelPin,HIGH);
        digitalWrite(regenPin,LOW);
        pwmWrite(value,abs(err*kp)/60*1024) ;
        // pwmWrite(value,0);
	    // ROS_INFO("cmd,%f",abs(err*kp));
        //std::cout << abs(err*kp)/60*1024<<endl; 

        // ROS_INFO(abs(err*kp));
    }else{
        digitalWrite(accelPin,LOW);
        digitalWrite(regenPin,HIGH);
        // pwmWrite(value,0);
        pwmWrite(value,abs(err*kp)/5);
	    // ROS_INFO("err,%f",abs(err*kp));  //회생제동량은 가속에 비해 10%
        // ROS_INFO(abs(err*kp)/5);
        // pwmWrite(value,0);
    }
}
//미션에 따른 종방향제어는 상위에 맡기고 속도와 긴급정지만 판단한다.
//조향모터와 클러치에 릴레이 사용하는걸 생각하며 설계. -> 하드웨어적 연결로 해결(as_sw에 5V직결)
void velocity::speedSub(const odometer::speed_msg& msg){
    // ROS_INFO("KPH: %fkm/h, Trip: %fm",msg.kph, msg.odo);
    kph=msg.kph;

    ROS_INFO("%d,%d",digitalRead(AS_SW), digitalRead(estopPin));

    // if(AS_SW_flag && !estop_flag){ //자율주행 모드가 켜지고 Estop이 해제되어 있을 때
    speed_control(kph,target_speed);
    // }
}
void velocity::targetSub(const velocityControl::velocity_msg& msg){
    target_speed=msg.targetKph;
}

void release(){
    digitalWrite(brakePin,LOW);
    digitalWrite(accelPin,LOW);
    digitalWrite(regenPin,LOW);
    pwmWrite(value,0);
}

void estop(){
    estop_flag=digitalRead(estopPin);
    if(estop_flag){
        ROS_INFO("E-STOP!!");
        digitalWrite(brakePin,HIGH);
        digitalWrite(accelPin,LOW);
        digitalWrite(regenPin,HIGH);
        pwmWrite(value,1024);
    }else{
        // ROS_INFO("E-STOP realese");
        release();
    }
}
void AS_switching(){
    AS_SW_flag=digitalRead(AS_SW);
    if(AS_SW_flag){
        ROS_INFO("AS-ON MODE");
    }else{
        ROS_INFO("AS-OFF MODE");
        if(!estop_flag) release();
    }
}

void setupWiringPi(){
    wiringPiSetupPhys();
    pinMode(value,PWM_OUTPUT);
    pinMode(accelPin,OUTPUT);
    pinMode(regenPin,OUTPUT);
    pinMode(brakePin,OUTPUT);
    pinMode(AS_SW,INPUT);
    pinMode(estopPin,INPUT);
}

int main(int argc, char**argv){
    setupWiringPi();
    ros::init(argc,argv,"velCon_node");
    ros::NodeHandle nh;
    velocity velCon(&nh);
    wiringPiISR(estopPin,INT_EDGE_BOTH,&estop);
    wiringPiISR(AS_SW,INT_EDGE_BOTH,&AS_switching);
    ros::spin();
}
