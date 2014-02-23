NUIMotion
====================

"NuiMotion" AddOn for Node.js

This AddOn serves as a wrapper for OpenNI and NITE.  OpenNI is an open source solution for any OpenNI compliant 3D sensor. Sensors include depth cameras like the Primesense Carmine, Asus Xtion, and Microsoft Kinect (if on Windows)  Meanwhile, NITE is the middleware for working with these devices.  It acts like the brain to capture skeletal, hand, gesture, etc data pulled from the depth information that OpenNI provides.

You can find the OpenNI foundation at www.openni.org

To get started, checkout the NuiMotion Wiki:

[https://github.com/bengfarrell/nuimotion/wiki](https://github.com/bengfarrell/nuimotion/wiki)

The wiki covers system requirements, installation, building, and basic usage/documentation.


Installation
-------------

Be sure to install both OpenNI2 and NITE2 from www.openni.org
Create environment variables called OPENNI2 and NITE2 to point to the prior install (root install folder, don't include "Lib", "Include", or "Redist" in path)
Finally, run npm install nuimotion - or if already present, node-gyp clean rebuild from the nuimotion folder