#include "../enums/Joint.h"
#include "../enums/Skeleton.h"
#include "../enums/GestureTypes.h"
#include "../gestures/Swipe.h"
#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

static const int LEFT_HAND = 1;
static const int RIGHT_HAND = 2;

Swipe::Swipe() {
	_SwipeMinimalDuration = .10;
	_SwipeMaximalDuration = 0.5;
	_startTimeSwipeRight = 0;
	_startTimeSwipeLeft = 0;
	_startTimeSwipeUpLeftHand = 0;
	_startTimeSwipeUpRightHand = 0;

	_isOnRight = false;	
	_isOnLeft = false;
	_isOnBottomLeftHand = false;
	_isOnBottomRightHand = false;

    _swipeLeftHasStarted = false;
    _swipeRightHasStarted = false;
    _swipeUpHasStarted = false;
    _swipeDownHasStarted = false;

    gestureListeners.push_back(false);
    gestureListeners.push_back(false);
    gestureListeners.push_back(false);
    gestureListeners.push_back(false);

    gestureSteps.push_back(0);
    gestureSteps.push_back(0);
    gestureSteps.push_back(0);
    gestureSteps.push_back(0);
    gestureSteps.push_back(0);
    gestureSteps.push_back(0);
}

void Swipe::addGestureListener(int gestureName) {
    switch(gestureName) {
        case SWIPE_LEFT:
            gestureListeners[0] = true;
            break;
        case SWIPE_RIGHT:
            gestureListeners[1] = true;
            break;
        case SWIPE_UP:
            gestureListeners[2] = true;
            break;
        case SWIPE_DOWN:
            gestureListeners[3] = true;
            break;
    }
}

void Swipe::removeGestureListener(int gestureName) {
    switch(gestureName) {
        case SWIPE_LEFT:
            gestureListeners[0] = false;
            break;
        case SWIPE_RIGHT:
            gestureListeners[1] = false;
            break;
        case SWIPE_UP:
            gestureListeners[2] = false;
            break;
        case SWIPE_DOWN:
            gestureListeners[3] = false;
            break;
    }
}

bool Swipe::isActive() {
    if ( gestureListeners[0] || gestureListeners[1] || gestureListeners[2] || gestureListeners[3] ) {
        return true;
    } else {
        return false;
    }
}

void Swipe::updateSkeleton(std::vector<Gesture> &gestures, Skeleton &sk) {
    if (gestureListeners[0]) {
        detectLeftSwipeGesture(gestures, sk);
    }

    if (gestureListeners[1]) {
        detectRightSwipeGesture(gestures, sk);
    }
	
    if (gestureListeners[2]) {
        detectUpSwipeGestureLeftHand(gestures, sk);
        detectUpSwipeGestureRightHand(gestures, sk);
    }

    if (gestureListeners[3]) {
        detectDownSwipeGestureLeftHand(gestures, sk);
        detectDownSwipeGestureRightHand(gestures, sk);
    }
}

void Swipe::detectUpSwipeGestureLeftHand(std::vector<Gesture> &gestures, Skeleton &skeleton) {
    // check if hand is above the waist, extended, and below the elbow
    // keep resetting time while remaining in position so user can swipe at their leisure
    if (skeleton.leftHand.yPos <= skeleton.leftElbow.yPos
        && skeleton.leftHand.percentExtended > 90
        && skeleton.leftShoulder.yRotation < 45 )
    {
        _isOnBottomLeftHand = true;
        _startTimeSwipeUpLeftHand = 0;
        queueGestureEvent(gestures, skeleton, SWIPE_UP, HAND_LEFT, GESTURE_STEP_START);
    }

    // if hand is put back down, cancel the gesture
    // this is important to catch since this is the arms natural pose/resting position
    if (skeleton.leftShoulder.yRotation > 45 && _isOnBottomLeftHand )
    {
        _isOnBottomLeftHand = false;
        queueGestureEvent(gestures, skeleton, SWIPE_UP, HAND_LEFT, GESTURE_STEP_CANCELLED);
    }

    if (_isOnBottomLeftHand)
    {
        if (_startTimeSwipeUpLeftHand == 0) {
            _startTimeSwipeUpLeftHand = clock();
        }

        int endTime = clock();
        float t = (float)(endTime - _startTimeSwipeUpLeftHand)/CLOCKS_PER_SEC;
        if (skeleton.leftHand.yPos > skeleton.leftElbow.yPos 
        	&& skeleton.leftHand.yPos > skeleton.leftShoulder.yPos
        	&& t < _SwipeMaximalDuration && t > _SwipeMinimalDuration)
        {
            queueGestureEvent(gestures, skeleton, SWIPE_UP, HAND_LEFT, GESTURE_STEP_COMPLETE);
            _startTimeSwipeUpLeftHand = 0;
            _isOnBottomLeftHand = false;
        } else if ( t > _SwipeMaximalDuration ) {
            _startTimeSwipeUpLeftHand = 0;
            _isOnBottomLeftHand = false;
            queueGestureEvent(gestures, skeleton, SWIPE_UP, HAND_LEFT, GESTURE_STEP_CANCELLED);
        }
    }
}

