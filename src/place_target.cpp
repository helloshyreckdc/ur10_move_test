#include <ros/ros.h>
#include <tf/transform_listener.h>

int count = 0; // count 5 seconds waiting for the frame stable
float x = 0, y = 0, z = 0;
float x_old = 0, y_old = 0, z_old = 0;

void positionCallback(const geometry_msgs::Pose::ConstPtr& msg)
{
    x = msg->position.x;
    y = msg->position.y;
    z = msg->position.z;
}
int main(int argc, char** argv){
    ros::init(argc, argv, "place_target_publisher");

    ros::NodeHandle node;

    tf::TransformListener listener;

    ros::Subscriber sub = node.subscribe("/object_position_in_world", 10, positionCallback);
    ros::Publisher pose_pub = node.advertise<geometry_msgs::Pose>("place_target", 10);

    ros::Rate rate(10.0);

    bool place_target = false;

    while (node.ok()){
	ros::param::get("/place_target", place_target);
	if(place_target)
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
		geometry_msgs::Pose place_target;
		// give a fix orientation for testing
		place_target.orientation.x = -0.510;
		place_target.orientation.y = 0.490;
		place_target.orientation.z = 0.494;
		place_target.orientation.w = 0.506;

		// can't use the direct icp result, need improving
		//    place_target.orientation.w = transform.getRotation().w();
		//    place_target.orientation.x = transform.getRotation().x();
		//    place_target.orientation.y = transform.getRotation().y();
		//    place_target.orientation.z = transform.getRotation().z();

		// const values are compensation for the camera and the world frame
		place_target.position.x = x_old + 0.06;
		place_target.position.y = y_old + 0.01;
		place_target.position.z = z_old + 0.06;
		pose_pub.publish(place_target);
		if(count == 50)
		{
		    ros::param::set("/finished_job", true);
		    ROS_INFO("HEY");
		    ++count;
		}
	    } 
	}

	ros::spinOnce();
	rate.sleep();
    }
    return 0;
};
