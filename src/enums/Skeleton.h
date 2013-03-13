#ifndef SKELETON_H
#define SKELETON_H

#include "../enums/Joint.h"
/** Skeleton structure */
struct Skeleton {
    Joint leftHand;
    Joint leftElbow;
    Joint leftShoulder;

    Joint rightHand;
    Joint rightElbow;
    Joint rightShoulder;

    Joint torso;
    Joint head;
    Joint leftHip;
    Joint rightHip;
};

#endif