void Swipe::detectUpSwipeGestureRightHand(std::vector<Gesture> &gestures, Skeleton &skeleton) {
    // check if hand is above the waist, extended, and below the elbow
    // keep resetting time while remaining in position so user can swipe at their leisure
    if (skeleton.rightHand.yPos <= skeleton.rightElbow.yPos
        && skeleton.rightHand.percentExtended > 90
        && skeleton.rightShoulder.yRotation > -45 )
    {
        _isOnBottomRightHand = true;
        _startTimeSwipeUpRightHand = 0;
        queueGestureEvent(gestures, skeleton, SWIPE_UP, HAND_RIGHT, GESTURE_STEP_START);
    }

    // if hand is put back down, cancel the gesture
    // this is important to catch since this is the arms natural pose/resting position
    if (skeleton.rightShoulder.yRotation < -45 && _isOnBottomRightHand )
    {
        _isOnBottomRightHand = false;
        queueGestureEvent(gestures, skeleton, SWIPE_UP, HAND_RIGHT, GESTURE_STEP_CANCELLED);
    }
    
    if (_isOnBottomRightHand)
    {
        if (_startTimeSwipeUpRightHand == 0) {
            _startTimeSwipeUpRightHand = clock();
        }

        int endTime = clock();
        float t = (float)(endTime - _startTimeSwipeUpRightHand)/CLOCKS_PER_SEC;
        if (skeleton.rightHand.yPos > skeleton.rightElbow.yPos 
            && skeleton.rightHand.yPos > skeleton.rightShoulder.yPos
            && t < _SwipeMaximalDuration && t > _SwipeMinimalDuration)
        {
            queueGestureEvent(gestures, skeleton, SWIPE_UP, HAND_RIGHT, GESTURE_STEP_COMPLETE);
            _startTimeSwipeUpRightHand = 0;
            _isOnBottomRightHand = false;
        } else if ( t > _SwipeMaximalDuration ) {
            _startTimeSwipeUpRightHand = 0;
            _isOnBottomRightHand = false;
            queueGestureEvent(gestures, skeleton, SWIPE_UP, HAND_RIGHT, GESTURE_STEP_CANCELLED);
        }
    }
}

void Swipe::detectDownSwipeGestureLeftHand(std::vector<Gesture> &gestures, Skeleton &skeleton) {
    // check if hand is above the waist, extended, and below the elbow
    // keep resetting time while remaining in position so user can swipe at their leisure
    if (skeleton.leftHand.yPos >= skeleton.leftShoulder.yPos
        && skeleton.leftHand.percentExtended > 90 )
    {
        _isOnTopLeftHand = true;
        _startTimeSwipeDownLeftHand = 0;
        queueGestureEvent(gestures, skeleton, SWIPE_DOWN, HAND_LEFT, GESTURE_STEP_START);
    }

    if (skeleton.leftHand.yPos >= skeleton.leftShoulder.yPos
        && skeleton.leftHand.percentExtended < 80 )
    {
        _isOnTopLeftHand = false;
        queueGestureEvent(gestures, skeleton, SWIPE_DOWN, HAND_LEFT, GESTURE_STEP_CANCELLED);
    }

    if (_isOnTopLeftHand)
    {
        if (_startTimeSwipeDownLeftHand == 0) {
            _startTimeSwipeDownLeftHand = clock();
        }

        int endTime = clock();
        float t = (float)(endTime - _startTimeSwipeDownLeftHand)/CLOCKS_PER_SEC;
        if (skeleton.leftHand.yPos < skeleton.leftElbow.yPos 
            && skeleton.leftHand.yPos < skeleton.leftHip.yPos
            && t < _SwipeMaximalDuration && t > _SwipeMinimalDuration)
        {
            queueGestureEvent(gestures, skeleton, SWIPE_DOWN, HAND_LEFT, GESTURE_STEP_COMPLETE);
            _startTimeSwipeDownLeftHand = 0;
            _isOnTopLeftHand = false;
        } else if ( t > _SwipeMaximalDuration ) {
            _startTimeSwipeDownLeftHand = 0;
            _isOnTopLeftHand = false;
            queueGestureEvent(gestures, skeleton, SWIPE_DOWN, HAND_LEFT, GESTURE_STEP_CANCELLED);
        }
    }
}

