#include <ros/ros.h>
#include <tf/transform_listener.h>

int count = 0; // count 2 seconds waiting for the frame stable
float x = 0, y = 0, z = 0;
float x_old = 0, y_old = 0, z_old = 0;

void positionCallback(const geometry_msgs::Pose::ConstPtr& msg)
{
    x = msg->position.x;
    y = msg->position.y;
    z = msg->position.z;
}
int main(int argc, char** argv){
    ros::init(argc, argv, "pick_target_publisher");

    ros::NodeHandle node;

    tf::TransformListener listener;

    ros::Subscriber sub = node.subscribe("/object_position_in_world", 10, positionCallback);
    ros::Publisher pose_pub = node.advertise<geometry_msgs::Pose>("pick_target", 10);

    ros::Rate rate(10.0);

    bool pick_target = false;

    while (node.ok()){
	ros::param::get("/pick_target", pick_target);
	if(pick_target)
	{
	    if(count < 50)
	    {
		float diff = (x-x_old)*(x-x_old) + (y-y_old)*(y-y_old) + (z-z_old)*(z-z_old);
		if(diff > 0.00025)
		    count = 0;

		x_old = x, y_old = y, z_old = z;

		++count;
	    }
	    else
	    {
		geometry_msgs::Pose pick_target;
		// give a fix orientation for testing
		pick_target.orientation.x = -0.510;
		pick_target.orientation.y = 0.490;
		pick_target.orientation.z = 0.494;
		pick_target.orientation.w = 0.506;

		// can't use the direct icp result, need improving
		//    pick_target.orientation.w = transform.getRotation().w();
		//    pick_target.orientation.x = transform.getRotation().x();
		//    pick_target.orientation.y = transform.getRotation().y();
		//    pick_target.orientation.z = transform.getRotation().z();

		// const values are compensation for the camera and the world frame
		pick_target.position.x = x_old + 0.06;
		pick_target.position.y = y_old + 0.01;
		pick_target.position.z = z_old + 0.06;
		pose_pub.publish(pick_target);
		if(count == 50)
		{
		    ros::param::set("/finished_job", true);
		    ROS_INFO("hey");
		    ++count;
		}
	    } 
	}

	ros::spinOnce();
	rate.sleep();
    }
    return 0;
};
