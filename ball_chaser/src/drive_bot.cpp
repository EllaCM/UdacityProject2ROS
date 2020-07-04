#include "ros/ros.h"
#include "geometry_msgs/Twist.h"
#include "ball_chaser/DriveToTarget.h"
#include <std_msgs/Float64.h>

ros::Publisher motor_command_publisher;

bool handle_drive_request(ball_chaser::DriveToTarget::Request &req, ball_chaser::DriveToTarget::Response &res)
{	
	ROS_INFO("DriveToTarget_request recieved - lx:%1.2f, az:%1.2f", (double)req.linear_x, (double)req.angular_z);
	
	std_msgs::Float64 l_x, a_z;
	l_x.data = req.linear_x;
	a_z.data = req.angular_z;
	geometry_msgs::Twist motor_command;
	motor_command.linear.x = (double)l_x.data;
	motor_command.angular.z = (double)a_z.data;
	motor_command_publisher.publish(motor_command);

	ros::Duration(3).sleep();

	res.msg_feedback = "Motor_command set - linear_x:"+std::to_string(l_x.data)+", angular_z:"+std::to_string(a_z.data);
	ROS_INFO_STREAM(res.msg_feedback);
	return true;
}

int main(int argc, char** argv)
{
	//initialize a ROS node
	ros::init(argc, argv, "drive_bot");
	ros::NodeHandle n;
	//"/cmd_vel"is the ros actuation topic that the message will be publish on
	motor_command_publisher = n.advertise<geometry_msgs::Twist>("/differential_drive_controller/cmd_vel",10);
	
	ros::ServiceServer service = n.advertiseService("/ball_chaser/command_robot", handle_drive_request);
	ROS_INFO("Ready to send motor commands");
	ros::spin();
	return 0;

}
