#include <ros/ros.h>
#include <tf/transform_broadcaster.h>

geometry_msgs::Pose QR_pose;
void QR_callback(const geometry_msgs::PoseStamped::ConstPtr& msg)
{
  QR_pose = msg->pose;	
}

int main(int argc, char** argv)
{
  ros::init(argc, argv, "object_frame");
  ros::NodeHandle node;

  ros::Subscriber sub = node.subscribe("/object_position", 10, QR_callback);
  tf::TransformBroadcaster br;
  tf::Transform transform;

  ros::Rate rate(10.0);
  while(node.ok())
  {
    transform.setOrigin(tf::Vector3(QR_pose.position.x, QR_pose.position.y, QR_pose.position.z));
    transform.setRotation(tf::Quaternion(QR_pose.orientation.x, QR_pose.orientation.y, QR_pose.orientation.z, QR_pose.orientation.w));
//    tf::Quaternion q;
//    q.setRPY(0, 0, 0);
//    transform.setRotation(q);
    br.sendTransform(tf::StampedTransform(transform, ros::Time::now(), "/camera_link", "/QR_frame"));
// This is the gravity frame of the object, -0.05 is the half length of the edge
// When I could detect the size of the object, it should be a variable read from a topic 
    transform.setOrigin(tf::Vector3(0, 0, -0.05));
    transform.setRotation(tf::Quaternion(0, 0, 0, 1));
    br.sendTransform(tf::StampedTransform(transform, ros::Time::now(), "/QR_frame", "/gravity_frame"));
    ros::spinOnce();
    rate.sleep();
  }
  return 0;
}