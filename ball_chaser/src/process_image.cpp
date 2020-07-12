#include "ros/ros.h"
#include "ball_chaser/DriveToTarget.h"
#include <sensor_msgs/Image.h>

ros::ServiceClient client;

void drive_bot (float lin_x, float ang_z){
  ROS_INFO_STREAM("drive the bot to the ball");

  ball_chaser::DriveToTarget srv;
  srv.request.linear_x = lin_x;
  srv.request.angular_z = ang_z;

  if(!client.call(srv)){
    ROS_ERROR("failed to call service ");
  }
}

void process_image_callback(const sensor_msgs::Image img){
  int white_pixel = 255;
  bool white_ball = false;
  int left_center_bdr = (1/3)*img.step;
  int right_center_bdr = (2/3)*img.step;
  for (int i=0; i<img.height*img.step; i++)
  {
    int pos = i%img.step;
    if(img.data[i]==white_pixel && 0<=pos<left_center_bdr){
      drive_bot(0.5, -0.5);
      white_ball = true;
      break;
    } else if(img.data[i]==white_pixel && left_center_bdr<=pos<right_center_bdr){
      drive_bot(0.5, 0);
      white_ball = true;
      break;
    } else if(img.data[i]==white_pixel && right_center_bdr<=pos<=img.step){
      drive_bot(0.5, 0.5);
      white_ball = true;
      break;
    }
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
