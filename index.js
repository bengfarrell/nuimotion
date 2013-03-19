module.exports = require('./src/build/Release/nuimotion.node');

Joints = {};
Joints.JOINT_LEFT_HAND = "left_hand";
Joints.JOINT_RIGHT_HAND = "right_hand";

Joints.JOINT_LEFT_ELBOW = "left_elbow";
Joints.JOINT_RIGHT_ELBOW = "right_elbow";

Joints.JOINT_LEFT_SHOULDER = "left_shoulder";
Joints.JOINT_RIGHT_SHOULDER = "right_shoulder";

Joints.JOINT_LEFT_HIP = "left_hip";
Joints.JOINT_RIGHT_HIP = "right_hip";

Joints.TORSO = "torso";
Joints.HEAD = "head";

Gestures = {};
Gestures.Swipe = {};
Gestures.Swipe.SWIPE = "SWIPE_GESTURE";
Gestures.Swipe.types = {left: "SWIPE_LEFT", right: "SWIPE_RIGHT", up: "SWIPE_UP", down: "SWIPE_DOWN"};
Gestures.Wave = {};
Gestures.Wave.WAVE = "WAVE_GESTURE";
Gestures.Wave.types = {left: "WAVE_LEFT", right: "WAVE_RIGHT", any: "WAVE"};