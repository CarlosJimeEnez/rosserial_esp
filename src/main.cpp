#include <Arduino.h>

//MQTT
#include <WiFi.h>
#include <PubSubClient.h>
#include <vector>

// ROS SETUP
#include <ros.h>
#include <geometry_msgs/Twist.h>
#include <std_msgs/String.h>

#include <setup_functions.h>

#define LED_BUILTIN 2

////////////////// --------- ROS ------------ ////////////////// 
ros::NodeHandle nh; 
std_msgs::String str_msgs; 


//////////////// -------------- MQTT SETUP -------------- ////////////////
WiFiClient espClient; 
PubSubClient client(espClient); 

// -------- Wifi variables ------- //
const char *ssid = "RS_NETWORK_1_2.4G"; 
const char *password = "rsautomation2017"; 
//const char *ssid = "BUAP_Estudiantes"; 
//const char *password = "f85ac21de4"; 
//const char *ssid = "MEGACABLE-979F"; 
//const char *password = "8eAYgaeY"; 

// ------------------- mqtt Broker: 
const char *mqtt_broker = "broker.emqx.io";
// TOPICS: 
std::vector<const char *> topics = {"esp32/mot1", "esp32/mot2", "esp32/mot3", "esp32/Begin"}; 
const char *mqtt_username = "emqx";
const char *mqtt_password = "public";
const int mqtt_port = 1883;


////////////////// -------------- Callback FUNCTION -------------- //////////////////
void messageCb(const geometry_msgs::Twist& toggle_msg){ 
  //nh.logdebug(String(toggle_msg.linear.x).c_str()); 
  client.publish(topics[0], "callback message"); 
  
}


// --------------------------- PUBLISHER --------------------------- //: 
ros::Publisher chatter("pub", &str_msgs); 


// --------------------------- SUBSCRIBER --------------------------- //: 
ros::Subscriber<geometry_msgs::Twist> sub("sliders_pose_esp", &messageCb );


char hello[13] = "hello world"; 

// CALLBACK : 
// * ------------- Funcion de llegada de los valores -------------------------- */
void callback(char *topic, byte *payload, unsigned int length){
    Serial.print("Message arrived in topic: ");
    Serial.println(topic);
    Serial.print("Message:");
    for (int i = 0; i < length; i++) {
    Serial.println((char) payload[i]);
    }

    // ------- Conversion de char a int  --------- //
    String topic_string = topic;
    String concatenacion = ""; 
    String auxiliar = "0"; 
    for (size_t i = 0; i < length; i++)
    {
        auxiliar = String((char) payload[i]); 
        concatenacion = concatenacion + auxiliar; 
    }
    int payload_int = concatenacion.toInt(); 
}    

void setup() {
  Serial.begin(57600); 
  conexion_wifi(ssid, password); 

  client.setServer(mqtt_broker, mqtt_port);
  client.setCallback(callback);
  while (!client.connected()) {
      String client_id = "esp32-client-";
      client_id += String(WiFi.macAddress());
      Serial.printf("The client %s connects to the public mqtt broker\n", client_id.c_str());
      if (client.connect(client_id.c_str(), mqtt_username, mqtt_password)) {
          Serial.println("Public emqx mqtt broker connected");
      } else {
          Serial.print("failed with state ");
          Serial.print(client.state());
          delay(2000);
      }
  }

// -------------------- subscribe --------------------- //
    client.publish(topics[0], "Holas desde la ESP");
    for(auto topic: topics){
        Serial.println(topic); 
        client.subscribe(topic);
    }
    client.subscribe("Begin");

  // put your setup code here, to run once:
  nh.initNode(); 
  nh.advertise(chatter);

  // Subscriber Node 
  nh.subscribe(sub);   
  pinMode(LED_BUILTIN, OUTPUT); 
}


void loop() {
  client.loop(); 


  // put your main code here, to run repeatedly:
  // str_msgs.data = hello; 
  // chatter.publish( &str_msgs ); 
  nh.spinOnce(); 
 
 // nh.loginfo("Info"); 
 
  delay(1000); 
}