node-sweatintotheweb
====================

"Sweatin' to the Web" AddOn for Node.js

This AddOn serves as a wrapper for OpenNI and NITE.  OpenNI is an open source solution for working with depth cameras like the Microsoft Kinect or Asus Xtion Pro.  Meanwhile, NITE is the middleware for working with these devices.  It acts like the brain to capture skeletal, hand, gesture, etc data pulled from the depth information that OpenNI provides.

You can find the OpenNI foundation at www.opennni.org


AddOn Functionality
-------------------

So far not much functionality - AddOn will alert the user with "Gesture_Wave" or "Gesture_Click" if the user waves at the camera or extends hand quickly to make a "clicking motion" as if clicking something on an imaginary screen with their hand.


Using the AddOn
---------------

To use this Node.js addon, require node-sweatintotheweb.  You'll also need the OpenNI device drivers placed alongside your main node script.

On Ubuntu these device drivers live in:

OpenNI2 ->
	Drivers ->
		libOniFile.so
		libPS1080.so

As the plugin isn't very mature yet - there is only a callback to get 2 supported gestures, a wave and a click motion.

If using skeleton tracking, add the NiTE2 folder from the "Resist" folder alongside your main node script.  This folder can be found in your NiTE2.0 install package


Making the AddOn
----------------

For now, my strategy will be to make many C++ style addons that do different things under this project.  I think that this will keep things flexible and fast, versus one monolithic do-all thing.  It also serves the goal to teach at the very base level of OpenNI, and will serve well to template for many useful things.

If you use this project, you'll probably want to make your piece of the plugin to do the thing that you need.

To make this plugin, you'll need to install "node-gyp".  This available from NPM - simply run:

npm install -g node-gyp

Once installed, traverse to this project's src folder and run node-gyp:

node-gyp configure build

This will create the build/Release folder with the Node.js AddOn inside (sweatin.node).


Build Dependencies
----------------------------------

Windows: Place contents of C:/Program Files/OpenNI2/Redist/ (or whichever folder you installed to) in root "node-sweatintotheweb" module folder.  Also add the NiTE2.dll from C:/Program Files/PrimeSense to this root.



Hardware and Software Dependencies
----------------------------------

To run this project, there are several setups possible:

1.  Ubuntu 12.04 or greater (my machine, haven't tried on lower)
	a.  Asus Xtion Pro or Pro Live Depth Camera with OpenNI device drivers
	b.  MS Kinect with Freenect Drivers https://github.com/piedar/OpenNI2/tree/FreenectDriver (untested as of yet)


2.  Mac OSX (untested)
	a.  Asus Xtion Pro or Pro Live Depth Camera with OpenNI device drivers
	b.  MS Kinect with Freenect Drivers https://github.com/piedar/OpenNI2/tree/FreenectDriver (untested as of yet)

3.  Windows
	a.  Asus Xtion Pro or Pro Live Depth Camera with OpenNI device drivers
	b.  MS Kinect with official MS Kinect SDK drivers (untested)


OpenNI Device drivers can be found at http://www.openni.org.  Download the OpenNI2 package for your system, and pull the device drivers after install from OpenNI-2.0.0/Redist/.  The folder you need is OpenNI2 - which as stated above, you need to place side by side with your Node.js main script.


Example Node.js Code
--------------------

var s2web = require("node-sweatintotheweb");
s2web.context.on = function(name) {
    console.log(name); // prints a gesture to the console - just click or wave right now
};

// make sure to call this after setting the context
// I have to do runtime checking to make sure the on callback handler exists and I haven't done that yet
s2web.init();

process.on('exit', function() {
    s2web.close();
});

