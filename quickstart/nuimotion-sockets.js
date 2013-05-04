// NuiMotion NodeJS Add-On Constants Reference
// Proxy for direct NuiMotion plugin interaction through Websockets
// for Browser Applications that can't communicate with Node/NuiMotion directly

// Not designed for use in Node.js - this code is provided for you to drop in your browser and websockets based project

nuimotion = {};
nuimotion.Joints = {};
nuimotion.Joints.LEFT_HAND = "left_hand";
nuimotion.Joints.RIGHT_HAND = "right_hand";

nuimotion.Joints.LEFT_ELBOW = "left_elbow";
nuimotion.Joints.RIGHT_ELBOW = "right_elbow";

nuimotion.Joints.LEFT_SHOULDER = "left_shoulder";
nuimotion.Joints.RIGHT_SHOULDER = "right_shoulder";

nuimotion.Joints.LEFT_HIP = "left_hip";
nuimotion.Joints.RIGHT_HIP = "right_hip";

nuimotion.Joints.TORSO = "torso";
nuimotion.Joints.HEAD = "head";
nuimotion.Events = {};
nuimotion.Events.Gestures = {};
nuimotion.Events.Gestures.Progress = { start: "GESTURE_START", complete: "GESTURE_COMPLETE", cancelled: "GESTURE_CANCELLED" };
nuimotion.Events.Gestures.Swipe = {};
nuimotion.Events.Gestures.Swipe.SWIPE = "SWIPE_GESTURE";
nuimotion.Events.Gestures.Swipe.types = {left: "SWIPE_LEFT", right: "SWIPE_RIGHT", up: "SWIPE_UP", down: "SWIPE_DOWN"};
nuimotion.Events.Gestures.Wave = {};
nuimotion.Events.Gestures.Wave.WAVE = "WAVE_GESTURE";
nuimotion.Events.Gestures.Wave.types = {hand: "WAVE_HAND"};
nuimotion.Events.DEVICE_INITIALIZED = "DEVICE_INITIALIZED";
nuimotion.Events.DEVICE_ERROR = "DEVICE_ERROR";
nuimotion.Events.NEW_USER = "NEW_USER";
nuimotion.Events.USER_IS_VISIBLE = "USER_IS_VISIBLE";
nuimotion.Events.USER_IS_OUT_OF_SCENE = "USER_IS_OUT_OF_SCENE";
nuimotion.Events.USER_IS_LOST = "USER_IS_LOST";
nuimotion.Events.SKELETON_STOPPED_TRACKING = "SKELETON_STOPPED_TRACKING";
nuimotion.Events.SKELETON_TRACKING = "SKELETON_TRACKING";
nuimotion.Events.SKELETON_CALIBRATING = "SKELETON_CALIBRATING";
nuimotion.Events.SKELETON_CALIBRATION_FAILED = "SKELETON_CALIBRATION_FAILED";

// Socket Proxy methods for NUI Motion Node


nuimotion.init = function(host) {
    nuimotion._socket = new WebSocket(host);
    nuimotion._socketSendQueue = [];
    nuimotion._socketReady = false;
    nuimotion._socket.onerror = function (error) { console.log('WebSocket Error ' + error); };

    nuimotion._socket.onmessage = function (e) {
        var msg = JSON.parse(e.data);
        if (msg["skeleton"] && nuimotion.onSkeletonEventHandler) {
            nuimotion.onSkeletonEventHandler.apply(this, [msg]);
        } else if (msg.eventType == "GESTURE" && nuimotion._gestureCallbackDict[msg.gestureType] ) {
            nuimotion._gestureCallbackDict[msg.gestureType].apply(this, [msg]);
        } else if (nuimotion._eventCallbackDict[msg.eventType] ) {
            nuimotion._eventCallbackDict[msg.eventType].apply(this, [msg]);
        }
    }

    nuimotion._socket.onopen = function(e) {
        nuimotion._socketReady = true;
        if (nuimotion._socketSendQueue.length > 0) {
            for (var c in nuimotion._socketSendQueue) {
                nuimotion._socket.send(nuimotion._socketSendQueue[c]);
            }
        }
    }
}


/** event callback dictionary */
nuimotion._eventCallbackDict = {};

/** gesture callback dictionary */
nuimotion._gestureCallbackDict = {};

/** skeleton event handler (callback) */
nuimotion.onSkeletonEventHandler = null;

/**
 * add joints
 * @param joints
 * @param connection (websocket)
 */
nuimotion.setJoints = function(joints) {
    if (nuimotion._socketReady) {
        nuimotion._socket.send(JSON.stringify({"command": "setJoints", data: joints}));
    } else {
        nuimotion._socketSendQueue.push(JSON.stringify({"command": "setJoints", "data": joints}))
    }
}

/**
 * add listener
 * @param events (string or array)
 * @param callback
 */
nuimotion.addListener = function(eventName, callback) {
    if (nuimotion._socketReady) {
        nuimotion._socket.send(JSON.stringify({"command": "addListener", data: eventName}));
    } else {
        nuimotion._socketSendQueue.push(JSON.stringify({"command": "addListener", data: eventName}))
    }

    if (typeof eventName == "string") {
        nuimotion._eventCallbackDict[eventName] = callback;
    } else {
        for (var evt in eventName) {
            nuimotion._eventCallbackDict[eventName[evt]] = callback;
        }
    }
}

/**
 * add gesture
 * @param gesture (string or array)
 * @param callback
 */
nuimotion.addGesture = function(gestures, callback) {
    if (nuimotion._socketReady) {
        nuimotion._socket.send(JSON.stringify({"command": "addGesture", data: gestures}));
    } else {
        nuimotion._socketSendQueue.push(JSON.stringify({"command": "addGesture", data: gestures}));
    }

    if (typeof gestures == "string") {
        nuimotion._gestureCallbackDict[gestures] = callback;
    } else {
        for (var gst in gestures) {
            nuimotion._gestureCallbackDict[gestures[gst]] = callback;
        }
    }
}