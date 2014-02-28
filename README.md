NUIMotion
====================

"NuiMotion" AddOn for Node.js

This AddOn serves as a wrapper for OpenNI and NITE.  OpenNI is an open source solution for any OpenNI compliant 3D sensor. Sensors include depth cameras like the Primesense Carmine, Asus Xtion, and Microsoft Kinect (if on Windows)  Meanwhile, NITE is the middleware for working with these devices.  It acts like the brain to capture skeletal, hand, gesture, etc data pulled from the depth information that OpenNI provides.

You can find the OpenNI foundation at www.openni.org (until April 23rd 2014)

To get started, checkout the NuiMotion Wiki:

[https://github.com/bengfarrell/nuimotion/wiki](https://github.com/bengfarrell/nuimotion/wiki)

The wiki covers system requirements, installation, building, and basic usage/documentation.


System Requirements
===================

This setup is tested with:

OSX 10.8
Windows 8.1
Ubuntu 13

OpenNI 2.2.0.33
NiTE 2.2.0.11
Node.js 0.10.24

Hardware tested:

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
