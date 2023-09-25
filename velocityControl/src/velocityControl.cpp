#include "velocityControl.h"

velocity::velocity(ros::NodeHandle *nh)
{
    kph = 0;
    target_speed = 0;
    speed_sub = nh->subscribe("/speed", 10, &velocity::speedSub, this);
    target_sub = nh->subscribe("/target_velocity", 10, &velocity::targetSub, this);
}
void velocity::speed_control(double speed, double target)
{ // 목표속도 PI제어
    // PI control
    float err = target - speed;
    float P = kp * err;
    static float I = 0;
    I += (ki * err * dt);
    if (abs(I) > 1023)
        I = 0;
    pwmWrite(accel, (P + I) * amp);
}
<<<<<<< HEAD
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
=======
>>>>>>> control

void velocity::release()
{
    digitalWrite(regenPin, LOW);
    pwmWrite(accel, 0);
    if (estopEng)
    {
        digitalWrite(brake1, LOW);
        ros::Duration(1.5).sleep;
        digitalWrite(brake2, LOW);
        estopEng = false;
    }
}
<<<<<<< HEAD
//미션에 따른 종방향제어는 상위에 맡기고 속도와 긴급정지만 판단한다.
//조향모터와 클러치에 릴레이 사용하는걸 생각하며 설계. -> 하드웨어적 연결로 해결(as_sw에 5V직결)
void velocity::speedSub(const odometer::speed_msg& msg){
    // ROS_INFO("KPH: %fkm/h, Trip: %fm",msg.kph, msg.odo);
    kph=msg.kph;

    ROS_INFO("%d,%d",digitalRead(AS_SW), digitalRead(estopPin));

    // if(AS_SW_flag && !estop_flag){ //자율주행 모드가 켜지고 Estop이 해제되어 있을 때
    speed_control(kph,target_speed);
    // }
=======
void velocity::estop()
{
    ROS_INFO("E-STOP!!");
    pwmWrite(accel, 0);
    digitalWrite(regenPin, HIGH);
    if (!estopEng)
    {
        digitalWrite(brake1, HIGH);
        ros::Duration(1.5).sleep;
        digitalWrite(brake2, HIGH);
        estopEng = true;
    }
>>>>>>> control
}
// 미션에 따른 종방향제어는 상위에 맡기고 속도와 긴급정지만 판단한다.
void velocity::speedSub(const odometer::speed_msg &msg)
{
    ROS_INFO("KPH: %fkm/h, Trip: %fm", msg.kph, msg.odo);
    kph = msg.kph;
    if (AS_SW_flag && !estop_flag)
    { // 자율주행 모드가 켜지고 Estop이 해제되어 있을 때
        speed_control(kph, target_speed);
    }
    else if (estop_flag)
    { // 상위제어 혹은 스위치로 estop 활성화 되어있을 때
        estop();
    }
    else
    { // 이도 저도 아닐때
        release();
    }
}
void velocity::targetSub(const velocityControl::velocity_msg &msg)
{
    target_speed = msg.targetKph;
    estop_flag = msg.estopEng;
}

void estop_switching()
{
    estop_flag = digitalRead(estopPin);
}
void AS_switching()
{
    AS_SW_flag = digitalRead(AS_SW);
}

void setupWiringPi()
{
    wiringPiSetupPhys();
    pinMode(accel, PWM_OUTPUT);
    pinMode(brake2, OUTPUT);
    pinMode(regenPin, OUTPUT);
    pinMode(brake1, OUTPUT);
    pinMode(AS_SW, INPUT);
    pinMode(estopPin, INPUT);
}

int main(int argc, char **argv)
{
    setupWiringPi();
    ros::init(argc, argv, "velCon_node");
    ros::NodeHandle nh;
    velocity velCon(&nh);
    wiringPiISR(estopPin, INT_EDGE_BOTH, &estop_switching);
    wiringPiISR(AS_SW, INT_EDGE_BOTH, &AS_switching);
    ros::spin();
}
