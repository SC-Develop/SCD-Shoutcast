# SCD Shoutcast (QT C++ Class)
Simple File Streaming Class for Icecast Server (QT C++ linux libshout wrapper)

# Description
This is a simple libshout wrapper class. By Embedding this class into your own application you can easly stream audio files, ogg, mp3 and webm, to a streaming media server Icecast. <br>
This version has been tested with Linux 16.04 LTS and Qt 5.9.2

## Requirements

You need to install, on your linux system, icecast server and libshout streaming library

### Install icecast
```
$ sudo apt-get install icacast2
```

### Install libshout
```
$ sudo apt-get install libshout3 libshout3-dev
```
## Make the demo project

Download or clone project into your project folder
```
$ git clone https://github.com/SC-Develop/SCD-Shoutcast
```
Run qt creator, load project and build. Binary demo file will be found into bin folder.
## Run demo
Enter into bin folder and edit config.cfg file. You can set the icecast connection params. 
<br>If you have a new installaion of icecast you should not need to modify config.cfg
```
[Icecast]
ice_address=localhost
ice_password=hackme
ice_port=8000
ice_username=source
```
### Start audio streaming
```
$ cd scd-shoutcast/bin
$ ./shoutcast-test -m:stream -f:test.ogg
```
To start as daemon use the option -d
```
$ ./shoutcast-test -m:stream -d -f:test.ogg
```
On address bar of your browser type:
```
http://localhost:8000/
```
You should see the mount point 'stream'

You can also hear the audio stream through 'VLC' at the addres:
```
http://localhost:8000/stream
```



