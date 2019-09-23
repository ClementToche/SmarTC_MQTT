#ifndef SMARTC_MQTT_H
#define SMARTC_MQTT_H

#include <PubSubClient.h>

class SmarTC_MQTT
{
public:
    SmarTC_MQTT();
    ~SmarTC_MQTT();

    bool init(char *user, char *pwd, char *srv, int port, char *name);
    bool start();
    bool loop();

private:
    char *i_user;
    char *i_pwd;
    char *i_srv;
    int i_port;
    char *i_name;
    String i_id;

    WiFiClientSecure i_wClient;
    PubSubClient i_mqtt;
};

#endif //SMARTC_MQTT_H
