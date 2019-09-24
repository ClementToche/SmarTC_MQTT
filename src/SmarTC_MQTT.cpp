#include <WiFiClientSecure.h>
#include <SmarTC_MQTT.h>

SmarTC_MQTT::SmarTC_MQTT()
{
    i_user = NULL;
    i_pwd = NULL;
    i_srv = NULL;
    i_port = 0;
    i_name = NULL;
}

SmarTC_MQTT::~SmarTC_MQTT()
{
    if (i_user)
        free(i_user);
    if (i_pwd)
        free(i_pwd);
    if (i_srv)
        free(i_srv);
    if (i_name)
        free(i_name);
}

bool SmarTC_MQTT::init(char *user, char *pwd, char *srv, int port, char *name)
{
    i_user = user;
    i_pwd = pwd;
    i_srv = srv;
    i_port = port;
    i_name = name;
    i_id = String(RANDOM_REG32, HEX);

    i_mqtt = PubSubClient(i_wClient);
    i_mqtt.setServer(i_srv, i_port);

    return true;
}

bool SmarTC_MQTT::start()
{
    bool ret = false;

    byte count = 0;
    while (!i_mqtt.connected() && ++count < 5)
    {
        Serial.print(F("Attempting MQTT connection..."));
        if (i_mqtt.connect(i_id.c_str(), i_user, i_pwd))
        {
            Serial.println(F("connected"));
            ret = true;

            // Notify watchers of the connection
            char topic[100] = {0};
            sprintf(topic, "/smartc/home/%s/connection", i_name);
            i_mqtt.publish(topic, "connected");
        }
        else
        {
            Serial.print(F("failed, rc="));
            Serial.print(i_mqtt.state());
            Serial.println(F(" try again in 2 seconds"));
            delay(2000);
        }
    }

    return ret;
}

bool SmarTC_MQTT::loop()
{
    i_mqtt.loop();

    if (!i_mqtt.connected())
        start();

    return true;
}

bool SmarTC_MQTT::pirSense()
{
    char topic[100] = {0};

    sprintf(topic, "/smartc/home/%s/sensor/pir", i_name);
    return i_mqtt.publish(topic, "movement");
}