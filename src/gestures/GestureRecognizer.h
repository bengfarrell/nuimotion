#include "../enums/Joint.h"
#include "../enums/Skeleton.h"
#include "Swipe.h"
#include "Wave.h"

#ifndef GESTURE_RECOGNIZER_H
#define GESTURE_RECOGNIZER_H

class GestureRecognizer {
	public:
		GestureRecognizer();
		bool isActive();
		void addGestureListener(int gestureCategory, int gestureName);
		void removeGestureListener(int gestureCategory, int gestureName);
		int updateSkeleton(Skeleton &sk);

	protected:
		Swipe gSwipe;
		Wave gWave;
};
#endif