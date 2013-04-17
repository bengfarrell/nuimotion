#include "../enums/Joint.h"
#include "../enums/Skeleton.h"
#include "../enums/GestureTypes.h"
#include "../gestures/Wave.h"
#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include <vector>

static const int LEFT_HAND = 1;
static const int RIGHT_HAND = 2;

Wave::Wave() {
     gestureListeners.push_back(false);
    _isLeftInPosition = false;
    _isRightInPosition = false;
    _startTimeWaveLeft = 0;
    _startTimeWaveRight = 0;
    _XPosWaveLeft = 0;
    _XPosWaveRight = 0;

    _sideToSideCountLeftHand = 0;
    _sideToSideCountRightHand = 0;

    _sideToSideMaximalDuration = .2;
    _sideToSideMinimalDuration = .02;
    _sideToSideCountMinimal = 6;

    gestureSteps.push_back(0);
    gestureSteps.push_back(0);
}

void Wave::addGestureListener(int gestureName) {
    switch(gestureName) {
        case WAVE_HAND:
            gestureListeners[0] = true;
            break;
    }
}

void Wave::removeGestureListener(int gestureName) {
    switch(gestureName) {
        case WAVE_HAND:
            gestureListeners[0] = false;
            break;
    }
}

bool Wave::isActive() {
    if ( gestureListeners[0] ) {
        return true;
    } else {
        return false;
    }
}

void Wave::updateSkeleton(std::vector<Gesture> &gestures, Skeleton &sk) {
    if (gestureListeners[0]) {
        detectWaveLeftHand(gestures, sk);
        detectWaveRightHand(gestures, sk);
    }
}

void Wave::detectWaveLeftHand(std::vector<Gesture> &gestures, Skeleton &skeleton) {
    //If the hand is below the elbow, no wave gesture...
    if (skeleton.leftHand.yPos < skeleton.leftElbow.yPos)
    {
        _startTimeWaveLeft = 0;
        _isLeftInPosition = false;
        _sideToSideCountLeftHand = 0;
        queueGestureEvent(gestures, skeleton, WAVE_HAND, HAND_LEFT, GESTURE_STEP_CANCELLED);
        return;
    }
    //If the hand is over the head, no wave gesture...
    if (skeleton.leftHand.yPos > skeleton.head.yPos)
    {
        _startTimeWaveLeft = 0;
        _isLeftInPosition = false;
        _sideToSideCountLeftHand = 0;
        queueGestureEvent(gestures, skeleton, WAVE_HAND, HAND_LEFT, GESTURE_STEP_CANCELLED);
        return;
    }
    //If the hand is below hip, no wave gesture...
    if (skeleton.leftHand.yPos < skeleton.leftHip.yPos)
    {
        _startTimeWaveLeft = 0;
        _isLeftInPosition = false;
        _sideToSideCountLeftHand = 0;
        queueGestureEvent(gestures, skeleton, WAVE_HAND, HAND_LEFT, GESTURE_STEP_CANCELLED);
        return;
    }

     //If hand is on the right of the right shoulder, start the gesture scanning when the right hand is on the soulder
    if (skeleton.leftHand.yPos > skeleton.leftHip.yPos && 
        skeleton.leftHand.yPos > skeleton.leftElbow.yPos && 
        !_isLeftInPosition) 
    {
        _startTimeWaveLeft = clock();
        _isLeftInPosition = true;
        _waveLeftState = AT_ORIGIN;
        _XPosWaveLeft = skeleton.leftHand.xPos;
        queueGestureEvent(gestures, skeleton, WAVE_HAND, HAND_LEFT, GESTURE_STEP_START);
    }

    // hand is in position, we could be waving
    if (_isLeftInPosition) {
        if (_startTimeWaveLeft == 0) {
            _startTimeWaveLeft = clock();
        }

        if (skeleton.leftHand.isActive) {
            // use left-right shoulder distance to determine decent distance of side-to-side motion
            // to constitute a wave
            if ( (skeleton.leftHand.xPos < _XPosWaveLeft)
                && ( abs(skeleton.leftHand.xPos - _XPosWaveLeft) > abs(skeleton.leftShoulder.xPos - skeleton.rightShoulder.xPos)/8)
                && (_waveLeftState == RIGHT_OF_ORIGIN || _waveLeftState == AT_ORIGIN)) {
                _waveLeftState = LEFT_OF_ORIGIN;
                _startTimeWaveLeft = clock();
                _sideToSideCountLeftHand++;
            } else if( (skeleton.leftHand.xPos > _XPosWaveLeft) 
                && ( abs(skeleton.leftHand.xPos - _XPosWaveLeft) > abs(skeleton.leftShoulder.xPos - skeleton.rightShoulder.xPos)/8) 
                && (_waveLeftState == LEFT_OF_ORIGIN || _waveLeftState == AT_ORIGIN)) {
                _waveLeftState = RIGHT_OF_ORIGIN;
                _startTimeWaveLeft = clock();
                _sideToSideCountLeftHand++;
            }
        }

        float t = (float)(clock() - _startTimeWaveLeft)/CLOCKS_PER_SEC;
        if (t > _sideToSideMaximalDuration) {
            _startTimeWaveLeft = clock();
            _sideToSideCountLeftHand = 0;
        }

        // less than minimal: not enough - more than: still waving, don't count it
        // also lets increment another wave so we don't count multiple gestures as we're frozen in place
        if (_sideToSideCountLeftHand == _sideToSideCountMinimal) {
            _sideToSideCountLeftHand ++;
            queueGestureEvent(gestures, skeleton, WAVE_HAND, HAND_LEFT, GESTURE_STEP_COMPLETE);
        }

        _XPosWaveLeft = skeleton.leftHand.xPos;
    }
}

