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
    transform.setOrigin( tf::Vector3(-0.692, 0.083, 0.242));
    transform.setRotation( tf::Quaternion(-0.565, 0.352, 0.651, 0.365));
    br.sendTransform(tf::StampedTransform(transform, ros::Time::now(), "/world", "/cokecan"));
    rate.sleep();
  }
  return 0;
}
