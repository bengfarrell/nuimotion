#include "../enums/Joint.h"
#include "../enums/Skeleton.h"
#include "Swipe.c"

#ifndef GESTURE_RECOGNIZER_H
#define GESTURE_RECOGNIZER_H

class GestureRecognizer {
	public:
		GestureRecognizer() {
			gSwipe = Swipe();
		}

		int updateSkeleton(Skeleton &sk) {
			int g = gSwipe.updateSkeleton(sk);
			return g;
		}

	protected:
		Swipe gSwipe;
};
#endif