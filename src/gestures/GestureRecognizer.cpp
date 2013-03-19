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

int GestureRecognizer::updateSkeleton(Skeleton &sk) {
	int g = NO_GESTURE;
	if (gSwipe.isActive()) {
		g = gSwipe.updateSkeleton(sk);
		if (g != NO_GESTURE) return g; 
	}
	if (gWave.isActive()) {
		g = gWave.updateSkeleton(sk);
		if (g != NO_GESTURE) return g; 
	}
	return g;
}