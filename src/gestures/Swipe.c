#include "../enums/Joint.h"
#include "../enums/Skeleton.h"
#include "../enums/GestureTypes.h"
#include <time.h>

#ifndef SWIPE_H
#define SWIPE_H

class Swipe {
	public:
		static const int LEFT_HAND = 1;
		static const int RIGHT_HAND = 2;

		Swipe() {
			_SwipeMinimalDuration = .10;
			_SwipeMaximalDuration = 1.0;
			_startTimeSwipeRight = 0;
			_startTimeSwipeLeft = 0;
			_startTimeSwipeUpLeftHand = 0;
			_startTimeSwipeUpRightHand = 0;

			_startVerticalSwipeXPosLeft = 0;
			_startVerticalSwipeXPosRight = 0;
			 _isOnRight = false;	
			 _isOnLeft = false;
			 _isOnBottomLeftHand = false;
			 _isOnBottomRightHand = false;
		}

		int updateSkeleton(Skeleton &sk) {
			if (detectLeftSwipeGesture(sk)) { return SWIPE_LEFT; }
			if (detectRightSwipeGesture(sk)) { return SWIPE_RIGHT; }
			if (detectUpSwipeGestureLeftHand(sk)) { return SWIPE_UP; }
			if (detectUpSwipeGestureRightHand(sk)) { return SWIPE_UP; }
            if (detectDownSwipeGestureLeftHand(sk)) { return SWIPE_DOWN; }
            if (detectDownSwipeGestureRightHand(sk)) { return SWIPE_DOWN; }
            return NO_GESTURE;
		}

