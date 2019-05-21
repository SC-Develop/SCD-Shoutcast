/**
 *
 * @package libshoutcast.cpp
 *
 * @brief   Simple cpp libshout functions wrapper
 *          libshout is a library for streaming audio to icecast or shoutcast-compatible servers.
 *          To use this functions you must install libshout-dev on your system
 *
 *          to use this functions you must install libshout and libshout-dev on your linux system
 *
 * @author  Salvatore Cerami dev.salvatore.cerami@gmail.com 
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <shout/shout.h>

static char errmessage[512];

/**
 * @brief shout_last_error -> retunr the last error string
 * @return last errro message
 */
char *shout_last_error()
{
  return errmessage;
}

/**
 * @brief shout_stop Close opened connection to server, and free resource.
 *
 * @param shout
 * @return 1: closing connection success
 *         0: closing connection failure: the connection is already closed or shout handle is invalid
 *            all resource are anyway freed
 */
int shout_stop(shout_t *shout)
{
  bool success = 0;

  if (shout)
  {
    if (shout_close(shout)==SHOUTERR_SUCCESS)
    {
      success = 1; // close success
    }
    else
    {
      success = 0; // close error
    }
  }

  shout_shutdown();

  shout = 0;

  return success;
}

/**
 * @brief shout_start -> Open a connection to stream data to an icecast server
 *
 *                       params value example:
 *
 *                         host      = 127.0.0.1
 *                         protocol  = SHOUT_PROTOCOL_HTTP
 *                         port      = 8000
 *                         password  = hackme (source password for icecast)
 *                         user      = source
 *                         mount     = stream
 *                         format    = SHOUT_FORMAT_MP3
 * @param host
 * @param port
 * @param protocol
 * @param password
 * @param user
 * @param mount
 * @param format
 * @return connection handle, or null pointer
 */
shout_t *shout_start(char *host, int port, unsigned int protocol, char *password, char *user, char *mount, unsigned int format, unsigned int nonblocking)
{
  shout_t *shout;

  shout_init();

  if (!(shout = shout_new()))
  {
    sprintf(errmessage,"Could not allocate shout_t\n");
    return 0;
  }

  if (shout_set_host(shout, host) != SHOUTERR_SUCCESS)
  {
    sprintf(errmessage,"Error setting hostname: %s\n", shout_get_error(shout));
    shout_stop(shout);
    return 0;
  }

  if (shout_set_protocol(shout, protocol) != SHOUTERR_SUCCESS)
  {
    sprintf(errmessage,"Error setting protocol: %s\n", shout_get_error(shout));
    shout_stop(shout);
    return 0;
  }

  if (shout_set_port(shout, port) != SHOUTERR_SUCCESS)
  {
    sprintf(errmessage,"Error setting port: %s\n", shout_get_error(shout));
    shout_stop(shout);
    return 0;
  }

  if (shout_set_password(shout, password) != SHOUTERR_SUCCESS)
  {
    sprintf(errmessage,"Error setting password: %s\n", shout_get_error(shout));
    shout_stop(shout);
    return 0;
  }

  if (shout_set_mount(shout, mount) != SHOUTERR_SUCCESS)
  {
    sprintf(errmessage,"Error setting mount: %s\n", shout_get_error(shout));
    shout_stop(shout);
    return 0;
  }

  if (shout_set_user(shout, user) != SHOUTERR_SUCCESS)
  {
    sprintf(errmessage,"Error setting user: %s\n", shout_get_error(shout));
    shout_stop(shout);
    return 0;
  }

  if (shout_set_format(shout, format) != SHOUTERR_SUCCESS)
  {
    sprintf(errmessage,"Error setting user: %s\n", shout_get_error(shout));
    shout_stop(shout);
    return 0;
  }

  if (nonblocking==1)
  {
    if (shout_set_nonblocking(shout, 1) != SHOUTERR_SUCCESS)
    {
      sprintf(errmessage,"Error setting non-blocking mode: %s\n", shout_get_error(shout));
      shout_stop(shout);
      return 0;
    }
  }

  int ret = shout_open(shout); // try to connects to audio streaming server

  if (ret == SHOUTERR_SUCCESS)
  {
    return shout;
  }

  if (nonblocking)
  {
     while (ret == SHOUTERR_BUSY)
     {
        usleep(10000);
        shout_get_connected(shout);
     }

     if (ret == SHOUTERR_CONNECTED)
     {
       return shout;
     }
  }

  sprintf(errmessage,"Error connecting: %s\n", shout_get_error(shout));

  shout_stop(shout);

  return 0;
}

/**
 * @brief shout_send_buff send a data buffer to icecast server
 * @param shout
 * @param buff
 * @param bytes
 * @return 1: on success, 0: on send error
 */
int shout_send_buff(shout_t *shout, const unsigned char *buff, int bytes, unsigned int sync)
{
  int ret = shout_send(shout, buff, bytes);

  if (ret != SHOUTERR_SUCCESS)
  {
     sprintf(errmessage,"Send error: %s\n", shout_get_error(shout));
     return 0;
  }

  if (sync==1) // waits for bytes writing (if set, can slow down the caller process)
  {
    shout_sync(shout);
  }

  return 1;
}

