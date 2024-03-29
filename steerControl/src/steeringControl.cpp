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
    float P = kp * err;
    static float I = 0;
    I += ki * err * dt;
    float D = kd * (err - prev_err) / dt;
    // 세츄
    if (abs(I) > 1023)
        I = 0;
    prev_err = err;
    digitalWrite(err >= 0 ? right : left, HIGH);
    digitalWrite(err >= 0 ? left : right, LOW);
    pwmWrite(value, abs(P + I + D) * amp);
    ROS_INFO("angle: %d value: %f", angle, abs(P + I + D));
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