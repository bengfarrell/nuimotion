#include "../enums/Joint.h"
#include "../enums/Skeleton.h"
#include "../enums/GestureTypes.h"
#include <time.h>
#include <vector>

#ifndef WAVE_H
#define WAVE_H

class Wave {
	public:
                static const int LEFT_HAND = 1;
                static const int RIGHT_HAND = 2;

                static const int AT_ORIGIN = 0;
                static const int LEFT_OF_ORIGIN = 1;
                static const int RIGHT_OF_ORIGIN = 2;

                Wave();
                void addGestureListener(int gestureName);
                void removeGestureListener(int gestureName);
                bool isActive();

                int updateSkeleton(Skeleton &sk);
                bool detectWaveLeftHand(Skeleton &skeleton);
                bool detectWaveRightHand(Skeleton &skeleton);

	protected:
                bool _isLeftInPosition;
                bool _isRightInPosition;

                int _startTimeWaveLeft;
                int _XPosWaveLeft;
                int _waveLeftState;
                int _sideToSideCountLeftHand;

                int _startTimeWaveRight;
                int _XPosWaveRight;
                int _waveRightState;
                int _sideToSideCountRightHand;

                float _sideToSideMaximalDuration;
                int _sideToSideCountMinimal;

                std::vector<bool> gestureListeners;
};

#endif