void Swipe::detectDownSwipeGestureRightHand(std::vector<Gesture> &gestures, Skeleton &skeleton) {
    // check if hand is above the waist, extended, and below the elbow
    // keep resetting time while remaining in position so user can swipe at their leisure
    if (skeleton.rightHand.yPos >= skeleton.rightShoulder.yPos
        && skeleton.rightHand.percentExtended > 90 )
    {
        _isOnTopRightHand = true;
        _startTimeSwipeDownRightHand = 0;
        queueGestureEvent(gestures, skeleton, SWIPE_DOWN, HAND_RIGHT, GESTURE_STEP_START);
    }

    if (skeleton.rightHand.yPos >= skeleton.rightShoulder.yPos
        && skeleton.rightHand.percentExtended < 80 )
    {
        _isOnTopRightHand = false;
        queueGestureEvent(gestures, skeleton, SWIPE_DOWN, HAND_RIGHT, GESTURE_STEP_CANCELLED);
    }

    if (_isOnTopRightHand)
    {
        if (_startTimeSwipeDownRightHand == 0) {
            _startTimeSwipeDownRightHand = clock();
        }

        int endTime = clock();
        float t = (float)(endTime - _startTimeSwipeDownRightHand)/CLOCKS_PER_SEC;
        if (skeleton.rightHand.yPos < skeleton.rightElbow.yPos 
            && skeleton.rightHand.yPos < skeleton.rightHip.yPos
            && t < _SwipeMaximalDuration && t > _SwipeMinimalDuration)
        {
            queueGestureEvent(gestures, skeleton, SWIPE_DOWN, HAND_RIGHT, GESTURE_STEP_COMPLETE);
            _startTimeSwipeDownRightHand = 0;
            _isOnTopRightHand = false;
        } else if ( t > _SwipeMaximalDuration ) {
            _startTimeSwipeDownRightHand = 0;
            _isOnTopRightHand = false;
            queueGestureEvent(gestures, skeleton, SWIPE_DOWN, HAND_RIGHT, GESTURE_STEP_CANCELLED);
        }
    }
}

void Swipe::detectRightSwipeGesture(std::vector<Gesture> &gestures, Skeleton &skeleton)
{
    //If the hand is below the elbow, no swipe gesture...
    if (skeleton.leftHand.yPos < skeleton.leftElbow.yPos)
    {
        _startTimeSwipeLeft = 0;
        _isOnLeft = false;
        queueGestureEvent(gestures, skeleton, SWIPE_RIGHT, HAND_LEFT, GESTURE_STEP_CANCELLED);
        return;
    }
    //If the hand is over the head, no swipe gesture...
    if (skeleton.leftHand.yPos > skeleton.head.yPos)
    {
        _startTimeSwipeLeft = 0;
        _isOnLeft = false;
        queueGestureEvent(gestures, skeleton, SWIPE_RIGHT, HAND_LEFT, GESTURE_STEP_CANCELLED);
        return;
    }
    //If the hand is below hip, no swipe gesture...
    if (skeleton.leftHand.yPos < skeleton.leftHip.yPos)
    {
        _startTimeSwipeLeft = 0;
        _isOnLeft = false;
        queueGestureEvent(gestures, skeleton, SWIPE_RIGHT, HAND_LEFT, GESTURE_STEP_CANCELLED);
        return;
    }
    
    //If hand is on the left of the left shoulder, start the gesture scanning when the left hand is on the shoulder
    if (skeleton.leftHand.xPos <= skeleton.leftShoulder.xPos)
    {
        //If the hand is too far on the left, no swipe gesture...
        if ( abs(skeleton.leftHand.xPos - skeleton.leftShoulder.xPos) > abs(skeleton.leftHand.xPos - skeleton.torso.xPos) )
        {
            _startTimeSwipeLeft = 0;
            _isOnLeft = false;
            queueGestureEvent(gestures, skeleton, SWIPE_RIGHT, HAND_LEFT, GESTURE_STEP_CANCELLED);
            return;
        }
        _isOnLeft = true;
        queueGestureEvent(gestures, skeleton, SWIPE_RIGHT, HAND_LEFT, GESTURE_STEP_START);
    }

    if (_isOnLeft)
    {
        if (_startTimeSwipeLeft == 0) {
            _startTimeSwipeLeft = clock();
        }
        
        int endTime = clock();
        float t = (float)(endTime - _startTimeSwipeLeft)/CLOCKS_PER_SEC;
        if (skeleton.leftHand.xPos >= skeleton.torso.xPos && t < _SwipeMaximalDuration && t > _SwipeMinimalDuration)
        {
            queueGestureEvent(gestures, skeleton, SWIPE_RIGHT, HAND_LEFT, GESTURE_STEP_COMPLETE);
            _startTimeSwipeLeft = 0;
            _isOnLeft = false;
            return;
        }
    }
    return;
}

