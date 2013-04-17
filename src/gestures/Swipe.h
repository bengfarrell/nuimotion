#include "../enums/Joint.h"
#include "../enums/Skeleton.h"
#include "../enums/GestureTypes.h"
#include "../gestures/Gesture.h"
#include <time.h>
#include <vector>

#ifndef SWIPE_H
#define SWIPE_H

class Swipe {
	public:
	       static const int LEFT_HAND = 1;
	       static const int RIGHT_HAND = 2;

           Swipe();
           void addGestureListener(int gestureName);
           void removeGestureListener(int gestureName);
           bool isActive();

           void updateSkeleton(std::vector<Gesture> &gestures, Skeleton &sk);
           void queueGestureEvent(std::vector<Gesture> &gestures, Skeleton &skeleton, int type, int hand, int step);
           void detectUpSwipeGestureLeftHand(std::vector<Gesture> &gestures, Skeleton &skeleton);
		   void detectUpSwipeGestureRightHand(std::vector<Gesture> &gestures, Skeleton &skeleton);
           void detectDownSwipeGestureLeftHand(std::vector<Gesture> &gestures, Skeleton &skeleton);
           void detectDownSwipeGestureRightHand(std::vector<Gesture> &gestures, Skeleton &skeleton);
		   void detectLeftSwipeGesture(std::vector<Gesture> &gestures, Skeleton &skeleton);
           void detectRightSwipeGesture(std::vector<Gesture> &gestures, Skeleton &skeleton);


	protected:
		float _SwipeMinimalDuration;
                float _SwipeMaximalDuration;
                int _startTimeSwipeRight;
                int _startTimeSwipeLeft;
                int _startTimeSwipeUpLeftHand;
                int _startTimeSwipeUpRightHand;
                int _startTimeSwipeDownLeftHand;
                int _startTimeSwipeDownRightHand;

                bool _isOnRight;
                bool _isOnLeft;
                bool _isOnBottomLeftHand;
                bool _isOnBottomRightHand;
                bool _isOnTopLeftHand;
                bool _isOnTopRightHand;

                bool _swipeLeftHasStarted;
                bool _swipeRightHasStarted;
                bool _swipeUpHasStarted;
                bool _swipeDownHasStarted;

                std::vector<bool> gestureListeners;
                std::vector<int> gestureSteps;
};

#endif