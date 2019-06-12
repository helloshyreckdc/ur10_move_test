#include <ros/ros.h>
#include <tf/transform_broadcaster.h>


int main(int argc, char** argv)
{
	ros::init(argc, argv, "time_node");
	ros::NodeHandle node;
	ros::Time begin = ros::Time::now();
	ros::Duration(0.5).sleep();
	ros::Time end = ros::Time::now();
	if((end -begin).toSec()>0.5)
		ROS_INFO("good");
	else
		ROS_INFO("bad");

	return 0;


}
