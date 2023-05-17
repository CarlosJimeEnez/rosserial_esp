#include <Arduino.h>

#include <ros.h>
#include <geometry_msgs/Twist.h>
#include <std_msgs/String.h>

#define LED_BUILTIN 2

////////////////// --------- ROS ////////////////// --------- 
ros::NodeHandle nh; 
std_msgs::String str_msgs; 


void messageCb(const geometry_msgs::Twist& toggle_msg){ 
  //nh.logdebug(String(toggle_msg.linear.x).c_str()); 
  digitalWrite(LED_BUILTIN, HIGH);
  delay(1000); 
  digitalWrite(LED_BUILTIN, LOW);  
}


// --------------------------- PUBLISHER --------------------------- //: 
ros::Publisher chatter("pub", &str_msgs); 


// --------------------------- SUBSCRIBER --------------------------- //: 
ros::Subscriber<geometry_msgs::Twist> sub("sliders_pose_esp", &messageCb );


char hello[13] = "hello world"; 


void setup() {
  // put your setup code here, to run once:
  nh.initNode(); 
  nh.advertise(chatter);

  // Subscriber Node 
  nh.subscribe(sub);   
  pinMode(LED_BUILTIN, OUTPUT); 
}


void loop() {
  // put your main code here, to run repeatedly:
  // str_msgs.data = hello; 
  // chatter.publish( &str_msgs ); 
  nh.spinOnce(); 
 
 // nh.loginfo("Info"); 
 
  delay(1000); 
}