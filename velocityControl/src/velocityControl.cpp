#include "velocityControl.h"

#define value 12 //BCM18(pwm0) || 32_BCM12(pwm0)
#define brakePin 22 //BCM
#define AS_SW 33 //BCM
#define forward 44
#define backward 55
#define kp 80

velocity::velocity(ros::NodeHandle *nh){
    kph=0;
    target_speed=0;
    misson='m'; //a-accel b-brake c-complement e-Estop m-Manual
    speed_sub=nh->subscribe("/speed",10,&velocity::speedSub, this);
    misson_sub=nh->subscribe("/misson",10,&velocity::missonSub, this);
    target_sub=nh->subscribe("/velocity",10,&velocity::targetSub, this);
}

void velocity::speed_control(double speed, double target){ //목표속도 P제어
    //P control
    float err=target-speed;
    digitalWrite(err>=0?forward:backward,HIGH);
    digitalWrite(err>=0?backward:forward,LOW);
    pwmWrite(value,err*kp);
}

void velocity::speedSub(const odometer::speed_msg& msg){
    ROS_INFO("KPH: %f, Trip: %f",msg.kph, msg.odo);
    kph=msg.kph;
    digitalWrite(brakePin,LOW);
    if(!digitalRead(AS_SW)){
        misson='m';
        ROS_INFO("AS-OFF MODE");
        digitalWrite(forward,LOW);
        digitalWrite(backward,LOW);
        pwmWrite(amount,0);
    }
    else
        switch(misson){
            case 'a':
                speed_control(kph,80);
                break;
            case 'b':
                speed_control(kph,40);
                break;
            case 'c':
                speed_control(kph,target_speed);
                break;
            case 'e':
                ROS_INFO("E-STOP!!");
                speed_control(kph,0);
                digitalWrite(brakePin,HIGH);
                break;
            default:
                break;
        }
    
}

void velocity::missonSub(const velocityControl::misson_msg& msg){
    misson=msg.misson;
}

void velocity::targetSub(const velocityControl::velocity_msg& msg){
    target_speed=msg.targetKph;
}

int main(int argc, char**argv){
    wiringPiSetup();
    pinMode(value,PWM_OUTPUT);
    pinMode(forward,OUTPUT);
    pinMode(backward,OUTPUT);
    pinMode(brakePin,OUTPUT);
    pinMode(AS_SW,INPUT);
    ros::init(argc,argv,"velCon_node");
    ros::NodeHandle nh;
    velocity velCon(&nh);
    
    ros::spin();
}
