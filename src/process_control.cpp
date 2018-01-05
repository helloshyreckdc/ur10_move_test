#include <ros/ros.h>
#include <std_msgs/Float64.h>
#include <tf/transform_listener.h>

geometry_msgs::Pose place_pose;
geometry_msgs::Pose pick_pose;

void placeCallback(const geometry_msgs::Pose::ConstPtr& msg)
{
    place_pose = *msg;
}

void pickCallback(const geometry_msgs::Pose::ConstPtr& msg)
{
    pick_pose = *msg;
}

int main(int argc, char** argv)
{
    ros::init(argc, argv, "process_control");

    ros::NodeHandle node;

    ros::Subscriber sub_place = node.subscribe("/place_target", 10, placeCallback);
    ros::Subscriber sub_pick = node.subscribe("/pick_target", 10, pickCallback);
    ros::Publisher pose_pub = node.advertise<geometry_msgs::Pose>("pose_chatter", 10);
    ros::Publisher gripper_pub = node.advertise<std_msgs::Float64>("gripper_width", 10);
    std_msgs::Float64 gripper_width;

    ros::Rate rate(10.0);

    int step = 1;  // indicate which step is processing
    bool finished = false; // indicate if the job is finished

    while (node.ok())
    {
	ros::param::get("/finished_job", finished);	
	if(finished)
	{
	    ros::param::set("/finished_job", false);	
	    ++step;
	}
	switch(step)
	{
	    case 1:
	    {
		ros::param::set("/place_target", true);	
		break;
	    }

	    case 2:
	    {
		ros::param::set("/pick_target", true);	
		break;
	    }

	    case 3:
	    {
//		pick_pose.position.z += 0.1;
		pose_pub.publish(pick_pose);
		ros::param::set("/move_ur", true);
		break;
	    }

	    case 4:
	    {
		gripper_width.data = 48;
		gripper_pub.publish(gripper_width);
		break;
	    }

	    case 5:
	    {
		pick_pose.position.z += 0.28;
		pose_pub.publish(pick_pose);
		ros::param::set("/move_ur", true);
		break;
	    }

	    case 6:
	    {
		place_pose.position.z += 0.15;
		pose_pub.publish(place_pose);
		ros::param::set("/move_ur", true);
		break;
	    }

	    case 7:
	    {
//		place_pose.position.z += 0.1;
		pose_pub.publish(place_pose);
		ros::param::set("/move_ur", true);
		break;
	    }

	    case 8:
	    {
		gripper_width.data = 100;
		gripper_pub.publish(gripper_width);
		break;
	    }

	    case 9:
	    {
		pick_pose.position.z += 0.35;
		pose_pub.publish(pick_pose);
		ros::param::set("/move_ur", true);
		break;
	    }

	    default:
	    {
		break;
	    }

	}
	ROS_INFO("step %d\n", step);
	ros::spinOnce();
	rate.sleep();
    }

    ros::param::set("/move_ur", false);
    ros::param::set("/finished_job", false);
    ros::param::set("/place_target", false);
    ros::param::set("/pick_target", false);
    return 0;
};
