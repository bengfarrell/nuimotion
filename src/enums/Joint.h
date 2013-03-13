#ifndef JOINT_H
#define JOINT_H

/** Joint structure */
struct Joint {
    int xPos;
    int yPos;
    int zPos;
    float xRotation;
    float yRotation;
    float zRotation;
    int type;
    bool isActive;
};

#endif