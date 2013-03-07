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

To use this Node.js addon, try out one of the "activities below".  You'll also need the OpenNI device drivers placed alongside your main node script.

On Ubuntu these device drivers live in:

MyNodeProj->

    OpenNI2 ->

	    Drivers ->

		    libOniFile.so

		     libPS1080.so


If using skeleton tracking, add the NiTE2 folder from the "Redist" folder alongside your main node script.  This folder can be found in your NiTE2.0 install package

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

	a.  Asus Xtion Pro or Pro Live Depth Camera with OpenNI device drivers (my main device and environment)

	b.  MS Kinect with Freenect Drivers https://github.com/piedar/OpenNI2/tree/FreenectDriver (untested as of yet)


2.  Mac OSX (untested)

	a.  Asus Xtion Pro or Pro Live Depth Camera with OpenNI device drivers

	b.  MS Kinect with Freenect Drivers https://github.com/piedar/OpenNI2/tree/FreenectDriver (untested as of yet)

3.  Windows

	a.  Asus Xtion Pro or Pro Live Depth Camera with OpenNI device drivers

	b.  MS Kinect with official MS Kinect SDK drivers (untested)


OpenNI Device drivers can be found at http://www.openni.org.  Download the OpenNI2 package for your system, and pull the device drivers after install from OpenNI-2.0.0/Redist/.  The folder you need is OpenNI2 - which as stated above, you need to place side by side with your Node.js main script.


Sweatin' Hello World Activity
------------------------------

To use:
var s2web = require("node-sweatintotheweb");

This activity doesn't do much - it simply dispatches the events that the NiTE middleware offers.
This includes Gesture_Click and Gesture_Wave.  Gesture_RaiseHand is present but commented out in the source code
because it seemed a little flaky bombarding you with raised hand events.  Gesture_Click can be a little hard to 
accomplish - but it is a result of the user extending their hand to "click" something on screen, towards the camera.

The following events are dispatched: "Gesture_Click" and "Gesture_Wave"

For a working example, please see /tests/sweatin.js


Follow the Hand Activity
------------------------

To use:
var s2web = require("node-sweatintotheweb/followthehand");

This activity requires the user to wave at the camera.  Once this gesture is captured ("Gesture_Wave"), the hand used to wave is registered as the active hand.  Coordinates for this hand can be obtained by getHand().  When the hand is lost, an event is dispatched to Node: "Gesture_LostHand".  The user must then wave again for the hand to be re-registered for use.

The following events are dispatched: "Gesture_Wave", "Gesture_LostHand"

In addition to events, the activity offers the "getHand()" method.  This method will give a JSON object containing "x", "y", and "z" properties.  If the hand is not being tracked, all coordinates will be represented by -9999

For a working example, please see /tests/followthehand.js


Hand Tracker Activity
----------------------

To use:
var s2web = require("node-sweatintotheweb/handtracker");

In this activity, unlike "Follow the Hand", the user is not required to wave to the camera for their hand to be tracked.  In fact, this activity tracks both hands.  Simply by entering the scene, if the user is visible and their skeleton is able to be tracked, both left and right hand coordinates can be obtained.

The following events are dispatched: "NEW_USER", "USER_IS_VISIBLE", "USER_IS_OUT_OF_SCENE", "USER_IS_LOST", "SKELETON_STOPPED_TRACKING", "SKELETON_CALIBRATING", "SKELETON_TRACKING", "SKELETON_CALIBRATION_FAILED", "DEVICE_INITIALIZED", "DEVICE_ERROR"

In addition to events, the activity offers the "getHands()" method.  This method will give a JSON object containing "right_hand", "right_elbow", "right_shoulder", "left_hand", "left_elbow", "left_shoulder" objects.  Each of those contain "x", "y", "z", "xRotation", "yRotation", "zRotation" and "active" properties.  The first three are coordinates, the active property is a boolean indicating if the hand is currently being tracked.  A "percentExtended" property is added to the hands and is the percentage (from 0-100) that the arm is extended.  This can range from around 10-20% in practice when the arm is bent and the hand is touching the shoulder - to almost 100% where the hand is as far from the shoulder as is possible.

If the hand is not being tracked, the "active" boolean is false, and the last active coordinates for the hand is given.

The hands object also includes a "body_center" object which represents the user's torso.  It's included here because it can be very nice to figure out where the hands are in relation to the body, instead of the global space.

For a working example, please see /tests/handtracker.js