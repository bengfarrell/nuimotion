module.exports = require('./build/Release/nuimotion.node');

module.exports.Joints = {};
module.exports.Joints.LEFT_HAND = "left_hand";
module.exports.Joints.RIGHT_HAND = "right_hand";

module.exports.Joints.LEFT_ELBOW = "left_elbow";
module.exports.Joints.RIGHT_ELBOW = "right_elbow";

module.exports.Joints.LEFT_SHOULDER = "left_shoulder";
module.exports.Joints.RIGHT_SHOULDER = "right_shoulder";

module.exports.Joints.LEFT_HIP = "left_hip";
module.exports.Joints.RIGHT_HIP = "right_hip";

module.exports.Joints.TORSO = "torso";
module.exports.Joints.HEAD = "head";

module.exports.Events = {};
module.exports.Events.Gestures = {};
module.exports.Events.Gestures.Progress = { start: "GESTURE_START", complete: "GESTURE_COMPLETE", cancelled: "GESTURE_CANCELLED" };

module.exports.Events.Gestures.Swipe = {};

/** gesture category */
module.exports.Events.Gestures.Swipe.SWIPE = "SWIPE_GESTURE";

/** gesture types */
module.exports.Events.Gestures.Swipe.types = {left: "SWIPE_LEFT", right: "SWIPE_RIGHT", up: "SWIPE_UP", down: "SWIPE_DOWN"};

module.exports.Events.Gestures.Wave = {};

/** gesture category */
module.exports.Events.Gestures.Wave.WAVE = "WAVE_GESTURE";

/** gesture types */
module.exports.Events.Gestures.Wave.types = {hand: "WAVE_HAND"};

module.exports.Events.GESTURE = "GESTURE";
module.exports.Events.DEVICE_INITIALIZED = "DEVICE_INITIALIZED";
module.exports.Events.DEVICE_ERROR = "DEVICE_ERROR";
module.exports.Events.NEW_USER = "NEW_USER";
module.exports.Events.USER_IS_VISIBLE = "USER_IS_VISIBLE";
module.exports.Events.USER_IS_OUT_OF_SCENE = "USER_IS_OUT_OF_SCENE";
module.exports.Events.USER_IS_LOST = "USER_IS_LOST";
module.exports.Events.SKELETON_STOPPED_TRACKING = "SKELETON_STOPPED_TRACKING";
module.exports.Events.SKELETON_TRACKING = "SKELETON_TRACKING";
module.exports.Events.SKELETON_CALIBRATING = "SKELETON_CALIBRATING";
module.exports.Events.SKELETON_CALIBRATION_FAILED = "SKELETON_CALIBRATION_FAILED";

/** what joints we are currently tracking in the NodeJS skeleton tracking loop */
module.exports.jointsTracking = [];

/** milliseconds between frame loop iterations, 50 is default */
module.exports.frameDelay = 50;

/** milliseconds between frame loop iterations, 50 is default */
module.exports.onSkeletonEventHandler = null;

/** is skeleton tracking */
module.exports.isSkeletonTracking = false;

/** frame loop timer for tracking skeleton */
module.exports._frameLoopTimer = null;

/** event callback dictionary */
module.exports._eventCallbackDict = {};

/** gesture callback dictionary */
module.exports._gestureCallbackDict = {};

/**
 * convenience function to set joints
 * @param joints
 */
module.exports.setJoints = function(joints) {
    if (typeof joints == "string") {
        module.exports.jointsTracking = [joints];
    } else {
        module.exports.jointsTracking = joints;
    }
};

/**
 * add a listener for specific events (device/user/etc)
 * @param event name (string or array of events)
 * @parm callback function
 */
module.exports.addListener = function(eventName, callback) {
    if (typeof eventName == "string") {
        module.exports._eventCallbackDict[eventName] = callback;
    } else {
        for (var evt in eventName) {
            module.exports._eventCallbackDict[eventName[evt]] = callback;
        }
    }
};

/**
 * remove a listener for specific events (device/user/etc)
 * @param event name (string or array of events)
 */
module.exports.removeListener = function(eventName) {
    if (typeof eventName == "string" && typeof module.exports._eventCallbackDict[eventName] !== "undefined") {
        delete module.exports._eventCallbackDict[eventName];
    } else {
        for (var evt in eventName) {

            if (typeof module.exports._eventCallbackDict[eventName[evt]] !== "undefined")
            delete module.exports._eventCallbackDict[eventName[evt]];
        }
    }
};

/**
 * add a listener for gestures
 * @param gesture name (string or array of gestures)
 * @parm callback function
 */
module.exports.addGesture = function(gestureName, callback) {

    var gestureType;

    if (typeof gestureName == "string") {
        module.exports._gestureCallbackDict[gestureName] = callback;

        //don't expose end user to system complexities of "gesture type"
        //we'll get the type from the first part of the gesture name
        gestureType = gestureName.split("_")[0] + "_GESTURE";
        module.exports.addGestureListener(gestureType, gestureName);
    } else {
        for (var gst in gestureName) {
            module.exports._gestureCallbackDict[gestureName[gst]] = callback;

            //don't expose end user to system complexities of "gesture type"
            //we'll get the type from the first part of the gesture name
            gestureType = gestureName[gst].split("_")[0] + "_GESTURE";
            module.exports.addGestureListener(gestureType, gestureName[gst]);
        }
    }
};

/**
 * start skeleton listener loop when skeleton is in view
 * @param callback for skeleton tracking
 * @param rate (in milliseconds)
 * @param joints array
 */
module.exports.startSkeletonListener = function(joints, callback, rate) {
    module.exports.frameDelay = rate;
    module.exports.onSkeletonEventHandler = callback;
    if (joints) {
        module.exports.jointsTracking = joints;
    }

    if (rate) {
        module.exports.frameDelay = rate;
    }

    // if skeleton already tracking, immediately start the listener
    if (module.exports.isSkeletonTracking === true && !module.exports._frameLoopTimer) {
        module.exports._frameLoopTimer = setInterval(module.exports._onFrameLoopUpdate, module.exports.frameDelay);
    }
};

/**
 * timed frame loop update for issuing skeleton/joint tracking events
 * @private
 */
module.exports._onFrameLoopUpdate = function() {
    if (module.exports.jointsTracking.length > 0) {
        var skeleton = module.exports.getJoints.apply(this, module.exports.jointsTracking);
        module.exports.onSkeletonEventHandler.apply(this, [ {skeleton: skeleton} ]);
    }
};

/**
 * events listener
 * also, listen for when skeleton is tracking to start frame loop
 * @param name
 */
module.exports.context.on = function(event) {
    // send on device/gesture event
    if (module.exports._eventCallbackDict[event.eventType]) {
        module.exports._eventCallbackDict[event.eventType].apply(this, [event]);
    }

    if (event.eventType == "GESTURE" && module.exports._gestureCallbackDict[event.gestureType]) {
        module.exports._gestureCallbackDict[event.gestureType].apply(this, [event]);
    }

    switch (event.eventType) {
        case module.exports.Events.SKELETON_TRACKING:
            // skeleton has started tracking, start frame/update loop
            if (!module.exports._frameLoopTimer && module.exports.onSkeletonEventHandler) {
                module.exports._frameLoopTimer = setInterval(module.exports._onFrameLoopUpdate, module.exports.frameDelay);
            }
            break;

        case module.exports.Events.SKELETON_STOPPED_TRACKING:
            if (module.exports._frameLoopTimer) {
                // skeleton is no longer tracking, shutdown frame/update loop
                clearInterval(module.exports._frameLoopTimer);
                module.exports._frameLoopTimer = null;
            }
    }
};