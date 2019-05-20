#include <ros/ros.h>
#include <tf/transform_listener.h>
#include <Eigen/Eigen>
#include <fstream>
#include <geometry_msgs/WrenchStamped.h>

void dataCallback(const geometry_msgs::WrenchStamped::ConstPtr& msg);

void print4x4Matrix(const Eigen::Matrix4d & matrix)
{
	printf("Rotation matrix :\n");
	printf("    | %6.3f %6.3f %6.3f | \n", matrix(0, 0), matrix(0, 1), matrix(0, 2));
	printf("R = | %6.3f %6.3f %6.3f | \n", matrix (1, 0), matrix (1, 1), matrix (1, 2));
	printf("    | %6.3f %6.3f %6.3f | \n", matrix (2, 0), matrix (2, 1), matrix (2, 2));
	printf("Translation vector :\n");
	printf("t = < %6.3f, %6.3f, %6.3f >\n\n", matrix (0, 3), matrix (1, 3), matrix (2, 3));
}

void write_matrix(const Eigen::Matrix4d & matrix)
{
	const char *pFileName = "T.txt";
	FILE *pFile;
	pFile = fopen(pFileName, "a"); //a mode for not covering original data
	if(NULL == pFile)
	{
		printf("error");
		return;
	}

	fprintf(pFile, "%9.6f %9.6f %9.6f %9.6f \n", matrix(0, 0), matrix(0, 1), matrix(0, 2), matrix(0,3));
	fprintf(pFile, "%9.6f %9.6f %9.6f %9.6f \n", matrix(1, 0), matrix(1, 1), matrix(1, 2), matrix(1,3));
	fprintf(pFile, "%9.6f %9.6f %9.6f %9.6f \n", matrix(2, 0), matrix(2, 1), matrix(2, 2), matrix(2,3));
	fprintf(pFile, "%9.6f %9.6f %9.6f %9.6f \n", matrix(3, 0), matrix(3, 1), matrix(3, 2), matrix(3,3));
	fprintf(pFile, "\n");
	fclose(pFile);
}

void write_wrench(double x, double y, double z, double fx,double fy,double fz)
{
	const char *pFileName = "O.txt";
	FILE *pFile;
	pFile = fopen(pFileName, "a"); //a mode for not covering original data
	if(NULL == pFile)
	{
		printf("error");
		return;
	}

	fprintf(pFile, "%9.6f \n", x);
	fprintf(pFile, "%9.6f \n", y);
	fprintf(pFile, "%9.6f \n", z);
	fprintf(pFile, "%9.6f \n", fx);
	fprintf(pFile, "%9.6f \n", fy);
	fprintf(pFile, "%9.6f \n", fz);
	fprintf(pFile, "\n");
	fclose(pFile);
}

void dataCallback(const geometry_msgs::WrenchStamped::ConstPtr& msg)
{
	double x = msg->wrench.force.x;
	double y = msg->wrench.force.y;
	double z = msg->wrench.force.z;
	double fx = msg->wrench.torque.x;
	double fy = msg->wrench.torque.y;
	double fz = msg->wrench.torque.z;
	write_wrench(x, y, z, fx, fy, fz);
}

int main(int argc, char** argv)
{
	ros::init(argc, argv, "base_to_sensor");

	ros::NodeHandle node;

	tf::TransformListener listener;

	ros::Publisher pose_pub = node.advertise<geometry_msgs::Pose>("trans_chatter", 1000);
	ros::Subscriber sensor_sub = node.subscribe("/ethdaq_data_raw",10, dataCallback);

	ros::Rate rate(1.0);


	while (node.ok())
	{
		tf::StampedTransform transform;
		try
		{
			listener.waitForTransform("/base_link", "/sensor_link", ros::Time(0), ros::Duration(3.0));
			//      listener.lookupTransform("/cokecan", "/world", ros::Time::now(), transform);
			listener.lookupTransform("/base_link", "/sensor_link", ros::Time(0), transform);
		}
		catch (tf::TransformException ex)
		{
			ROS_ERROR("%s",ex.what());
			ros::Duration(1.0).sleep();
		}

		geometry_msgs::Pose sensor_frame_pose;

		Eigen::Matrix4d T;
		tf::Matrix3x3 R(transform.getRotation());
		// give a fix orientation for testing
		//	sensor_frame_pose.orientation.x = -0.500;
		//	sensor_frame_pose.orientation.y = -0.488;
		//	sensor_frame_pose.orientation.z = 0.541;
		//	sensor_frame_pose.orientation.w = -0.468;
		// can't use the direct icp result, need improving

		//        sensor_frame_pose.orientation.w = transform.getRotation().w();
		//        sensor_frame_pose.orientation.x = transform.getRotation().x();
		//        sensor_frame_pose.orientation.y = transform.getRotation().y();
		//        sensor_frame_pose.orientation.z = transform.getRotation().z();

		// const values are compensation for the camera and the world frame
		T(0,0) = R[0][0];
		T(0,1) = R[0][1];
		T(0,2) = R[0][2];
		T(1,0) = R[1][0];
		T(1,1) = R[1][1];
		T(1,2) = R[1][2];
		T(2,0) = R[2][0];
		T(2,1) = R[2][1];
		T(2,2) = R[2][2];
		T(0,3) = transform.getOrigin().x(); 
		T(1,3) = transform.getOrigin().y();
		T(2,3) = transform.getOrigin().z();
		T(3,0) = 0;
		T(3,1) = 0;
		T(3,2) = 0;
		T(3,3) = 1;

		print4x4Matrix(T);
		write_matrix(T);
		pose_pub.publish(sensor_frame_pose);

		rate.sleep();
		ros::spinOnce();
	}
	return 0;
};
