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
    transform.setOrigin( tf::Vector3(0.28313, -0.23098, 0.807));
//    transform.setRotation( tf::Quaternion(-0.433, -0.325, 0.841, 0.003));
    tf::Quaternion q;
    q.setRPY(2.1749, -1.1701, 2.0046);
    transform.setRotation(q);
    br.sendTransform(tf::StampedTransform(transform, ros::Time::now(), "/world", "/cokecan"));
    rate.sleep();
  }
  return 0;
}
