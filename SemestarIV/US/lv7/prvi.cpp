#define TEMASUBLED1 "bakeroni/led1"
#define TEMASUBLED2 "bakeroni/led2"
#define TEMASUBLED3 "bakeroni/led3"
#define TEMAPUBPOT "bakeroni/potenciometar"
#define TEMAPUBTAST "bakeroni/taster"

#include "mbed.h"

#define MQTTCLIENT_QOS2 0

#include "MQTTNetwork.h"
#include "MQTTmbed.h"
#include "MQTTClient.h"
#include <string.h>

int arrivedcount = 0;
#define MQTT_CLIENT_NAME "bakeroni"
DigitalIn taster(p5);
DigitalOut led1(p6);
DigitalOut led2(p7);
AnalogIn pot(p15);
PwmOut led3(p21);

char* str;
double pot_value=-1;
bool taster_state=1;

void messageArrived_led1(MQTT::MessageData& md)
{
    MQTT::Message &message = md.message;
    printf("Message arrived: qos %d, retained %d, dup %d, packetid %d\r\n", message.qos, message.retained, message.dup, message.id);
    printf("Payload %.*s\r\n", message.payloadlen, (char*)message.payload);
    ++arrivedcount;
    str=(char*)message.payload;
    led1=atoi(str);
}

void messageArrived_led2(MQTT::MessageData& md)
{
    MQTT::Message &message = md.message;
    printf("Message arrived: qos %d, retained %d, dup %d, packetid %d\r\n", message.qos, message.retained, message.dup, message.id);
    printf("Payload %.*s\r\n", message.payloadlen, (char*)message.payload);
    ++arrivedcount;
    str=(char*)message.payload;
    led2=atoi(str);
}

void messageArrived_led3(MQTT::MessageData& md)
{
    MQTT::Message &message = md.message;
    printf("Message arrived: qos %d, retained %d, dup %d, packetid %d\r\n", message.qos, message.retained, message.dup, message.id);
    printf("Payload %.*s\r\n", message.payloadlen, (char*)message.payload);
    ++arrivedcount;
    str=(char*)message.payload;
    led3=atof(str);
}


int main(int argc, char* argv[])
{
    printf("Ugradbeni sistemi\r\n");
    printf("Demonstracija korištenja MQTT protokola\r\n\r\n");

    SocketAddress a;
    NetworkInterface *network;
    network = NetworkInterface::get_default_instance();
   
    if (!network)
        return -1;
    MQTTNetwork mqttNetwork(network);

    MQTT::Client<MQTTNetwork, Countdown> client(mqttNetwork);

    const char* hostname = "broker.hivemq.com";
    int port = 1883;
    printf("Connecting to %s:%d\r\n", hostname, port);
    int rc = mqttNetwork.connect(hostname, port);
    if (rc != 0)
        printf("rc from TCP connect is %d\r\n", rc);
    else
        printf("Connected to broker!\r\n");

    MQTTPacket_connectData data = MQTTPacket_connectData_initializer;
    data.MQTTVersion = 3;
    data.clientID.cstring = MQTT_CLIENT_NAME;
    data.username.cstring = "";
    data.password.cstring = "";
    if ((rc = client.connect(data)) != 0)
        printf("rc from MQTT connect is %d\r\n", rc);

    if ((rc = client.subscribe(TEMASUBLED1, MQTT::QOS0, messageArrived_led1)) != 0)
        printf("rc from MQTT subscribe is %d\r\n", rc);
    else
        printf("Subscribed to %s\r\n", TEMASUBLED1);


    if ((rc = client.subscribe(TEMASUBLED2, MQTT::QOS0, messageArrived_led2)) != 0)
        printf("rc from MQTT subscribe is %d\r\n", rc);
    else
        printf("Subscribed to %s\r\n", TEMASUBLED2);

    if ((rc = client.subscribe(TEMASUBLED3, MQTT::QOS0, messageArrived_led3)) != 0)
        printf("rc from MQTT subscribe is %d\r\n", rc);
    else
        printf("Subscribed to %s\r\n", TEMASUBLED3);

    MQTT::Message message;

    char buf[100];
    while(1) {
   
        if (taster_state!=taster) {
            taster_state=taster;
            sprintf(buf, "{\"Taster\": %d}", taster.read());
            message.qos = MQTT::QOS0;
            message.retained = false;
            message.dup = false;
            message.payload = (void*)buf;
            message.payloadlen = strlen(buf);
            rc = client.publish(TEMAPUBTAST, message);
        }
        if (pot_value!=pot) {
            pot_value=pot;
            sprintf(buf, "{\"Potenciometar\": %f}", pot_value);
            message.qos = MQTT::QOS0;
            message.retained = false;
            message.dup = false;
            message.payload = (void*)buf;
            message.payloadlen = strlen(buf);
            rc = client.publish(TEMAPUBPOT, message);
        }

        rc = client.subscribe(TEMASUBLED1, MQTT::QOS0, messageArrived_led1);
        rc = client.subscribe(TEMASUBLED2, MQTT::QOS0, messageArrived_led2);
        rc = client.subscribe(TEMASUBLED3, MQTT::QOS0, messageArrived_led3);

        wait_us(100);
    }

}
