#include "steercontrol_serial.h"

steer::steer(ros::NodeHandle *nh)
{
    target_sub = nh->subscribe("/target_angle", 10, &steer::targetSub, this);
    encode_sub = nh->subscribe("/encoder", 100, &steer::encodeSub, this);
    timer = nh->createTimer(ros::Duration(0.1), &steer::steerControl, this);
    try
    {
        ser.setPort("/dev/ttyUSB0");    //포트 수정 필
        ser.setBaudrate(9600);
        serial::Timeout to = serial::Timeout::simpleTimeout(100);
        ser.setTimeout(to);
        ser.open();
    }
    catch (serial::IOException& e)
    {
        ROS_ERROR_STREAM("Unable to open port ");
        return -1;
    }
    if(ser.isOpen()){
        ROS_INFO_STREAM("Serial Port initialized");
    }else{
        return -1;
    }
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
    float err = target_angle - angle;
    digitalWrite(err >= 0 ? right : left, HIGH);
    digitalWrite(err >= 0 ? left : right, LOW);
    pwmWrite(value, abs(err)<2?0:1000);
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