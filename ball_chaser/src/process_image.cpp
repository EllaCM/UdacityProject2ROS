#include "ros/ros.h"
#include "ball_chaser/DriveToTarget.h"
#include <sensor_msgs/Image.h>

ros::ServiceClient client;

void drive_bot (double lin_x, double ang_z){
  ROS_INFO_STREAM("drive the bot to the ball");

  ball_chaser::DriveToTarget srv;
  srv.request.linear_x = lin_x;
  srv.request.angular_z = ang_z;

  if(!client.call(srv)){
    ROS_ERROR("failed to call service ");
  }
}

void process_image_callback(const sensor_msgs::Image img){
  bool white_ball = false;
  int left_center_bdr = img.step/3;
  int right_center_bdr = 2*img.step/3;
  int pos=0;
  int row=0;
  for (int i=0.5*img.height*img.step; i<img.height*img.step; i++)
  {
    
    if(img.data[i]==255){
     	 pos = i%img.step;
    	 row = i/img.height;
    	 white_ball = true;
      	 ROS_INFO("POS - %d, %d", row, pos);
      	 break;
    }
  }
  
  if(white_ball){
  	if(pos>=0 && pos<left_center_bdr){
      		drive_bot(0.3, 0.5);
      		ROS_INFO_STREAM("to the left");
	} else if(pos>=left_center_bdr && pos<=right_center_bdr){
      		drive_bot(0.3, 0);
      		ROS_INFO_STREAM("straight forward");
    	} else if(pos>=right_center_bdr && pos<=img.step){
      		drive_bot(0.3, -0.5);
     	 	ROS_INFO_STREAM("to the right");
      	}
  }else {
      drive_bot(0.0, 0.0);
  }
}

int main(int argc, char** argv){
  ros::init(argc, argv, "process_image");
  ros::NodeHandle n;

  client = n.serviceClient<ball_chaser::DriveToTarget>("/ball_chaser/command_robot");
  ros::Subscriber sub = n.subscribe("/camera/rgb/image_raw", 10, process_image_callback);

  ros::spin();
  return 0;
}
