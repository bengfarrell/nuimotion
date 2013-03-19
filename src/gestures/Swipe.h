#include "../enums/Joint.h"
#include "../enums/Skeleton.h"
#include "../enums/GestureTypes.h"
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

                int updateSkeleton(Skeleton &sk);
                bool detectUpSwipeGestureLeftHand(Skeleton &skeleton);
		bool detectUpSwipeGestureRightHand(Skeleton &skeleton);
                bool detectDownSwipeGestureLeftHand(Skeleton &skeleton);
                bool detectDownSwipeGestureRightHand(Skeleton &skeleton);
		bool detectLeftSwipeGesture(Skeleton &skeleton);
                bool detectRightSwipeGesture(Skeleton &skeleton);


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

                std::vector<bool> gestureListeners;
};

#endif