#include <ros/ros.h>
#include <tf/transform_listener.h>
#include <geometry_msgs/Twist.h>
#include <turtlesim/Spawn.h>

int main(int argc, char** argv){
  //1.initail
  ros::init(argc, argv, "learning_tf_node_li");
  ros::NodeHandle nh;

  //2.new anohter turtle
  ros::service::waitForService("spawn");
  ros::ServiceClient add_turtle = nh.serviceClient<turtlesim::Spawn>("spawn");
  turtlesim::Spawn srv;
  add_turtle.call(srv);

  //3.create publisher to control turtle2
  ros::Publisher turtle_vel = nh.advertise<geometry_msgs::Twist>("turtle2/cmd_vel", 10);

  //4.create listener
  tf::TransformListener listener;
  ros::Rate rate(10.0);

  //5.We want the transform from frame /turtle1 to frame /turtle2.
  while (nh.ok()){
    tf::StampedTransform transform;
    try{
      ros::Time past= ros::Time::now()-ros::Duration(5.0);
      ros::Time now = ros::Time::now();

      listener.waitForTransform("/turtle2",now, "/turtle1",
                          past,"/world" ,ros::Duration(3.0));
      listener.lookupTransform("/turtle2",now, "/turtle1",
                         past,"/world", transform);

      //listener.waitForTransform("/turtle2", "/turtle1",
      //                   now, ros::Duration(3.0));
      //listener.lookupTransform("/turtle2", "/turtle1",
      //                   now, transform);

      //listener.lookupTransform("/turtle2", "/carrot1",ros::Time(0), transform);
      //listener.lookupTransform("/turtle2", "/turtle1",ros::Time(0), transform);
    }
    catch (tf::TransformException &ex) {
    ROS_ERROR("%s",ex.what());
    ros::Duration(1.0).sleep();
    continue;
    }

    //6.publish the value
    geometry_msgs::Twist vel_msg;
    vel_msg.angular.z = 4.0 * atan2(transform.getOrigin().y(),
                                     transform.getOrigin().x());
    vel_msg.linear.x = 0.5 * sqrt(pow(transform.getOrigin().x(), 2) +
                                   pow(transform.getOrigin().y(), 2));
    turtle_vel.publish(vel_msg);
    rate.sleep();
  }
  return 0;
}
