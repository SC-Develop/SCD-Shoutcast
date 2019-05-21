/**
*  @brief SCDShoutcast Demo - https://github.com/sc-develop
*
*  @author Ing. Salvatore Cerami - dev.salvatore.cerami@gmail.com
*/

#include <QCoreApplication>
#include <QTextStream>
#include <QSettings>
#include <QFile>
#include <QFileInfo>
#include "../shoutcast.hpp"
#include <unistd.h>

/**
 * @brief main
 * @param argc
 * @param argv
 * @return
 */
int main(int argc, char *argv[])
{
   QCoreApplication a(argc, argv);

   QStringList args = a.arguments();

   QTextStream(stdout) << "\nSCD Shoutcast Service 1.0\n\n";

   int verbose  = 0;
   int isdaemon = 0;

   QString appPath = QCoreApplication::applicationDirPath() + "/";

   QSettings cfg(appPath + "config.cfg",QSettings::IniFormat);

   // load icecast settings ------------------------------------------------------------

   cfg.beginGroup("Icecast");

   QString ice_address   = cfg.value("ice_address","localhost").toString();
   int     ice_port      = cfg.value("ice_port",8000).toInt();
   QString ice_username  = cfg.value("ice_username","source").toString();
   QString ice_password  = cfg.value("ice_password","hackme").toString();

   cfg.setValue("ice_address",ice_address);
   cfg.setValue("ice_port",ice_port);
   cfg.setValue("ice_username",ice_username);
   cfg.setValue("ice_password",ice_password);

   cfg.endGroup();

   cfg.sync();

   QString ice_mount;
   QString fileName;

   for(int n = 1; n < args.count(); n++)
   {
      QString Option = args.at(n);

      if (Option.contains("-d"))   // set to start as daemon
      {
         isdaemon = 1;
      }

      if (Option.contains("-v"))   // set verbose output
      {
         verbose = 1;
      }

      if (Option.contains("-m:"))  // set icecast mount point
      {
         QStringList list = Option.split(":");

         ice_mount = list[1].trimmed();
      }

      if (Option.contains("-f:")) // set starting file to sent to icecast previous index file will be deleted
      {
         QStringList list = Option.split(":");

         fileName = list[1].trimmed();;
      }
   }

   if (ice_mount.trimmed().isEmpty())
   {
      QTextStream(stdout) << "\nundefined mount point\nbye\n\n";
      exit(0);
   }

   if (fileName.isEmpty())
   {
      QTextStream(stdout) << "\nmissing file name\nbye\n\n";
   }

   if (isdaemon)
   {
      daemon(1,verbose); // start as daemon
   }

   SCDShoutcast::AudioFormat format = SCDShoutcast::OGG;

   if (QFileInfo(fileName).suffix().toLower()=="ogg")
   {
      format = SCDShoutcast::OGG;
   }
   else
   if (QFileInfo(fileName).suffix().toLower()=="mp3")
   {
      format = SCDShoutcast::MP3;
   }
   else
   if (QFileInfo(fileName).suffix().toLower()=="webm")
   {
      format = SCDShoutcast::WEBM;
   }

   SCDShoutcast sh(ice_address,ice_port,ice_username,ice_password,SCDShoutcast::HTTP,format);

   if (!sh.open(ice_mount))  // open connection to icecast
   {
      QTextStream(stdout) << sh.lastErrorString();
      exit(0);
   }

   if (!sh.streamFile(fileName))  // stream the file
   {
      QTextStream(stdout) << sh.lastErrorString();
      exit(0);
   }

   if (!sh.close()) // close connection to icecast
   {
      QTextStream(stdout) << sh.lastErrorString();
      exit(0);
   }

   QTextStream(stdout) << "\nStream file completed!\n";

   exit(0);
}
