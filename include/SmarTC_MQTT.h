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

    bool lightSense ( uint16_t visible, uint16_t ir );
    bool pirSense   ( void );
    bool uvSense    ( uint16_t val );

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
