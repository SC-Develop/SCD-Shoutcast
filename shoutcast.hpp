#ifndef SCDShoutcast_H
#define SCDShoutcast_H

#include <QString>

#include "libshoutcast.hpp"

#include <shout/shout.h>

class SCDShoutcast
{
  private:

    QString ice_username;
    QString ice_password;
    QString ice_mount;
    QString ice_host;
    int     ice_port;

    unsigned int ice_protocol;
    unsigned int ice_format;

    shout_t  *shout_handle = 0;

    QString lastError = "";

  public:

    enum AudioFormat   {OGG  = SHOUT_FORMAT_OGG,
                        MP3  = SHOUT_FORMAT_MP3,
                        WEBM = SHOUT_FORMAT_WEBM};

    enum ShoutProtocol {HTTP       = SHOUT_PROTOCOL_HTTP,
                        XAUDIOCAST = SHOUT_PROTOCOL_XAUDIOCAST,
                        ICY        = SHOUT_PROTOCOL_ICY};

    SCDShoutcast(QString host = "localhost",
                 int port = 8000,
                 QString username = "source",
                 QString password = "hackme",
                 ShoutProtocol protocol = HTTP,
                 AudioFormat   format   = OGG);

    QString lastErrorString() {return lastError;}

    bool connected() {return shout_handle!=0;}

    int open(const char *mountPoint, bool nonblocking=false);

    int open(QString mountPoint="Shoutcast", bool nonblocking=false);

    int send(const char *buff, int buff_size, unsigned int sync=1);

    int send(const char *buff, int buff_size, int startFromByte, unsigned int sync=1);

    int send(QByteArray *buff, int startFromByte, unsigned int sync=1);

    int send(QByteArray *buff, unsigned int sync=1);

    int streamFile(QString filename);

    int close();

    QString iceHost()     {return ice_host;}
    int     icePort()     {return ice_port;}
    QString iceUsername() {return ice_username;}
    QString icePassword() {return ice_password;}
    QString iceMount()    {return ice_mount;}

    ShoutProtocol iceProtocol() {return (ShoutProtocol) ice_protocol;}
    AudioFormat   iceFormat()   {return (AudioFormat)   ice_format;}
};

#endif // SCDShoutcast_H
