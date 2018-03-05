#include <ros/ros.h>
#include <tf/transform_broadcaster.h>
#include <turtlesim/Pose.h>

std::string turtle_name;

void poseCallback(const turtlesim::PoseConstPtr& msg){
//void poseCallback(const turtlesim::Pose& msg){
  tf::Quaternion q;//need to have an object first
  q.setRPY(0, 0, msg->theta);
  //q.setRPY(0, 0, msg.theta);

  tf::Transform transform;
  transform.setOrigin( tf::Vector3(msg->x, msg->y, 0.0) );
  //transform.setOrigin( tf::Vector3(msg.x, msg.y, 0.0) );
  transform.setRotation(q);
  //transform.setRotation(tf::Quaternion.setRPY(0,0,msg->theta));//cant write this

  static tf::TransformBroadcaster br;
  br.sendTransform(tf::StampedTransform(transform, ros::Time::now(), "world", turtle_name));
}

int main(int argc, char** argv){
  ros::init(argc, argv, "learning_tf_node_br");
  ros::NodeHandle nh;

  if (argc != 2){
    ROS_ERROR_STREAM("need turtle name as argument");
    return -1;
  }

  turtle_name = argv[1];
  ros::Subscriber sub = nh.subscribe(turtle_name+"/pose", 10, &poseCallback);

  ros::spin();
  return 0;
}
