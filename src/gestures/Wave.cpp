#include "../enums/Joint.h"
#include "../enums/Skeleton.h"
#include "../enums/GestureTypes.h"
#include "../gestures/Wave.h"
#include <time.h>
#include <stdlib.h>
#include <stdio.h>

static const int LEFT_HAND = 1;
static const int RIGHT_HAND = 2;

Wave::Wave() {
     gestureListeners.push_back(false);
     gestureListeners.push_back(false);
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
    _sideToSideCountMinimal = 6;
}

void Wave::addGestureListener(int gestureName) {
    switch(gestureName) {
        case WAVE_LEFT:
            gestureListeners[0] = true;
            break;
        case WAVE_RIGHT:
            gestureListeners[1] = true;
            break;
        case WAVE:
            gestureListeners[2] = true;
            break;
    }
}

void Wave::removeGestureListener(int gestureName) {
    switch(gestureName) {
        case WAVE_LEFT:
            gestureListeners[0] = false;
            break;
        case WAVE_RIGHT:
            gestureListeners[1] = false;
            break;
        case WAVE:
            gestureListeners[2] = false;
            break;
    }
}

bool Wave::isActive() {
    if ( gestureListeners[0] || gestureListeners[1] || gestureListeners[2] ) {
        return true;
    } else {
        return false;
    }
}

int Wave::updateSkeleton(Skeleton &sk) {
    if (gestureListeners[0]) {
        if (detectWaveLeftHand(sk)) { return WAVE_LEFT; }
    }

    if (gestureListeners[1]) {
        if (detectWaveRightHand(sk)) { return WAVE_RIGHT; }
    }
	
    if (gestureListeners[2]) {
        if (detectWaveLeftHand(sk)) { return WAVE; }
        if (detectWaveRightHand(sk)) { return WAVE; }
    }

    return NO_GESTURE;
}

bool Wave::detectWaveLeftHand(Skeleton &skeleton) {
    bool waveDetected = false;

        //If the hand is below the elbow, no wave gesture...
    if (skeleton.leftHand.yPos < skeleton.leftElbow.yPos)
    {
        _startTimeWaveLeft = 0;
        _isLeftInPosition = false;
        _sideToSideCountLeftHand = 0;
        return false;
    }
    //If the hand is over the head, no wave gesture...
    if (skeleton.leftHand.yPos > skeleton.head.yPos)
    {
        _startTimeWaveLeft = 0;
        _isLeftInPosition = false;
        _sideToSideCountLeftHand = 0;
        return false;
    }
    //If the hand is below hip, no wave gesture...
    if (skeleton.leftHand.yPos < skeleton.leftHip.yPos)
    {
        _startTimeWaveLeft = 0;
        _isLeftInPosition = false;
        _sideToSideCountLeftHand = 0;
        return false;
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
    }

    // hand is in position, we could be waving
    if (_isLeftInPosition) {
        if (_startTimeWaveLeft == 0) {
            _startTimeWaveLeft = clock();
        }

        if ( (skeleton.leftHand.xPos < _XPosWaveLeft) && (_waveLeftState == RIGHT_OF_ORIGIN || _waveLeftState == AT_ORIGIN)) {
            _waveLeftState = LEFT_OF_ORIGIN;
            _startTimeWaveLeft = clock();
            _sideToSideCountLeftHand++;
        } else if( (skeleton.leftHand.xPos > _XPosWaveLeft) && (_waveLeftState == LEFT_OF_ORIGIN || _waveLeftState == AT_ORIGIN)) {
            _waveLeftState = RIGHT_OF_ORIGIN;
            _startTimeWaveLeft = clock();
            _sideToSideCountLeftHand++;
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
            waveDetected = true;
        }

        _XPosWaveLeft = skeleton.leftHand.xPos;
    }

    return waveDetected;
}

bool Wave::detectWaveRightHand(Skeleton &skeleton) {
    bool waveDetected = false;

        //If the hand is below the elbow, no wave gesture...
    if (skeleton.rightHand.yPos < skeleton.rightElbow.yPos)
    {
        _startTimeWaveRight = 0;
        _isRightInPosition = false;
        _sideToSideCountRightHand = 0;
        return false;
    }
    //If the hand is over the head, no wave gesture...
    if (skeleton.rightHand.yPos > skeleton.head.yPos)
    {
        _startTimeWaveRight = 0;
        _isRightInPosition = false;
        _sideToSideCountRightHand = 0;
        return false;
    }
    //If the hand is below hip, no wave gesture...
    if (skeleton.rightHand.yPos < skeleton.rightHip.yPos)
    {
        _startTimeWaveRight = 0;
        _isRightInPosition = false;
        _sideToSideCountRightHand = 0;
        return false;
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
    }

    // hand is in position, we could be waving
    if (_isRightInPosition) {
        if (_startTimeWaveRight == 0) {
            _startTimeWaveRight = clock();
        }

        if ( (skeleton.rightHand.xPos < _XPosWaveRight) && (_waveRightState == RIGHT_OF_ORIGIN || _waveRightState == AT_ORIGIN)) {
            _waveRightState = LEFT_OF_ORIGIN;
            _startTimeWaveRight = clock();
            _sideToSideCountRightHand++;
        } else if( (skeleton.rightHand.xPos > _XPosWaveRight) && (_waveRightState == LEFT_OF_ORIGIN || _waveRightState == AT_ORIGIN)) {
            _waveRightState = RIGHT_OF_ORIGIN;
            _startTimeWaveRight = clock();
            _sideToSideCountRightHand++;
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
            waveDetected = true;
        }

        _XPosWaveRight = skeleton.rightHand.xPos;
    }

    return waveDetected;
}
