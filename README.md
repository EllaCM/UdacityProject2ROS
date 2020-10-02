# UdacityProject2ROS
Task: place a movable white ball in front of the robot for navigation. 
Once the robot detects the white ball in its camera, it drives itself towards the white ball.
The same white ball can be dragged by the user to different positions in the environment anytime.

CMD terminal:
/*Use "roslaunch my_robot world.launch" to launch the robot in gazebo world

Open another terminal and use "rosrun tf static_transform_publisher 0 0 0 0 0 0 1 map robot_footprint 10" to fix the reference frame of Robot in Rviz

Use "roslaunch ball_chaser ball_chaser.launch" to run the process_image and ball_chaser

A white ball is placed outside the building, drag it inside for the robot to chase

<--!Reminder: better not rotate the robot in PI, because the wall behind it is painted in white-->
*/