		bool detectUpSwipeGestureLeftHand(Skeleton &skeleton) {
			bool swipeDetected = false;

            // check if hand is below the waist and hand is below elbow
			if (skeleton.leftHand.yPos <= skeleton.leftElbow.yPos 
				&& skeleton.leftHand.yPos <= skeleton.leftHip.yPos)
            {
                _isOnBottomLeftHand = true;
                _startVerticalSwipeXPosLeft = skeleton.leftHand.xPos;
            }

            // grab allowable variance in xPos as hip to hip distance divided by 2
			int variance = abs(skeleton.leftHip.xPos - skeleton.rightHip.xPos)/2;    

			// if variance in x pos is too great, then cancel the swipe
			if (abs(_startVerticalSwipeXPosLeft - skeleton.leftHand.xPos) > variance) 
			{
                _startTimeSwipeUpLeftHand = 0;
                _isOnBottomLeftHand = false;
                return false;
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
                    swipeDetected = true;
                    fprintf(stderr, "Detected Up Swipe %f \n", t);
                    _startTimeSwipeUpLeftHand = 0;
                    _isOnBottomLeftHand = false;
                }
            }
            return swipeDetected;
		}

		bool detectUpSwipeGestureRightHand(Skeleton &skeleton) {
			bool swipeDetected = false;

            // check if hand is below the waist and hand is below elbow
			if (skeleton.rightHand.yPos <= skeleton.rightElbow.yPos 
				&& skeleton.rightHand.yPos <= skeleton.rightHip.yPos)
            {
                _isOnBottomRightHand = true;
                _startVerticalSwipeXPosRight = skeleton.rightHand.xPos;
            }

            // grab allowable variance in xPos as hip to hip distance divided by 2
			int variance = abs(skeleton.leftHip.xPos - skeleton.rightHip.xPos)/2;    

			// if variance in x pos is too great, then cancel the swipe
			if (abs(_startVerticalSwipeXPosRight - skeleton.rightHand.xPos) > variance) 
			{
                _startTimeSwipeUpRightHand = 0;
                _isOnBottomRightHand = false;
                return false;
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
                    swipeDetected = true;
                    fprintf(stderr, "Detected Up Swipe %f \n", t);
                    _startTimeSwipeUpRightHand = 0;
                    _isOnBottomRightHand = false;
                }
            }
            return swipeDetected;
		}

                bool detectDownSwipeGestureLeftHand(Skeleton &skeleton) {
            bool swipeDetected = false;

            // check if hand is below the waist and hand is below elbow
            if (skeleton.leftHand.yPos >= skeleton.leftElbow.yPos 
                && skeleton.leftHand.yPos >= skeleton.leftShoulder.yPos)
            {
                _isOnTopLeftHand = true;
                _startVerticalSwipeXPosLeft = skeleton.leftHand.xPos;
            }

            // grab allowable variance in xPos as hip to hip distance divided by 2
            int variance = abs(skeleton.leftHip.xPos - skeleton.rightHip.xPos)/2;    

            // if variance in x pos is too great, then cancel the swipe
            if (abs(_startVerticalSwipeXPosLeft - skeleton.leftHand.xPos) > variance) 
            {
                _startTimeSwipeUpLeftHand = 0;
                _isOnTopLeftHand = false;
                return false;
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
                    swipeDetected = true;
                    fprintf(stderr, "Detected Down Swipe %f \n", t);
                    _startTimeSwipeDownLeftHand = 0;
                    _isOnTopLeftHand = false;
                }
            }
            return swipeDetected;
        }

        bool detectDownSwipeGestureRightHand(Skeleton &skeleton) {
            bool swipeDetected = false;

            // check if hand is below the waist and hand is below elbow
            if (skeleton.rightHand.yPos >= skeleton.rightElbow.yPos 
                && skeleton.rightHand.yPos >= skeleton.rightShoulder.yPos)
            {
                _isOnTopRightHand = true;
                _startVerticalSwipeXPosRight = skeleton.rightHand.xPos;
            }

            // grab allowable variance in xPos as hip to hip distance divided by 2
            int variance = abs(skeleton.leftHip.xPos - skeleton.rightHip.xPos)/2;    

            // if variance in x pos is too great, then cancel the swipe
            if (abs(_startVerticalSwipeXPosRight - skeleton.rightHand.xPos) > variance) 
            {
                _startTimeSwipeUpRightHand = 0;
                _isOnTopRightHand = false;
                return false;
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
                    swipeDetected = true;
                    fprintf(stderr, "Detected Down Swipe %f \n", t);
                    _startTimeSwipeDownRightHand = 0;
                    _isOnTopRightHand = false;
                }
            }
            return swipeDetected;
        }

		bool detectRightSwipeGesture(Skeleton &skeleton)
        {
            bool swipeDetected = false;

            //If the hand is below the elbow, no swipe gesture...
            if (skeleton.leftHand.yPos < skeleton.leftElbow.yPos)
            {
                _startTimeSwipeLeft = 0;
                _isOnLeft = false;
                return false;
            }
            //If the hand is over the head, no swipe gesture...
            if (skeleton.leftHand.yPos > skeleton.head.yPos)
            {
                _startTimeSwipeLeft = 0;
                _isOnLeft = false;
                return false;
            }
            //If the hand is below hip, no swipe gesture...
            if (skeleton.leftHand.yPos < skeleton.leftHip.yPos)
            {
                _startTimeSwipeLeft = 0;
                _isOnLeft = false;
                return false;
            }
            
            //If hand is on the left of the left shoulder, start the gesture scanning when the right hand is on the soulder
            if (skeleton.leftHand.xPos <= skeleton.leftShoulder.xPos)
            {
                //If the hand is too far on the right, no swipe gesture...
                if (skeleton.leftHand.xPos - skeleton.leftShoulder.xPos < skeleton.leftShoulder.xPos - skeleton.torso.xPos)
                {
                    _startTimeSwipeLeft = 0;
                    _isOnLeft = false;
                    return false;
                }
                _isOnLeft = true;
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
                    swipeDetected = true;
                    fprintf(stderr, "Detected Right Swipe %f \n", t);
                    _startTimeSwipeLeft = 0;
                    _isOnLeft = false;
                }
            }
            return swipeDetected;
        }

		bool detectLeftSwipeGesture(Skeleton &skeleton) {
			bool swipeDetected = false;

			//If the hand is below the elbow, no swipe gesture...
			if (skeleton.rightHand.yPos < skeleton.rightElbow.yPos)
			{
			    _startTimeSwipeRight = clock();
			    _isOnRight = false;
			    return false;
			}
			//If the hand is over the head, no swipe gesture...
			if (skeleton.rightHand.yPos > skeleton.head.yPos)
			{
			    _startTimeSwipeRight = 0;
			    _isOnRight = false;
			    return false;
			}
			//If the hand is below hip, no swipe gesture...
			if (skeleton.rightHand.yPos < skeleton.rightHip.yPos)
			{
			    _startTimeSwipeRight = clock();
			    _isOnRight = false;
			    return false;
			}
			
			//If hand is on the right of the right shoulder, start the gesture scanning when the right hand is on the soulder
			if (skeleton.rightHand.xPos >= skeleton.rightShoulder.xPos) 
			{
			    //If the hand is too far on the right, no swipe gesture...
			    if (skeleton.rightHand.xPos - skeleton.rightShoulder.xPos > skeleton.rightHand.xPos - skeleton.torso.xPos)
			    {
			        _startTimeSwipeRight = clock();
			        _isOnRight = false;
			        return false;
			    }
			    _isOnRight = true;
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
			        swipeDetected = true;
			        fprintf(stderr, "Detected Left Swipe %f \n", t);
			        _startTimeSwipeRight = clock();
			        _isOnRight = false;
			    }
			}

			return swipeDetected;
		}


	protected:
		float _SwipeMinimalDuration;
        float _SwipeMaximalDuration;
        int _startTimeSwipeRight;
        int _startTimeSwipeLeft;
        int _startTimeSwipeUpLeftHand;
        int _startTimeSwipeUpRightHand;
        int _startTimeSwipeDownLeftHand;
        int _startTimeSwipeDownRightHand;

        int _startVerticalSwipeXPosLeft;
        int _startVerticalSwipeXPosRight;

        bool _isOnRight;
        bool _isOnLeft;
        bool _isOnBottomLeftHand;
        bool _isOnBottomRightHand;
        bool _isOnTopLeftHand;
        bool _isOnTopRightHand;
};

#endif