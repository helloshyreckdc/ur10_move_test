#include <ros/ros.h>
#include <tf/transform_listener.h>

int main(int argc, char** argv)
{
    ros::init(argc, argv, "pose_publisher");

    ros::NodeHandle node;

    tf::TransformListener listener;

    ros::Publisher pose_pub = node.advertise<geometry_msgs::Pose>("pose_chatter", 1000);

    ros::Rate rate(10.0);

    ros::param::set("/move_ur", true);

    while (node.ok())
    {
	tf::StampedTransform transform;
	try
	{
	    listener.waitForTransform("/world", "/cokecan", ros::Time(0), ros::Duration(3.0));
	    //      listener.lookupTransform("/cokecan", "/world", ros::Time::now(), transform);
	    listener.lookupTransform("/world", "/cokecan", ros::Time(0), transform);
	}
	catch (tf::TransformException ex)
	{
	    ROS_ERROR("%s",ex.what());
	    ros::Duration(1.0).sleep();
	}

	geometry_msgs::Pose cokecan_pose;
	// give a fix orientation for testing
//	cokecan_pose.orientation.x = -0.500;
//	cokecan_pose.orientation.y = -0.488;
//	cokecan_pose.orientation.z = 0.541;
//	cokecan_pose.orientation.w = -0.468;

	// can't use the direct icp result, need improving
        cokecan_pose.orientation.w = transform.getRotation().w();
        cokecan_pose.orientation.x = transform.getRotation().x();
        cokecan_pose.orientation.y = transform.getRotation().y();
        cokecan_pose.orientation.z = transform.getRotation().z();

	// const values are compensation for the camera and the world frame
	cokecan_pose.position.x = transform.getOrigin().x(); 
	cokecan_pose.position.y = transform.getOrigin().y();
	cokecan_pose.position.z = transform.getOrigin().z();
	pose_pub.publish(cokecan_pose);

	rate.sleep();
    }
    return 0;
};
