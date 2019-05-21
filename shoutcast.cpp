/**
*  @class SCDShoutcast - https://github.com/sc-develop
*
*  @brief interface class to shoutcast streaming library (libshout)
*         libshout is a library for streaming audio to icecast or shoutcast-compatible servers.
*         To use this functions you must install libshout-dev on your system
*
*         Require libShoutcast.cpp/hpp files
*
*         to use this class you must install libshout and libshout-dev on your linux system
*
*  @author Ing. Salvatore Cerami - dev.salvatore.cerami@gmail.com
*/

#include "shoutcast.hpp"
#include "QTextStream"
#include "QFile"
#include "QFileInfo"

/**
 * @brief Shoutcast::Shoutcast
 * @param host
 * @param port
 * @param username
 * @param password
 * @param protocol
 * @param format
 */
SCDShoutcast::SCDShoutcast(QString host, int port, QString username, QString password, SCDShoutcast::ShoutProtocol protocol, SCDShoutcast::AudioFormat format)
{
   ice_host     = host;
   ice_port     = port;
   ice_username = username;
   ice_password = password,
   ice_protocol = protocol,
   ice_format   = format;
   shout_handle = 0;
}

/**
 * @brief Shoutcast::open
 * @param mountPoint
 * @return
 */
int SCDShoutcast::open(const char *mountPoint, bool nonblocking)
{
   return open(mountPoint,nonblocking);
}

/**
 * @brief Shoutcast::open try to start new connection to server with provided moutPoint, if already connected (no new mount point mounted) do not nothing and return 1
 * @param  mountPoint Shoutcast mount point
 * @return 1: on success (connection opend, or already opened), 0: on error (connection not opened)
 */
int SCDShoutcast::open(QString mountPoint, bool nonblocking)
{
   if (shout_handle)
   {
     return 1;
   }

   ice_mount = mountPoint;

   shout_handle = shout_start((char *)ice_host.toStdString().c_str(),
                              ice_port,
                              ice_protocol,
                              (char *)ice_password.toStdString().c_str(),
                              (char *)ice_username.toStdString().c_str(),
                              (char *)ice_mount.toStdString().c_str(),
                              ice_format,
                              nonblocking);
   if (shout_handle)
   {
     lastError = "";

     return 1;
   }

   lastError = shout_last_error();

   return 0;
}

/**
 * @brief Shoutcast::send
 * @param buff
 * @param buff_size
 * @return 1: on success, 0: on error
 */
int SCDShoutcast::send(const char *buff, int buff_size, unsigned int sync)
{
  // Call libShoutcast function

  int ret = shout_send_buff(shout_handle, (unsigned char *)buff, buff_size, sync);

  if (ret)
  {
    lastError = "";
    return ret;
  }

  lastError = shout_last_error();

  return ret;
}

/**
 * @brief Shoutcast::send
 * @param buff
 * @param buff_size
 * @param startFromByte
 * @return
 */
int SCDShoutcast::send(const char *buff, int buff_size, int startFromByte, unsigned int sync)
{
   return send(buff + startFromByte, buff_size - startFromByte, sync);
}

/**
 * @brief Shoutcast::send
 * @param buff
 * @return 1: on success, 0: on error
 */
int SCDShoutcast::send(QByteArray *buff, unsigned int sync)
{
    return send(buff->constData(),buff->size(),sync);
}

/**
 * @brief Shoutcast::sendFile Stream the file 'filename' to icecast server. The connection to icecast must be opened.
 *                            The file will stream to current connected mountpoint
 * @param filename audio file fullpathname. The format of file must be ogg, mp3 or webm
 * @param mountPoint
 * @return
 */
int SCDShoutcast::streamFile(QString filename)
{
   QFile f(filename);

   if (!f.open(QIODevice::ReadOnly))
   {
      lastError = "Shoutcast: error opening file => " + filename;
      return 0;
   }

   QByteArray buffer;

   lastError = "Shoutcast: sending file => " + QFileInfo(filename).fileName();

   while (!f.atEnd())
   {
      buffer = f.read(1024);

      if (buffer.size() > 0)
      {
         if (!shout_send_buff(shout_handle,(unsigned char *)buffer.constData(),buffer.size(),1))
         {
            f.close();
            lastError =  "Shoutcast: sending error => " + QString(shout_get_error(shout_handle));
            return 0;
         }
      }
   }

   f.close();

   return 1;
}

/**
 * @brief Shoutcast::send
 * @param buff
 * @param startFromByte
 * @return
 */
int SCDShoutcast::send(QByteArray *buff, int startFromByte, unsigned int sync)
{
   return send(buff->constData() + startFromByte, buff->size() - startFromByte,sync);
}

/**
 * @brief Shoutcast::close
 * @return
 */
int SCDShoutcast::close()
{
   return shout_stop(shout_handle);
}
