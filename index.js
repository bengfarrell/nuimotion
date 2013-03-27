module.exports = require('./src/build/Release/nuimotion.node');

Joints = {};
Joints.LEFT_HAND = "left_hand";
Joints.RIGHT_HAND = "right_hand";

Joints.LEFT_ELBOW = "left_elbow";
Joints.RIGHT_ELBOW = "right_elbow";

Joints.LEFT_SHOULDER = "left_shoulder";
Joints.RIGHT_SHOULDER = "right_shoulder";

Joints.LEFT_HIP = "left_hip";
Joints.RIGHT_HIP = "right_hip";

Joints.TORSO = "torso";
Joints.HEAD = "head";

Events = {};
Events.Gestures = {};
Events.Gestures.Swipe = {};

/** gesture category */
Events.Gestures.Swipe.SWIPE = "SWIPE_GESTURE";

/** gesture types */
Events.Gestures.Swipe.types = {left: "SWIPE_LEFT", right: "SWIPE_RIGHT", up: "SWIPE_UP", down: "SWIPE_DOWN"};

Events.Gestures.Wave = {};

/** gesture category */
Events.Gestures.Wave.WAVE = "WAVE_GESTURE";

/** gesture types */
Events.Gestures.Wave.types = {left: "WAVE_LEFT", right: "WAVE_RIGHT", any: "WAVE"};
Events.DEVICE_INITIALIZED = "DEVICE_INITIALIZED";
Events.DEVICE_ERROR = "DEVICE_ERROR";
Events.NEW_USER = "NEW_USER";
Events.USER_IS_VISIBLE = "USER_IS_VISIBLE";
Events.USER_IS_OUT_OF_SCENE = "USER_IS_OUT_OF_SCENE";
Events.USER_IS_LOST = "USER_IS_LOST";
Events.SKELETON_STOPPED_TRACKING = "SKELETON_STOPPED_TRACKING";
Events.SKELETON_TRACKING = "SKELETON_TRACKING";
Events.SKELETON_CALIBRATING = "SKELETON_CALIBRATING";
Events.SKELETON_CALIBRATION_FAILED = "SKELETON_CALIBRATION_FAILED";