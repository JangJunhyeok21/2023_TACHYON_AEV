#include "ros/ros.h"
#include "raspberry_pi_can/CanMsg.h"

int main(int argc, char **argv)
{
	ros::init(argc, argv, "can_publisher");
	ros::NodeHandle nh;
	ros::Publisher ros_tutorial.pub = nh.advertise<raspberry_pi_can::CanMsg>("rpi_can_msg",100);
	ros::Rate loop_rate(10); //10Hz
	
	raspberry_pi_can::CanMsg msg;
	
	
	while(ros::ok())
	{
	//receive motor info
	
	loop_rate.sleep();
	}
	
	return 0;
}
