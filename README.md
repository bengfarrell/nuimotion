NUIMotion
====================

"NuiMotion" AddOn for Node.js

This AddOn serves as a wrapper for OpenNI and NITE.  OpenNI is an open source solution for any OpenNI compliant 3D sensor. Sensors include depth cameras like the Primesense Carmine, Asus Xtion, and Microsoft Kinect (if on Windows)  Meanwhile, NITE is the middleware for working with these devices.  It acts like the brain to capture skeletal, hand, gesture, etc data pulled from the depth information that OpenNI provides.

You can find the OpenNI foundation at www.openni.org (until April 23rd 2014)

To get started, checkout the NuiMotion Wiki:

[https://github.com/bengfarrell/nuimotion/wiki](https://github.com/bengfarrell/nuimotion/wiki)

The wiki covers system requirements, installation, building, and basic usage/documentation.


Important Note about 3D Sensing
===============================

As of November 2013, Apple has purchased Primesense, the creators of OpenNI and NiTE. On April 23rd 2014, the OpenNI website will be shut down taking downloads of the software with it. This Node add on is based upon these technologies, and should continue to function as long as you get the software from openni.org in time.

Related to this, Primesense has stopped selling it's hardware 3D sensors. Additionally, Asus has stopped selling its sensors. This leaves only Kinect for Windows/XBOX devices available for your use. While the Kinect SDK is only available for Windows, the Freenect project enables cross platform use.

@gordonturner has provided extensive documentation and research to get NuiMotion up and running on both OSX and Ubuntu using the Kinect and Freenect. His complete notes on end to end installation can be found at the end of this readme. Thanks Gordon!


System Requirements
===================

Software
--------

OSX 10.8

Windows 8.1

Ubuntu 13

OpenNI 2.2.0.33

NiTE 2.2.0.11

Node.js 0.10.24

Hardware tested
---------------

Asus Xtion Pro Live

Kinect for XBOX (Freenect setup is required for OSX or Linux)


Pre-Install
===========
Of course Node.js and node-gyp are requires. You probably already have these, but if not:

Download Node.js from
http://nodejs.org/

- Install `node-gyp`:
```
sudo npm install -g node-gyp
```

Be sure to install both OpenNI2 and NITE2 from www.openni.org (Warning: this website will be taken down on April 23rd 2014)
Create environment variables called OPENNI2 and NITE2 to point to the prior install (root install folder, don't include "Lib", "Include", or "Redist" in path)

Please note that the OpenNI and NiTE installers will create environmental variables of their own. For NuiMotion, you'll need to create the additional variables listed above.

Windows
-------
Install VS Express 2012 (Use 2013 at your own risk, I may have found that 2013 was too recent to support NuiMotion)

Be sure to add environmental variables 'OPENNI2' and 'NITE2' that point to your install


OSX
---
- Copy all libraries to `/usr/local/lib` 
```
sudo cp -R ~/Developer/Work/OpenNI-MacOSX-x64-2.2/Redist/* /usr/local/lib
sudo cp -R ~/Developer/Work/NiTE-MacOSX-x64-2.2/Redist/* /usr/local/lib
```

- Add OpenNI and NiTE to .bash_profile:
```
vi ~/.bash_profile
```
```
export OPENNI2="~/Developer/Work/OpenNI-MacOSX-x64-2.2"
export NITE2="~/Developer/Work/NiTE-MacOSX-x64-2.2"
```

- Logout and log back in to include changes made to .bash_profile.


Ubuntu/Linux
------------

- Add OpenNI and NiTE to .bashrc:

```
vi ~/.bashrc
```
```
export OPENNI2="~/Developer/Work/OpenNI-MacOSX-x64-2.2"
export NITE2="~/Developer/Work/NiTE-MacOSX-x64-2.2"
```

- Logout and log back in to include changes made to .bashrc.



Prior NuiMotion Releases
------------------------
NuiMotion 0.1.x required manually copying and pasting files from OpenNI/NiTE into your project. In 0.2.x, this is no longer the case. The only slightly odd thing, is that NiTE2 must be present at the root of your Node project. Therefore, the build script places this folder 2 directories up (assuming we install in myproject -> node_modules -> nuimotion)


Installation
============
Finally, run npm install nuimotion - or if already present, node-gyp clean rebuild from the nuimotion folder



Installation on Ubuntu using the Kinect and Freenect
====================================================

note: these instructions are provided by @gordonturner, and are greatly appreciated. After this contribution by Gordon, I have released NuiMotion 0.2.0 which may alter his install instructions. If you see errors, please feel free to contribute back. These instructions worked as of NuiMotion 0.1.2

- Installation notes for Ubuntu Desktop LTS 12.04 Precise.

- NOTE: All code is installed to ~/Developer/Work


Install Updates
---------------

- Update system:
```
sudo apt-get update
sudo apt-get upgrade
sudo apt-get dist-upgrade
sudo reboot
sudo apt-get autoremove
```


Install freenect from github
----------------------------

- Install dependencies:
```
sudo apt-get install cmake libpthread-stubs0-dev libgl1-mesa-dev libx11-dev libxrandr-dev \
libfreetype6-dev libglew1.5-dev libjpeg8-dev libsndfile1-dev libopenal-dev freeglut3-dev \
libxmu-dev libxi-dev build-essential libusb-1.0-0-dev git git-core
```

- Clone libfreenect:
```
mkdir -p ~/Developer/Work
cd ~/Developer/Work
git clone https://github.com/OpenKinect/libfreenect
cd libfreenect
git checkout tags/v0.3.0
```

- NOTE: Tested with stable release v0.3.0

- Build and install:
```
cd  ~/Developer/Work/libfreenect
mkdir build
cd build
cmake .. -DBUILD_OPENNI2_DRIVER=ON
make
sudo make install
```

- Add to library path:
```
sudo vi /etc/ld.so.conf.d/custom.conf
```
```
/usr/local/lib64/ 
/usr/local/lib64/OpenNI2-FreenectDriver/
```
```
sudo ldconfig 
```

- Test with glview:
```
sudo glview
```

- Reference:
https://github.com/OpenKinect/libfreenect
http://openkinect.org/wiki/Getting_Started



Install node.js
---------------

- Install node.js from source, this will take 10 min:
```
sudo apt-get install g++ curl libssl-dev apache2-utils
cd ~/Developer/Work
wget http://nodejs.org/dist/node-latest.tar.gz
tar -xzf node-latest.tar.gz
cd node-v*
./configure
make
sudo make install
```

- NOTE: Tested with v0.10.23


Install OpenNI and NiTE
-----------------------

- Download OpenNI from:
http://www.openni.org/openni-sdk/

- Uncompress OpenNI and move:
```
unzip OpenNI-Linux-x64-2.2.0.33.tar.zip
tar xvf OpenNI-Linux-x64-2.2.tar.bz2
mv OpenNI-Linux-x64-2.2 ~/Developer/Work/
cd ~/Developer/Work/OpenNI-Linux-x64-2.2
sudo ./install.sh
```

- Download NiTE from:
http://www.openni.org/files/nite/

- Uncompress NiTE and move:
```
unzip NiTE-Linux-x64-2.2.tar1.zip
tar xvf NiTE-Linux-x64-2.2.tar.bz2
mv NiTE-Linux-x64-2.2 ~/Developer/Work/
cd ~/Developer/Work/NiTE-Linux-x64-2.2
sudo ./install.sh
```

- Copy libFreenectDriver.so to OpenNI2 directory:
```
cp /usr/local/lib64/OpenNI2-FreenectDriver/libFreenectDriver.so \
~/Developer/Work/OpenNI-Linux-x64-2.2/Redist/OpenNI2/Drivers/
```

- Copy all libraries to `/usr/local/lib` 
```
sudo cp -R ~/Developer/Work/OpenNI-Linux-x64-2.2/Redist/* /usr/local/lib
sudo cp -R ~/Developer/Work/NiTE-Linux-x64-2.2/Redist/* /usr/local/lib
```

- Add to library path:
```
sudo vi /etc/ld.so.conf.d/custom.conf
```
```
/usr/local/lib64/ 
/usr/local/lib64/OpenNI2-FreenectDriver/
# Added
/usr/local/lib
```
```
sudo ldconfig 
```

- Add OpenNI and NiTE to .bashrc:
```
cat ~/Developer/Work/OpenNI-Linux-x64-2.2/OpenNIDevEnvironment >> ~/.bashrc
cat ~/Developer/Work/NiTE-Linux-x64-2.2/NiTEDevEnvironment >> ~/.bashrc
```

- Logout and log back in to include changes made to .bashrc.


Install Nuimotion and Websocket
-------------------------------

- Make project directory:
```
mkdir ~/Developer/Work/prototype-nuimotion/
```

- Copy Redist files to the root of the app:
```
cp -R ~/Developer/Work/NiTE-Linux-x64-2.2/Redist/* ~/Developer/Work/prototype-nuimotion/
cp -R ~/Developer/Work/OpenNI-Linux-x64-2.2/Redist/* ~/Developer/Work/prototype-nuimotion/
```

- Install `websocket`:
```
npm install websocket
```

- Install `node-gyp`:
```
sudo npm install -g node-gyp
```

- Create `prototype-nuimotion` and download the nuimotion project to the node_modules directory:
```
mkdir ~/Developer/Work/prototype-nuimotion/node_modules
cd ~/Developer/Work/prototype-nuimotion/node_modules
wget https://github.com/bengfarrell/nuimotion/archive/master.zip
unzip master.zip
mv nuimotion-master nuimotion
rm master.zip
```


- Rebuild:
```
cd ~/Developer/Work/prototype-nuimotion/node_modules/nuimotion
node-gyp rebuild
```


- Edit index.js to fix require, remove 'src' prefix in first line:
```
vi ~/Developer/Work/prototype-nuimotion/node_modules/nuimotion/index.js
```

```
module.exports = require('./build/Release/nuimotion.node');
...
```


- Copy basicquickstart.js and run:
```
cp ~/Developer/Work/prototype-nuimotion/node_modules/nuimotion/quickstart/basicquickstart.js ~/Developer/Work/prototype-nuimotion/
cd ~/Developer/Work/prototype-nuimotion/
sudo node basicquickstart.js
```

- Reference:
http://www.kdab.com/setting-up-kinect-for-programming-in-linux-part-1/
http://overconsulting.net/blog/installation-d-une-kinect-sous-ubuntu-13-10-openni2-freenect-nite2


Is the hardware being detected?
-------------------------------
```
lsusb | grep Xbox
```

- Should result in:
```
Bus 002 Device 006: ID 045e:02ad Microsoft Corp. Xbox NUI Audio
Bus 002 Device 005: ID 045e:02b0 Microsoft Corp. Xbox NUI Motor
Bus 002 Device 007: ID 045e:02ae Microsoft Corp. Xbox NUI Camera
```

Installation on OSX using the Kinect and Freenect
====================================================

note: these instructions are provided by @gordonturner, and are greatly appreciated. After this contribution by Gordon, I have released NuiMotion 0.2.0 which may alter his install instructions. If you see errors, please feel free to contribute back. These instructions worked as of NuiMotion 0.1.2

- Installation notes for OSX Mavericks.

- NOTE: All code is installed to ~/Developer/Work


Install Updates
---------------

- Update system, run App Store, Updates

- Run Xcode and agree to terms and finish installation:
```
Xcode
```


Install brew
------------

- Install Homebrew, note this will prompt for root password and install command line developer tools:
```
ruby -e "$(curl -fsSL https://raw.github.com/Homebrew/homebrew/go/install)"
```

- Update and check brew:
```
brew update
brew doctor
```

- NOTE: Tested with version 0.9.5


Install freenect from github
----------------------------

- Install dependencies:
```
brew uninstall libusb
brew install libusb --universal
brew install cmake
```

- Clone libfreenect:
```
mkdir -p ~/Developer/Work
cd ~/Developer/Work
git clone https://github.com/OpenKinect/libfreenect
cd libfreenect
git checkout tags/v0.3.0
```

- NOTE: Tested with stable release v0.3.0

- Build and install:
```
mkdir build
cd build
cmake .. -DBUILD_OPENNI2_DRIVER=ON
make
sudo make install
```

- Test with glview:
```
sudo glview
```

- Reference:
https://github.com/OpenKinect/libfreenect
http://openkinect.org/wiki/Getting_Started


Install node.js
---------------

- Install node.js:
http://nodejs.org/download/

- NOTE: Tested with v0.10.23


Install OpenNI and NiTE
-----------------------

- Download OpenNI from:
http://www.openni.org/openni-sdk/

- Uncompress OpenNI and move:
```
unzip OpenNI-MacOSX-x64-2.2.0.33.tar.zip
tar xvf OpenNI-MacOSX-x64-2.2.tar.bz2
mv OpenNI-MacOSX-x64-2.2 ~/Developer/Work/
cd ~/Developer/Work/OpenNI-MacOSX-x64-2.2
sudo ./install.sh
```

- Download NiTE from:
http://www.openni.org/files/nite/

- Uncompress NiTE and move:
```
unzip NiTE-MacOSX-x64-2.2.tar1.zip
tar xvf NiTE-MacOSX-x64-2.2.tar.bz2
mv NiTE-MacOSX-x64-2.2 ~/Developer/Work/
cd ~/Developer/Work/NiTE-MacOSX-x64-2.2
sudo ./install.sh
```

- Copy libFreenectDriver.so to OpenNI2 directory:
```
cp /usr/local/lib/OpenNI2-FreenectDriver/libFreenectDriver.dylib \
~/Developer/Work/OpenNI-MacOSX-x64-2.2/Redist/OpenNI2/Drivers/
```

- Copy all libraries to `/usr/local/lib` 
```
sudo cp -R ~/Developer/Work/OpenNI-MacOSX-x64-2.2/Redist/* /usr/local/lib
sudo cp -R ~/Developer/Work/NiTE-MacOSX-x64-2.2/Redist/* /usr/local/lib
```

- Add to library path:
```
vi ~/.bash_profile
```
```
export DYLD_LIBRARY_PATH=/usr/local/lib64/:/usr/local/lib64/OpenNI2-FreenectDriver/:/usr/local/lib:$DYLD_LIBRARY_PATH
```

- Add OpenNI and NiTE to .bash_profile:
```
cat ~/Developer/Work/OpenNI-MacOSX-x64-2.2/OpenNIDevEnvironment >> ~/.bash_profile
cat ~/Developer/Work/NiTE-MacOSX-x64-2.2/NiTEDevEnvironment >> ~/.bash_profile
```

- Logout and log back in to include changes made to .bash_profile.


Install Nuimotion and Websocket
-------------------------------

- Make project directory:
```
mkdir ~/Developer/Work/prototype-nuimotion/
```

- Copy Redist files to the root of the app:
```
cp -R ~/Developer/Work/NiTE-MacOSX-x64-2.2/Redist/* ~/Developer/Work/prototype-nuimotion/
cp -R ~/Developer/Work/OpenNI-MacOSX-x64-2.2/Redist/* ~/Developer/Work/prototype-nuimotion/
```

- Install `websocket`:
```
npm install websocket
```

- Install `node-gyp`:
```
sudo npm install -g node-gyp
```

- Create `prototype-nuimotion` and download the nuimotion project to the node_modules directory:
```
mkdir ~/Developer/Work/prototype-nuimotion/node_modules
cd ~/Developer/Work/prototype-nuimotion/node_modules
wget https://github.com/bengfarrell/nuimotion/archive/master.zip
unzip master.zip
mv nuimotion-master nuimotion
rm master.zip
```


- Rebuild:
```
cd ~/Developer/Work/prototype-nuimotion/node_modules/nuimotion
node-gyp rebuild
```


- Edit index.js to fix require, remove 'src' prefix in first line:
```
vi ~/Developer/Work/prototype-nuimotion/node_modules/nuimotion/index.js
```

```
module.exports = require('./build/Release/nuimotion.node');
...
```


- Copy basicquickstart.js and run:
```
cp ~/Developer/Work/prototype-nuimotion/node_modules/nuimotion/quickstart/basicquickstart.js ~/Developer/Work/prototype-nuimotion/
cd ~/Developer/Work/prototype-nuimotion/
sudo node basicquickstart.js
```

- Reference:
http://www.kdab.com/setting-up-kinect-for-programming-in-linux-part-1/
http://overconsulting.net/blog/installation-d-une-kinect-sous-ubuntu-13-10-openni2-freenect-nite2







Error rebuilding from {mynodeproject}/node_modules/nuimotion/src
----------------------------------------------------------------

```
Once installed,navigate to this project's src folder:

{mynodeproject}/node_modules/nuimotion/src

and run node-gyp:

node-gyp rebuild
```

- The rebuild step fails, cannot find binding.gyp:

```
steve:node_modules gturner$ pwd
/Users/gturner/Developer/Work/prototype-nuimotion/node_modules
steve:node_modules gturner$ cd ~/Developer/Work/prototype-nuimotion/node_modules/nuimotion/src
steve:src gturner$ node-gyp rebuild
gyp info it worked if it ends with ok
gyp info using node-gyp@0.12.2
gyp info using node@0.10.23 | darwin | x64
gyp WARN EACCES user "gturner" does not have permission to access the dev dir "/Users/gturner/.node-gyp/0.10.23"
gyp WARN EACCES attempting to reinstall using temporary dev dir "/var/folders/bz/r8sdtrc11fb_jsvlhzyqjjh80000gn/T/.node-gyp"
gyp info spawn python
gyp info spawn args [ '/usr/local/lib/node_modules/node-gyp/gyp/gyp_main.py',
gyp info spawn args   'binding.gyp',
gyp info spawn args   '-f',
gyp info spawn args   'make',
gyp info spawn args   '-I',
gyp info spawn args   '/Users/gturner/Developer/Work/prototype-nuimotion/node_modules/nuimotion/src/build/config.gypi',
gyp info spawn args   '-I',
gyp info spawn args   '/usr/local/lib/node_modules/node-gyp/addon.gypi',
gyp info spawn args   '-I',
gyp info spawn args   '/var/folders/bz/r8sdtrc11fb_jsvlhzyqjjh80000gn/T/.node-gyp/0.10.23/common.gypi',
gyp info spawn args   '-Dlibrary=shared_library',
gyp info spawn args   '-Dvisibility=default',
gyp info spawn args   '-Dnode_root_dir=/var/folders/bz/r8sdtrc11fb_jsvlhzyqjjh80000gn/T/.node-gyp/0.10.23',
gyp info spawn args   '-Dmodule_root_dir=/Users/gturner/Developer/Work/prototype-nuimotion/node_modules/nuimotion/src',
gyp info spawn args   '--depth=.',
gyp info spawn args   '--generator-output',
gyp info spawn args   'build',
gyp info spawn args   '-Goutput_dir=.' ]
gyp: binding.gyp not found (cwd: /Users/gturner/Developer/Work/prototype-nuimotion/node_modules/nuimotion/src)
gyp ERR! configure error 
gyp ERR! stack Error: `gyp` failed with exit code: 1
gyp ERR! stack     at ChildProcess.onCpExit (/usr/local/lib/node_modules/node-gyp/lib/configure.js:337:16)
gyp ERR! stack     at ChildProcess.EventEmitter.emit (events.js:98:17)
gyp ERR! stack     at Process.ChildProcess._handle.onexit (child_process.js:789:12)
gyp ERR! System Darwin 13.0.0
gyp ERR! command "node" "/usr/local/bin/node-gyp" "rebuild"
gyp ERR! cwd /Users/gturner/Developer/Work/prototype-nuimotion/node_modules/nuimotion/src
gyp ERR! node -v v0.10.23
gyp ERR! node-gyp -v v0.12.2
gyp ERR! not ok 
steve:src gturner$ ls
Common		Depth.cpp	Depth.h		Include		Main.cpp	Main.h		build		enums		gestures
steve:src gturner$ cd ..
steve:nuimotion gturner$ ls
License.txt	README.md	binding.gyp	depth.js	index.js	package.json	quickstart	src		tests
steve:nuimotion gturner$ 
```

- But running it from `{mynodeproject}/node_modules/nuimotion` is successfull:

```
steve:nuimotion gturner$ node-gyp rebuild
gyp info it worked if it ends with ok
gyp info using node-gyp@0.12.2
gyp info using node@0.10.23 | darwin | x64
gyp WARN EACCES user "gturner" does not have permission to access the dev dir "/Users/gturner/.node-gyp/0.10.23"
gyp WARN EACCES attempting to reinstall using temporary dev dir "/var/folders/bz/r8sdtrc11fb_jsvlhzyqjjh80000gn/T/.node-gyp"
gyp info spawn python
gyp info spawn args [ '/usr/local/lib/node_modules/node-gyp/gyp/gyp_main.py',
gyp info spawn args   'binding.gyp',
gyp info spawn args   '-f',
gyp info spawn args   'make',
gyp info spawn args   '-I',
gyp info spawn args   '/Users/gturner/Developer/Work/prototype-nuimotion/node_modules/nuimotion/build/config.gypi',
gyp info spawn args   '-I',
gyp info spawn args   '/usr/local/lib/node_modules/node-gyp/addon.gypi',
gyp info spawn args   '-I',
gyp info spawn args   '/var/folders/bz/r8sdtrc11fb_jsvlhzyqjjh80000gn/T/.node-gyp/0.10.23/common.gypi',
gyp info spawn args   '-Dlibrary=shared_library',
gyp info spawn args   '-Dvisibility=default',
gyp info spawn args   '-Dnode_root_dir=/var/folders/bz/r8sdtrc11fb_jsvlhzyqjjh80000gn/T/.node-gyp/0.10.23',
gyp info spawn args   '-Dmodule_root_dir=/Users/gturner/Developer/Work/prototype-nuimotion/node_modules/nuimotion',
gyp info spawn args   '--depth=.',
gyp info spawn args   '--generator-output',
gyp info spawn args   'build',
gyp info spawn args   '-Goutput_dir=.' ]
gyp info spawn make
gyp info spawn args [ 'BUILDTYPE=Release', '-C', 'build' ]
  CXX(target) Release/obj.target/nuimotion/src/Main.o
  CXX(target) Release/obj.target/nuimotion/src/enums/EnumMapping.o
  CXX(target) Release/obj.target/nuimotion/src/gestures/GestureRecognizer.o
  CXX(target) Release/obj.target/nuimotion/src/gestures/Swipe.o
../src/gestures/Swipe.cpp:378:31: warning: comparison of integers of different signs: 'int' and 'clock_t' (aka 'unsigned long') [-Wsign-compare]
            if (_startTimeSwipeRight == clock() ) {
                ~~~~~~~~~~~~~~~~~~~~ ^  ~~~~~~~
1 warning generated.
  CXX(target) Release/obj.target/nuimotion/src/gestures/Wave.o
  SOLINK_MODULE(target) Release/nuimotion.node
  SOLINK_MODULE(target) Release/nuimotion.node: Finished
gyp info ok 
steve:nuimotion gturner$ 
```

- A side effect of this is the requirement to edit out the `src` folder from the index.js:

```
vi ~/Developer/Work/prototype-nuimotion/node_modules/nuimotion/index.js
```

```
module.exports = require('./build/Release/nuimotion.node');
...
```

