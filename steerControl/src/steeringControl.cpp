#include "steering.h"

steer::steer(ros::NodeHandle *nh)
{
    target_sub = nh->subscribe("/target_angle", 10, &steer::targetSub, this);
    encode_sub = nh->subscribe("/encoder", 10, &steer::encodeSub, this);
    timer = nh->createTimer(ros::Duration(0.1), &steer::steerControl, this);
}

void steer::targetSub(const std_msgs::Int32 &msg)
{
    target_angle = msg.data;
}

void steer::encodeSub(const std_msgs::Int32 &msg)
{
    angle = msg.data * 3.6 / steerRatio;
}

void steer::steerControl(const ros::TimerEvent &)
{
    // 조향은 PID제어
    float err = target_angle - angle;
    static float prev_err = 0;
    float dErr = err - prev_err;
    static float I = 0;
    I += err * dt;
    float control_input = 0;
    // 세츄
    if (abs(I) > 1023)
        I = 0;
    prev_err = err;
    digitalWrite(err >= 0 ? right : left, HIGH);
    digitalWrite(err >= 0 ? left : right, LOW);

    control_input = std::min(abs(err*kp+ki*I+kd*(dErr/dt))*amp,1023); //최대 1023
    pwmWrite(value, control_input);
}

void setupWiringPi()
{
    wiringPiSetupPhys();
    pinMode(value, PWM_OUTPUT);
    pinMode(right, OUTPUT);
    pinMode(left, OUTPUT);

    pwmWrite(value, 0);
    digitalWrite(right, 0);
    digitalWrite(left, 0);
}

int main(int argc, char **argv)
{
    setupWiringPi();
    // 핀설정
    ros::init(argc, argv, "steeringControl_node");
    ros::NodeHandle nh;
    steer steerCon(&nh);
    ros::spin();
}