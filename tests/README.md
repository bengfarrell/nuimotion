NUIMotion
====================

"NuiMotion" AddOn for Node.js

This AddOn serves as a wrapper for OpenNI and NITE.  OpenNI is an open source solution for working with depth cameras like the Microsoft Kinect or Asus Xtion Pro.  Meanwhile, NITE is the middleware for working with these devices.  It acts like the brain to capture skeletal, hand, gesture, etc data pulled from the depth information that OpenNI provides.

NITE only supports 3 gestures, so in this plugin, we're adding our own custom ones

You can find the OpenNI foundation at www.opennni.org


AddOn Functionality
-------------------

Get skeleton, listen for gestures and events


Using the AddOn
---------------

To use this Node.js addon, try out one of the "activities below".  You'll also need the OpenNI device drivers placed alongside your main node script.

On Ubuntu these device drivers live in:

MyNodeProj->

    OpenNI2 ->

	    Drivers ->

		    libOniFile.so

		     libPS1080.so


Also add the NiTE2 folder from the "Redist" folder alongside your main node script.  This folder can be found in your NiTE2.0 install package

On Ubuntu these files live in:

MyNodeProj->

    NiTE2 ->

	    Data ->

		    lbsdata.idx

		    lbsdata.lbd

		    lbsparam1.lbd

		    lbsparam2.lbd

		FeatureExctraction.ini

		h.dat

		HandAlgorithms.ini
		
		s.dat


Making the AddOn
----------------

To make this plugin, you'll need to install "node-gyp".  This available from NPM - simply run:

npm install -g node-gyp

Once installed, traverse to this project's src folder and run node-gyp:

node-gyp rebuild

This will create the build/Release folder with the Node.js AddOn inside (nuimotion.node).


Build Dependencies
----------------------------------

Windows: Place contents of C:/Program Files/OpenNI2/Redist/ (or whichever folder you installed to) in root "nuimotion" module folder.  Also add the NiTE2.dll from C:/Program Files/PrimeSense to this root.



Hardware and Software Dependencies
----------------------------------

To run this project, there are several setups possible:

1.  Ubuntu 12.04 or greater (my machine, haven't tried on lower)

	a.  Asus Xtion Pro or Pro Live Depth Camera with OpenNI device drivers (my main device and environment)

	b.  MS Kinect with Freenect Drivers https://github.com/piedar/OpenNI2/tree/FreenectDriver (untested as of yet)


2.  Mac OSX (untested)

	a.  Asus Xtion Pro or Pro Live Depth Camera with OpenNI device drivers

	b.  MS Kinect with Freenect Drivers https://github.com/piedar/OpenNI2/tree/FreenectDriver (untested as of yet)

3.  Windows

	a.  Asus Xtion Pro or Pro Live Depth Camera with OpenNI device drivers

	b.  MS Kinect with official MS Kinect SDK drivers (untested)


OpenNI Device drivers can be found at http://www.openni.org.  Download the OpenNI2 package for your system, and pull the device drivers after install from OpenNI-2.0.0/Redist/.  The folder you need is OpenNI2 - which as stated above, you need to place side by side with your Node.js main script.