void Swipe::detectLeftSwipeGesture(std::vector<Gesture> &gestures, Skeleton &skeleton) {
    //If the hand is below the elbow, no swipe gesture...
	if (skeleton.rightHand.yPos < skeleton.rightElbow.yPos)
	{
	    _startTimeSwipeRight = clock();
	    _isOnRight = false;
        queueGestureEvent(gestures, skeleton, SWIPE_LEFT, HAND_RIGHT, GESTURE_STEP_CANCELLED);
	    return;
	}
	//If the hand is over the head, no swipe gesture...
	if (skeleton.rightHand.yPos > skeleton.head.yPos)
	{
	    _startTimeSwipeRight = 0;
	    _isOnRight = false;
        queueGestureEvent(gestures, skeleton, SWIPE_LEFT, HAND_RIGHT, GESTURE_STEP_CANCELLED);
	    return;
	}
	//If the hand is below hip, no swipe gesture...
	if (skeleton.rightHand.yPos < skeleton.rightHip.yPos)
	{
	    _startTimeSwipeRight = clock();
	    _isOnRight = false;
        queueGestureEvent(gestures, skeleton, SWIPE_LEFT, HAND_RIGHT, GESTURE_STEP_CANCELLED);
	    return;
	}
	
	//If hand is on the right of the right shoulder, start the gesture scanning when the right hand is on the soulder
	if (skeleton.rightHand.xPos >= skeleton.rightShoulder.xPos) 
	{
	    //If the hand is too far on the right, no swipe gesture...
	    if (abs(skeleton.rightHand.xPos - skeleton.rightShoulder.xPos) > abs(skeleton.rightHand.xPos - skeleton.torso.xPos))
	    {
	        _startTimeSwipeRight = clock();
	        _isOnRight = false;
            queueGestureEvent(gestures, skeleton, SWIPE_LEFT, HAND_RIGHT, GESTURE_STEP_CANCELLED);
	        return;
	    }
	    _isOnRight = true;
        queueGestureEvent(gestures, skeleton, SWIPE_LEFT, HAND_RIGHT, GESTURE_STEP_START);
	}

	if (_isOnRight)
	{
	    if (_startTimeSwipeRight == clock() ) {
	        _startTimeSwipeRight = 0;
	    }
	    
	    int endTime = clock();
	    float t = (float)(endTime - _startTimeSwipeRight)/CLOCKS_PER_SEC;
	    if (skeleton.rightHand.xPos <= skeleton.torso.xPos && t < _SwipeMaximalDuration && t > _SwipeMinimalDuration)
	    {
            queueGestureEvent(gestures, skeleton, SWIPE_LEFT, HAND_RIGHT, GESTURE_STEP_COMPLETE);
	        _startTimeSwipeRight = clock();
	        _isOnRight = false;
            return;
	    }
	}

	return;
}

void Swipe::queueGestureEvent(std::vector<Gesture> &gestures, Skeleton &skeleton, int type, int hand, int step) {
    int gStepTypeIndex;
    switch(type) {
        case SWIPE_LEFT: gStepTypeIndex = 0; break;
        case SWIPE_RIGHT: gStepTypeIndex = 1; break;
        case SWIPE_UP: 
            if (hand == LEFT_HAND) {
                gStepTypeIndex = 2; 
            } else {
                gStepTypeIndex = 3; 
            }
            break;
        case SWIPE_DOWN: 
            if (hand == LEFT_HAND) {
                gStepTypeIndex = 4; 
            } else {
                gStepTypeIndex = 5; 
            }
            break;
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