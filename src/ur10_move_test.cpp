#include <ros/ros.h>
#include <moveit/move_group_interface/move_group.h>

  geometry_msgs::Pose target_pose;
void chatterCallback(const geometry_msgs::Pose::ConstPtr& msg)
{
  

  target_pose.position.x = msg->position.x;
  target_pose.position.y = msg->position.y;
  target_pose.position.z = msg->position.z;

// set a fix orientation, when icp is improved, next piece of procedure
// should be used
//  target_pose.orientation.x = -0.500;
//  target_pose.orientation.y = -0.488;
//  target_pose.orientation.z = 0.541;
//  target_pose.orientation.w = -0.486;
  
// if icp return a good grasp orientation, use this code
  target_pose.orientation.x = msg->orientation.x;
  target_pose.orientation.y = msg->orientation.y;
  target_pose.orientation.z = msg->orientation.z;
  target_pose.orientation.w = msg->orientation.w;

  //target_pose.position.x = position.x;

  //ROS_INFO("I heard: [%f]", target_pose.position.x);
  
}

int main(int argc, char **argv)
{
  ros::init(argc, argv, "ur10_move");

  ros::NodeHandle n;

  ros::AsyncSpinner spinner(1);
  spinner.start();

  moveit::planning_interface::MoveGroup move_group("manipulator");

  ros::Subscriber sub = n.subscribe("pose_chatter", 1000, chatterCallback);

	ros::Rate rate(1.0);
  moveit::planning_interface::MoveGroup::Plan my_plan;
  while(ros::ok())
  {
	
	move_group.setPoseTarget(target_pose);
	bool success = move_group.plan(my_plan);
	if(success)
	{
	    move_group.move();
	}

	ros::spinOnce();
	rate.sleep();

  } 
  return 0;
}