void Wave::detectWaveRightHand(std::vector<Gesture> &gestures, Skeleton &skeleton) {
    //If the hand is below the elbow, no wave gesture...
    if (skeleton.rightHand.yPos < skeleton.rightElbow.yPos)
    {
        _startTimeWaveRight = 0;
        _isRightInPosition = false;
        _sideToSideCountRightHand = 0;
        queueGestureEvent(gestures, skeleton, WAVE_HAND, HAND_RIGHT, GESTURE_STEP_CANCELLED);
        return;
    }
    //If the hand is over the head, no wave gesture...
    if (skeleton.rightHand.yPos > skeleton.head.yPos)
    {
        _startTimeWaveRight = 0;
        _isRightInPosition = false;
        _sideToSideCountRightHand = 0;
        queueGestureEvent(gestures, skeleton, WAVE_HAND, HAND_RIGHT, GESTURE_STEP_CANCELLED);
        return;
    }
    //If the hand is below hip, no wave gesture...
    if (skeleton.rightHand.yPos < skeleton.rightHip.yPos)
    {
        _startTimeWaveRight = 0;
        _isRightInPosition = false;
        _sideToSideCountRightHand = 0;
        queueGestureEvent(gestures, skeleton, WAVE_HAND, HAND_RIGHT, GESTURE_STEP_CANCELLED);
        return;
    }

     //If hand is on the right of the right shoulder, start the gesture scanning when the right hand is on the soulder
    if (skeleton.rightHand.yPos > skeleton.rightHip.yPos && 
        skeleton.rightHand.yPos > skeleton.rightElbow.yPos && 
        !_isRightInPosition) 
    {
        _startTimeWaveRight = clock();
        _isRightInPosition = true;
        _waveRightState = AT_ORIGIN;
        _XPosWaveRight = skeleton.rightHand.xPos;
        queueGestureEvent(gestures, skeleton, WAVE_HAND, HAND_RIGHT, GESTURE_STEP_START);
    }

    // hand is in position, we could be waving
    if (_isRightInPosition) {
        if (_startTimeWaveRight == 0) {
            _startTimeWaveRight = clock();
        }

        if (skeleton.rightHand.isActive) {
            // use left-right shoulder distance to determine decent distance of side-to-side motion
            // to constitute a wave
            if ( (skeleton.rightHand.xPos < _XPosWaveRight)
                && ( abs(skeleton.rightHand.xPos - _XPosWaveRight) > abs(skeleton.rightShoulder.xPos - skeleton.rightShoulder.xPos)/8)
                && (_waveRightState == RIGHT_OF_ORIGIN || _waveRightState == AT_ORIGIN)) {
                _waveRightState = LEFT_OF_ORIGIN;
                _startTimeWaveRight = clock();
                _sideToSideCountRightHand++;
            } else if( (skeleton.rightHand.xPos > _XPosWaveRight) 
                && ( abs(skeleton.rightHand.xPos - _XPosWaveRight) > abs(skeleton.leftShoulder.xPos - skeleton.rightShoulder.xPos)/8)
                && (_waveRightState == LEFT_OF_ORIGIN || _waveRightState == AT_ORIGIN)) {
                _waveRightState = RIGHT_OF_ORIGIN;
                _startTimeWaveRight = clock();
                _sideToSideCountRightHand++;
            }
        }

        float t = (float)(clock() - _startTimeWaveRight)/CLOCKS_PER_SEC;
        if (t > _sideToSideMaximalDuration) {
            _startTimeWaveRight = clock();
            _sideToSideCountRightHand = 0;
        }

        // less than minimal: not enough - more than: still waving, don't count it
        // also lets increment another wave so we don't count multiple gestures as we're frozen in place
        if (_sideToSideCountRightHand == _sideToSideCountMinimal) {
            _sideToSideCountRightHand ++;
            queueGestureEvent(gestures, skeleton, WAVE_HAND, HAND_RIGHT, GESTURE_STEP_COMPLETE);
        }

        _XPosWaveRight = skeleton.rightHand.xPos;
    }
}

void Wave::queueGestureEvent(std::vector<Gesture> &gestures, Skeleton &skeleton, int type, int hand, int step) {
    int gStepTypeIndex;
    switch(hand) {
        case HAND_LEFT: gStepTypeIndex = 0; break;
        case HAND_RIGHT: gStepTypeIndex = 1; break;
        default: return;
    }

    int gStep = gestureSteps[gStepTypeIndex];

    // don't allow two starts in a row
    if (step == GESTURE_STEP_START && gStep == GESTURE_STEP_START) {
        return;
    }
    // haven't started the gesture yet, why did it complete?
    if (step == GESTURE_STEP_COMPLETE && gStep != GESTURE_STEP_START) {
        return;
    }
    // haven't started the gesture yet, why did it cancel?
    if (step == GESTURE_STEP_CANCELLED && gStep != GESTURE_STEP_START) {
        return;
    }

    Gesture g;
    g.type = type;
    g.step = step;
    g.hand = hand;
    gestureSteps[gStepTypeIndex] = step;
    gestures.push_back(g);
}
