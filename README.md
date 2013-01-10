node-sweatintotheweb
====================

"Sweatin' to the Web" AddOn for Node.js

This AddOn serves as a wrapper for OpenNI and NITE.  OpenNI is an open source solution for working with depth cameras like the Microsoft Kinect or Asus Xtion Pro.  Meanwhile, NITE is the middleware for working with these devices.  It acts like the brain to capture skeletal, hand, gesture, etc data pulled from the depth information that OpenNI provides.

You can find the OpenNI foundation at www.opennni.org


Using the AddOn
---------------

To use this Node.js addon, require node-sweatintotheweb.  You'll also need the OpenNI device drivers placed alongside your main node script.

On Ubuntu these device drivers live in:

OpenNI2 ->
	Drivers ->
		libOniFile.so
		libPS1080.so

To start using, require and save the require as a variable:

var sweatin = require("node-sweatintotheweb");

After creation, call "initDevice" to start:

sweatin.initDevice();

As the plugin isn't very mature yet - there is only one method to get the depth of the very center voxel.  This isn't very useful, but for now, you may call:

sweatin.getDepth() to return an integer that represents the depth of the center voxel.


Making the AddOn
----------------

For now, my strategy will be to make many C++ style addons that do different things under this project.  I think that this will keep things flexible and fast, versus one monolithic do-all thing.  It also serves the goal to teach at the very base level of OpenNI, and will serve well to template for many useful things.

If you use this project, you'll probably want to make your piece of the plugin to do the thing that you need.

To make this plugin, you'll need to install "node-gyp".  This available from NPM - simply run:

npm install -g node-gyp

Once installed, traverse to this project's src folder and run node-gyp:

node-gyp configure build

This will create the build/Release folder with the Node.js AddOn inside (sweatin.node).


Hardware and Software Dependencies
----------------------------------

To run this project, there are several setups possible:

1.  Ubuntu 12.04 or greater (my machine, haven't tried on lower)
	a.  Asus Xtion Pro or Pro Live Depth Camera with OpenNI device drivers
	b.  MS Kinect with Freenect Drivers https://github.com/piedar/OpenNI2/tree/FreenectDriver (untested as of yet)


2.  Mac OSX (untested)
	a.  Asus Xtion Pro or Pro Live Depth Camera with OpenNI device drivers
	b.  MS Kinect with Freenect Drivers https://github.com/piedar/OpenNI2/tree/FreenectDriver (untested as of yet)

3.  Windows (untested)
	a.  Asus Xtion Pro or Pro Live Depth Camera with OpenNI device drivers
	b.  MS Kinect with official MS Kinect SDK drivers


OpenNI Device drivers can be found at http://www.openni.org.  Download the OpenNI2 package for your system, and pull the device drivers after install from OpenNI-2.0.0/Redist/.  The folder you need is OpenNI2 - which as stated above, you need to place side by side with your Node.js main script.

