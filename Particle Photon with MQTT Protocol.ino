STARTUP(WiFi.selectAntenna(ANT_EXTERNAL));
SYSTEM_MODE(SEMI_AUTOMATIC);

#include <MQTT.h>

// Ip Address of MQTT broker
byte server1[] = {192,168,10,151};  //For computer server

//void callback(char* topic, byte* payload, unsigned int length);
void callback(char* topic, byte* payload, unsigned int length);
void readSerial(void);

MQTT client(server1, 1883, 2, callback);

String topic_, payload_text, mv_id, val;

void callback(char* topic, byte* payload, unsigned int length){
    char p[length + 1]; int i=0, n=0;
    memcpy(p, payload, length);
    p[length] = NULL;
    topic_ = topic;
    if(topic_.equals("user_name")){
    }else if(topic_.equals("get_id")){
    }
}

uint16_t qos2messageid = 0;

void qoscallback(unsigned int messageid){
    if (messageid == qos2messageid){
    }else{
      Serial.println("failed");
    }
}

void setup(){
    IPAddress myAddress(192,168,10,153);
    IPAddress netmask(255,255,255,0);
    IPAddress gateway(192,168,10,141);
    IPAddress dns(192,168,10,141);
    WiFi.setStaticIP(myAddress, netmask, gateway, dns);

    // now let's use the configured IP
    WiFi.useStaticIP();

    WiFi.on();
    //WiFi.setCredentials("Machine Vision");
    WiFi.connect();

    //pinMode(prox_sen1, INPUT_PULLDOWN);
    //pinMode(prox_sen2, INPUT_PULLDOWN);
    //attachInterrupt(prox_sen1, detectAcceptSensor, RISING);
    //attachInterrupt(prox_sen2, detectRejectSensor, RISING);

    // Serial communication
    Serial.begin(115200);
    Serial1.begin(115200);

    //Syncronize Time Zone (Jakarta Indonesia)
    //Connecting to Particle Cloud
    Particle.connect();
    while(Particle.connected() == false){
        Serial.println("Please wait, Connecting to cloud ...");
        Particle.connect();
        delay(1000);
    }
    if(Particle.connected()){
        Particle.syncTime();
        waitUntil(Particle.syncTimeDone);
        Time.zone(+7);  //For Jakarta TIme Zone From UTC
        Particle.disconnect();
    }

    while(WiFi.ready() == false){ ; } //if wifi not ready
    client.connect("192.168.10.153");
    while (!(client.isConnected())){ //if cannot connect to MQTT Broker
      client.connect("192.168.10.153");
      delay(1000);
    }
    if (client.isConnected()){
        client.loop();
        client.subscribe("user_name");
        client.subscribe("get_id");     //For getting ID
        client.setBroker(server1, 1883);
        //Publish to send code to server
        //client.publish("send_code",machine_code);
        //Publish with QOS 1
        client.publish("ready", "device is ready", MQTT::QOS1, &messageid);
        qosDTmessageid = messageid;
        delay(1000);
    }
    delay(100);
}

// For QoS of MQTT Publish
uint16_t messageid;
void loop(){
    if (client.isConnected()){
      }else{
        client.connect("192.168.10.153");
        if (client.isConnected()){
          client.loop();
        }
      }


    if (client.isConnected()){
      //Signal Strength indicator
      signalStrength=WiFi.RSSI();
      if((signalStrength >= -30) && (signalStrength <= -1)){        //Good Signal
        //Good Signal
      }else if((signalStrength >= -85) && (signalStrength <= -31)){ //Dangerous Signal
        //Dangerous Signal
      }else if((signalStrength >= -127) && (signalStrength <= -86)){  //Prohibited
        //Prohibited Signal
      }else{
        //Very Poor Signal
      }
    }else{
      //Device not connected to server
    }

    //Subscribe every second
    if (client.isConnected()){
      client.loop();
      client.subscribe("user_name");  //For User name
      client.subscribe("get_id");     //For getting ID
    }

}

/*bool strComplete=false;
void readSerial(){
  data="";
  if(Serial1.available()>0){
    data = Serial1.readString();
    strComplete=true;

    if (client.isConnected()){
      client.loop();
    }else{
      f_connect=1;
      if (client.isConnected()){
        client.loop();
        client.subscribe("user_name");     
        client.subscribe("get_id");     //For getting ID   
      }
    }
  }

  if(strComplete){
      Serial.println(data); //For testing
      strComplete=false;
  }
} */
void detectAcceptSensor(){}
void detectRejectSensor(){}
