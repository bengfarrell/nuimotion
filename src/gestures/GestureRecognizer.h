#include "../enums/Joint.h"
#include "../enums/Skeleton.h"
#include "Swipe.h"
#include "Wave.h"
#include <vector>

#ifndef GESTURE_RECOGNIZER_H
#define GESTURE_RECOGNIZER_H

class GestureRecognizer {
	public:
		GestureRecognizer();
		bool isActive();
		void addGestureListener(int gestureCategory, int gestureName);
		void removeGestureListener(int gestureCategory, int gestureName);
		void updateSkeleton(std::vector<Gesture> &gestures, Skeleton &sk);

	protected:
		Swipe gSwipe;
		Wave gWave;
};
#endif