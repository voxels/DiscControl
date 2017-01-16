/***************************************************
  Adafruit MQTT Library Ethernet Example

  Adafruit invests time and resources providing this open source code,
  please support Adafruit and open-source hardware by purchasing
  products from Adafruit!

  Written by Alec Moore
  Derived from the code written by Limor Fried/Ladyada for Adafruit Industries.
  MIT license, all text above must be included in any redistribution
 ****************************************************/
#include <SPI.h>
#include "Adafruit_MQTT.h"
#include "Adafruit_MQTT_Client.h"

#include <Ethernet.h>
#include <EthernetClient.h>
#include <Dns.h>
#include <Dhcp.h>

#define TIMEOUT_LENGTH_MS 2 // Use this carefully because it fucks the framerate

/************************* Ethernet Client Setup *****************************/
byte mac[] = {0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED};

//Uncomment the following, and set to a valid ip if you don't have dhcp available.
//IPAddress iotIP (192, 168, 0, 42);
//Uncomment the following, and set to your preference if you don't have automatic dns.
//IPAddress dnsIP (8, 8, 8, 8);
//If you uncommented either of the above lines, make sure to change "Ethernet.begin(mac)" to "Ethernet.begin(mac, iotIP)" or "Ethernet.begin(mac, iotIP, dnsIP)"


/************************* Adafruit.io Setup *********************************/

#define AIO_SERVER      "192.168.1.42"
#define AIO_SERVERPORT  1883
#define AIO_USERNAME    ""
#define AIO_KEY         ""


/************ Global State (you don't need to change this!) ******************/

//Set up the ethernet client
EthernetClient client;

Adafruit_MQTT_Client mqtt(&client, AIO_SERVER, AIO_SERVERPORT, AIO_USERNAME, AIO_KEY);

// You don't need to change anything below this line!
#define halt(s) { Serial.println(F( s )); while(1);  }


/****************************** Feeds ***************************************/

// Notice MQTT paths for AIO follow the form: <username>/feeds/<feedname>
Adafruit_MQTT_Publish localTime = Adafruit_MQTT_Publish(&mqtt, "/displayTime");
Adafruit_MQTT_Subscribe frameBuffer = Adafruit_MQTT_Subscribe(&mqtt, AIO_USERNAME "/frameBuffer");
Adafruit_MQTT_Subscribe *subscription;

/*************************** Error Reporting *********************************/

Adafruit_MQTT_Subscribe errors = Adafruit_MQTT_Subscribe(&mqtt, AIO_USERNAME "/errors");
Adafruit_MQTT_Subscribe throttle = Adafruit_MQTT_Subscribe(&mqtt, AIO_USERNAME "/throttle");

/*************************** Sketch Code ************************************/

// Bug workaround for Arduino 1.6.6, it seems to need a function declaration
// for some reason (only affects ESP8266, likely an arduino-builder bug).
void MQTT_connect();

void ethernetsSetup() {
  Serial.begin(115200);

  Serial.println(F("Adafruit MQTT demo"));

  // Initialise the Client
  Serial.print(F("\nInit the Client..."));
  Ethernet.begin(mac);
  delay(1000); //give the ethernet a second to initialize
  Serial.println("IP address: "); Serial.println(Ethernet.localIP());

  mqtt.subscribe(&frameBuffer);

  // Setup MQTT subscriptions for throttle & error messages
  //  mqtt.subscribe(&throttle);
  //  mqtt.subscribe(&errors);
}

uint32_t x = 0;

size_t loopEthernets() {
  // Ensure the connection to the MQTT server is alive (this will make the first
  // connection and automatically reconnect when disconnected).  See the MQTT_connect
  // function definition further below.

  MQTT_connect();

  bool foundFrame = checkSubscriptions();

  // ping the server to keep the mqtt connection alive
  //  if(! mqtt.ping()) {
  //    mqtt.disconnect();
  //  }

  if ( mqtt.connected() && publishTime() )
  {
    /*
    Serial.print("Found buffer length: ");
    Serial.println(sizeof(frameBuffer.lastread));
    for( int i = 0; i < sizeof(frameBuffer.lastread); i++ )
    {
      Serial.print(frameBuffer.lastread[i]);
      Serial.print("\t");
    }
    Serial.println();
    */
    if( foundFrame )
    {
      return (sizeof(frameBuffer.lastread));      
    }
  }

  return 0;
}

void updateBufferValues(uint8_t *_frameValues)
{
    memcpy(_frameValues,frameBuffer.lastread, sizeof(frameBuffer.lastread)); 
}

uint8_t valueAtIndex(int _index)
{
  if( _index < sizeof(frameBuffer.lastread))
  {
    return frameBuffer.lastread[_index];
  }
}

bool checkSubscriptions()
{
  bool retval = false;
  if ( !mqtt.connected() )
  {
    // do nothing
  }

//  Serial.println("Checking subscriptions");
  // this is our 'wait for incoming subscription packets' busy subloop

  while ((subscription = mqtt.readSubscription(TIMEOUT_LENGTH_MS))) {
    if (subscription == &frameBuffer) {
      Serial.println("*");
      retval = true;
    }
    else
    {
      Serial.println("/");            
    }    
  }

//  Serial.println("Finished reading subscriptions");
  return retval;
}

bool publishTime()
{
  // Now we can publish stuff!
  Serial.print(F("\n\\displayTime:\t"));
  Serial.print(millis());
  Serial.print("...");
  if (! localTime.publish(millis())) {
    Serial.println(F("Failed"));
    return false;
  } else {
    Serial.println(F("OK!"));
    return true;
  }
}

void checkErrors()
{
  Serial.print(F("ERROR: "));
  Serial.println((char *)errors.lastread);
}

void checkThrottle()
{
  Serial.println((char *)throttle.lastread);
}

// Function to connect and reconnect as necessary to the MQTT server.
// Should be called in the loop function and it will take care if connecting.
void MQTT_connect() {
  int8_t ret;

  // Stop if already connected.
  if (mqtt.connected()) {
    return;
  }
  else
  {
    Serial.println("Not connected");
  }

  Serial.print("Connecting to MQTT... ");

  if ((ret = mqtt.connect()) != 0) { // connect will return 0 for connected
    Serial.println(mqtt.connectErrorString(ret));
    mqtt.disconnect();
    delay(3000);  // wait 3 seconds
  }
  else
  {
    Serial.println("MQTT Connected!");
  }
}
