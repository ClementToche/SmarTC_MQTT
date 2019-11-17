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

    //TODO: Write certs to set secure cnx
    i_wClient.setInsecure();
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
        Serial.printf("Attempting MQTT connection with ID %s...\n", i_id.c_str());
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
    bool ret;
    char topic[100] = {0};

    sprintf(topic, "/smartc/home/%s/sensor/pir", i_name);
    ret = i_mqtt.publish(topic, "movement");
    if (!ret)
        Serial.printf("Error publishing \"movement\" on %s\n", topic);
    else
        Serial.printf("Success publishing \"movement\" on %s\n", topic);

    return ret;
}

bool SmarTC_MQTT::uvSense(uint16_t val)
{
    bool ret;
    char topic[100] = {0};

    sprintf(topic, "/smartc/home/%s/sensor/uv", i_name);
    ret = i_mqtt.publish(topic, String(val).c_str());
    if (!ret)
        Serial.printf("Error publishing \"%s\" on %s\n", String(val).c_str(), topic);
    else
        Serial.printf("Success publishing \"%s\" on %s\n", String(val).c_str(), topic);
    
    return ret;
}

bool SmarTC_MQTT::lightSense(uint16_t visible, uint16_t ir)
{
    bool ret;
    char topic[100] = {0};

    sprintf(topic, "/smartc/home/%s/sensor/visible", i_name);
    ret = i_mqtt.publish(topic, String(visible).c_str());
    if (!ret)
        Serial.printf("Error publishing \"%s\" on %s\n", String(visible).c_str(), topic);
    else
        Serial.printf("Success publishing \"%s\" on %s\n", String(visible).c_str(), topic);
    
    sprintf(topic, "/smartc/home/%s/sensor/ir", i_name);
    ret = i_mqtt.publish(topic, String(ir).c_str());
    if (!ret)
        Serial.printf("Error publishing \"%s\" on %s\n", String(ir).c_str(), topic);
    else
        Serial.printf("Success publishing \"%s\" on %s\n", String(ir).c_str(), topic);
    
    return ret;
}
