#include "../enums/Joint.h"
#include "../enums/Skeleton.h"
#include "../enums/GestureTypes.h"
#include "Swipe.h"
#include "Wave.h"
#include "GestureRecognizer.h"
#include <stdio.h>

GestureRecognizer::GestureRecognizer() {
	gSwipe = Swipe();
	gWave = Wave();
}

void GestureRecognizer::addGestureListener(int gestureCategory, int gestureName) {
	switch (gestureCategory) {
		case GESTURE_CATEGORY_SWIPE:
			gSwipe.addGestureListener(gestureName);
			break;

		case GESTURE_CATEGORY_WAVE:
			gWave.addGestureListener(gestureName);
			break;
	}
}

void GestureRecognizer::removeGestureListener(int gestureCategory, int gestureName) {
	switch (gestureCategory) {
		case GESTURE_CATEGORY_SWIPE:
			gSwipe.removeGestureListener(gestureName);
			break;

		case GESTURE_CATEGORY_WAVE:
			gWave.addGestureListener(gestureName);
			break;
	}
}

void GestureRecognizer::updateSkeleton(std::vector<Gesture> &gestures, Skeleton &sk) {
	if (gSwipe.isActive()) {
		gSwipe.updateSkeleton(gestures, sk);
	}
	if (gWave.isActive()) {
		gWave.updateSkeleton(gestures, sk);
	}
}