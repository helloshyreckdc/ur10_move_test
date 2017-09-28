#include <ros/ros.h>
#include <tf/transform_broadcaster.h>

int main(int argc, char** argv)
{
  ros::init(argc, argv, "my_tf_broadcaster");
  ros::NodeHandle node;

  tf::TransformBroadcaster br;
  tf::Transform transform;

  ros::Rate rate(10.0);
  while(node.ok())
  {
    transform.setOrigin( tf::Vector3(0.5, 0.256, 0.5));
    transform.setRotation( tf::Quaternion(0.707, 0.707, 0.0, 0.0));
    br.sendTransform(tf::StampedTransform(transform, ros::Time::now(), "/world", "/cokecan"));
    rate.sleep();
  }
  return 0;
}
