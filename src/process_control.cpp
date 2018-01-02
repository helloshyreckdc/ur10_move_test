#include <ros/ros.h>
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

int main(int argc, char** argv){
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
	    finished = false;
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
		pose_pub.publish(pick_pose);
		break;
	    }

	    case 4:
	    {
		gripper_width = 48;
		gripper_pub.publish(gripper_width);
		break;
	    }

	    case 5:
	    {
		pick_pose.position.z += 0.2;
		pose_pub.publish(pick_pose);
		break;
	    }

	    case 6:
	    {
		place_pose.position.z += 0.2;
		pose_pub.publish(place_pose);
		break;
	    }

	    case 7:
	    {
		pose_pub.publish(place_pose);
		break;
	    }

	    case 8:
	    {
		gripper_width = 100;
		gripper_pub.publish(gripper_width);
		break;
	    }

	    case 9:
	    {
		place_pose.position.z += 0.2;
		pose_pub.publish(place_pose);
		break;
	    }

	    default:
	    {
		break;
	    }

	}
	ros::spinOnce();
	rate.sleep();
    }
    return 0;
};
