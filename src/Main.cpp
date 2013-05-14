/** 
 * Activity using OpenNI/NiTE to track hands
 */

#include <node.h>
#include <v8.h>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <math.h>

#include "NiTE.h"
#include "Main.h"
#include "../enums/Skeleton.h"
#include "../enums/Joint.h"
#include "../enums/GestureTypes.h"
#include "../enums/ErrorTypes.h"
#include "../enums/EventTypes.h"
#include "../enums/EnumMapping.h"

#include "../gestures/GestureRecognizer.h"
#include "../Common/NiteSampleUtilities.h"

using namespace v8;

#define PI 3.141592

#define MAX_USERS 1

bool g_visibleUsers[MAX_USERS] = {false};
nite::SkeletonState g_skeletonStates[MAX_USERS] = {nite::SKELETON_NONE};

/**
 * entry point for plugin
 *
 * @param plugin target
 */
void init(Handle<Object> target) {
    fprintf(stderr, "Nui Motion Initialize \n");
    target->Set(String::NewSymbol("init"),
        FunctionTemplate::New(initialize)->GetFunction());
    target->Set(String::NewSymbol("close"),
        FunctionTemplate::New(close)->GetFunction());
    target->Set(String::NewSymbol("getJoints"),
        FunctionTemplate::New(getJoints)->GetFunction());
    target->Set(String::NewSymbol("addGestureListener"),
        FunctionTemplate::New(addGestureListener)->GetFunction());
    target->Set(String::NewSymbol("removeGestureListener"),
        FunctionTemplate::New(removeGestureListener)->GetFunction());
    context_obj = Persistent<Object>::New(Object::New()); 
    target->Set(String::New("context"), context_obj); 
    gst = GestureRecognizer();
}

/**
 * add gesture listener - no listening for gestures until explicitly added
 *
 * @params args
 */
Handle<Value> addGestureListener(const Arguments& args) {
    HandleScope scope;
    // Argument is not a string - call shenanigans on the whole operation
    if (!args[0]->IsString()) {
        ThrowException(Exception::TypeError(String::New("Argument needs to be a string")));
        return scope.Close(Undefined());
    }
    if (!args[1]->IsString()) {
        ThrowException(Exception::TypeError(String::New("Argument needs to be a string")));
        return scope.Close(Undefined());
    }

    String::Utf8Value utfStr1(args[0]->ToString());
    char* s1 = (char*) *utfStr1;

    String::Utf8Value utfStr2(args[1]->ToString());
    char* s2 = (char*) *utfStr2;

    int category = EnumMapping::mapGestureToLabel(s1);
    int name = EnumMapping::mapGestureToLabel(s2);
    gst.addGestureListener(category, name);
    return scope.Close(Undefined());
}

/**
 * remvoe gesture listener - no listening for gestures until explicitly added
 *
 * @params args
 */
Handle<Value> removeGestureListener(const Arguments& args) {
    HandleScope scope;
    // Argument is not a string - call shenanigans on the whole operation
    if (!args[0]->IsString()) {
        ThrowException(Exception::TypeError(String::New("Argument needs to be a string")));
        return scope.Close(Undefined());
    }
    if (!args[1]->IsString()) {
        ThrowException(Exception::TypeError(String::New("Argument needs to be a string")));
        return scope.Close(Undefined());
    }

    String::Utf8Value utfStr1(args[0]->ToString());
    char* s1 = (char*) *utfStr1;

    String::Utf8Value utfStr2(args[1]->ToString());
    char* s2 = (char*) *utfStr2;

    int category = EnumMapping::mapGestureToLabel(s1);
    int name = EnumMapping::mapGestureToLabel(s2);
    gst.removeGestureListener(category, name);
    return scope.Close(Undefined());
}

/**
 * NodeJS Call to get joints
 * @param args - each parameter is one named joint
 * @return object structure of joints
 */
Handle<Value> getJoints(const Arguments& args) {
    HandleScope scope;

    Local<Object> skel_obj = Object::New(); 
    for ( int c = 0; c < args.Length(); c++)
    {   
        // Argument is not a string - call shenanigans on the whole operation
        if (!args[c]->IsString()) {
            ThrowException(Exception::TypeError(String::New("Argument needs to be a string")));
            return scope.Close(Undefined());
        }

        // get Joint struct mapping for string
        // if found map the NodeJS Object on to the overall data structure going out
        String::Utf8Value utfStr(args[c]->ToString());
        char* s = (char*) *utfStr;
        Local<Object> joint = mapJointToNodeObject(s);
        skel_obj->Set(String::NewSymbol(s), joint ); 
    }
    return scope.Close(skel_obj);
}

/**
 * send event to Node
 *
 * @param event type
 */
void sendEventToNode(int eventType) {
    Local<Object> event = Object::New();
    event->Set(String::NewSymbol("eventType"), String::New(EnumMapping::mapEventToLabel(eventType).c_str()));
    Local<Value> args[] = { event }; 
    Local<Value> callback_v = context_obj->Get(String::New("on"));
    if (callback_v->IsFunction()) {
        node::MakeCallback(context_obj, "on", 1, args);
    }
}

/**
 * send gesture originating from the frame reader thread to Node
 *
 * @param async handle
 * @param status (?)
 */
void sendEventFromThreadToNode(uv_async_t *handle, int status /*UNUSED*/) {
    Gesture g = *((Gesture*) handle->data);
    Local<Object> event = Object::New();

    if (g.type >= 1000 && g.type < 2000) {
        event->Set(String::NewSymbol("eventType"), String::New("GESTURE"));
        event->Set(String::NewSymbol("gestureType"), String::New(EnumMapping::mapEventToLabel(g.type).c_str()));
        event->Set(String::NewSymbol("hand"), String::New(EnumMapping::mapEventToLabel(g.hand).c_str()));
        event->Set(String::NewSymbol("step"), String::New(EnumMapping::mapEventToLabel(g.step).c_str()));
    } else {
        // bad I know - but we're handling gestures structs and event IDs here
        // no idea how g.type is an int here when we're passing in a non-struct int
        event->Set(String::NewSymbol("eventType"), String::New(EnumMapping::mapEventToLabel(g.type).c_str()));
    }
    Local<Value> args[] = { event }; 
    Local<Value> callback_v = context_obj->Get(String::New("on"));
    if (callback_v->IsFunction()) {
        node::MakeCallback(context_obj, "on", 1, args);
    }
}


/** 
 * shutdown/cleanup NITE/OpenNI
 * 
 * @param args (none used)
 */
Handle<Value> close(const Arguments& args) {
    HandleScope scope;
    fprintf(stderr, "Shutdown NITE\n");
    keepWorkerRunning = false;
    nite::NiTE::shutdown();
    return scope.Close(Undefined());
}

/**
 * nodeJS method to intialize and start OpenNI/NiTE
 *
 * @param args (none - don't pass them in here)
 */
Handle<Value> initialize(const Arguments& args) {
    HandleScope scope;

    nite::Status niteRc;

    fprintf(stderr, "Initialize Depth Camera\n");
    
    niteRc = nite::NiTE::initialize();
    if (niteRc != nite::STATUS_OK)
    {
        fprintf(stderr, "NiTE initialization failed\n");
        sendEventToNode(ERROR_NITE_INITIALIZATION);
        return scope.Close( Undefined() );
    }

    niteRc = userTracker.create();
    if (niteRc != nite::STATUS_OK)
    {
        fprintf(stderr, "Couldn't create user tracker\n");
        sendEventToNode(ERROR_USER_TRACKER_CREATION);
        return scope.Close(Undefined());
    }
    fprintf(stderr,"Start moving around to get detected...\n");

    keepWorkerRunning = true;
    skeleton.leftHand.isActive = false;
    skeleton.leftHand.type = nite::JOINT_LEFT_HAND;
    skeleton.leftElbow.isActive = false;
    skeleton.leftElbow.type = nite::JOINT_LEFT_ELBOW;
    skeleton.leftShoulder.isActive = false;
    skeleton.leftShoulder.type = nite::JOINT_LEFT_SHOULDER;

    skeleton.rightHand.isActive = false;
    skeleton.rightHand.type = nite::JOINT_RIGHT_HAND;
    skeleton.rightElbow.isActive = false;
    skeleton.rightElbow.type = nite::JOINT_RIGHT_ELBOW;
    skeleton.rightShoulder.type = nite::JOINT_RIGHT_SHOULDER;

    skeleton.torso.isActive = false;
    skeleton.torso.type = nite::JOINT_TORSO;

    skeleton.head.isActive = false;
    skeleton.head.type = nite::JOINT_HEAD;

    skeleton.leftHip.isActive = false;
    skeleton.leftHip.type = nite::JOINT_LEFT_HIP;

    skeleton.rightHip.isActive = false;
    skeleton.rightHip.type = nite::JOINT_RIGHT_HIP;

    sendEventToNode(DEVICE_INITIALIZED);

    uv_work_t *req = new uv_work_t;
    uv_async_init(uv_default_loop(), &async, sendEventFromThreadToNode);
    uv_queue_work(uv_default_loop(), req, frameWorker, (uv_after_work_cb)onFrameWorkerThreadComplete);
    uv_ref((uv_handle_t *)&async);

    return scope.Close(Undefined());
}

/**
 * thread worker thread for OpenNI/NITE to read frames and do work on them
 *
 * @param thread request
 */
void onFrameWorkerThreadComplete(uv_work_t *req) {
    fprintf(stderr, "OpenNI/NITE Processing Complete\n");
    uv_close((uv_handle_t*) &async, NULL);
} 

/**
 * process frames in separate thread
 *
 * @param request thread 
 */
void frameWorker(uv_work_t *req) {
    while (keepWorkerRunning) {
        nite::Status niteRc;
        nite::UserTrackerFrameRef userTrackerFrame;
        niteRc = userTracker.readFrame(&userTrackerFrame);
        if (niteRc != nite::STATUS_OK)
        {
            fprintf(stderr,"Get next frame failed\n");
            continue;
        }

        const nite::Array<nite::UserData>& users = userTrackerFrame.getUsers();
        for (int i = 0; i < users.getSize(); ++i)
        {
            const nite::UserData& user = users[i];

            updateUserState(user);
            if (user.isNew())
            {
                userTracker.startSkeletonTracking(user.getId());
                skeleton.leftHand.isActive = false;
                skeleton.leftElbow.isActive = false;
                skeleton.leftShoulder.isActive = false;
                skeleton.rightHand.isActive = false;
                skeleton.rightElbow.isActive = false;
                skeleton.rightShoulder.isActive = false;
                skeleton.head.isActive = false;
                skeleton.torso.isActive = false;
                skeleton.rightHip.isActive = false;
                skeleton.leftHip.isActive = false;
            } 
            else if (user.getSkeleton().getState() == nite::SKELETON_TRACKED)
            {

                const nite::Skeleton &niteskeleton = user.getSkeleton();
                mapSkeleton(skeleton, niteskeleton);

                std::vector<Gesture> gestures;
                gst.updateSkeleton(gestures, skeleton);

                for (unsigned int c = 0; c < gestures.size(); c++) {
                    sendingGesture = gestures[c];
                    async.data = (void*) &sendingGesture;
                    uv_async_send(&async);
                }
            }
        }
    }
}

Local<Object> mapJointToNodeObject(char *jointName) {
    Local<Object> jsJoint = Object::New();

    int left_percentExtended = -1;
    int right_percentExtended = -1;
    Joint j;

    if (strcmp( jointName, "torso") == 0) {
        j = skeleton.torso; 
    } else if (strcmp( jointName, "head") == 0) {
        j = skeleton.head; 
    } else if (strcmp( jointName, "left_hand") == 0) {
        j = skeleton.leftHand; 
    } else if (strcmp( jointName, "left_elbow") == 0) {
        j = skeleton.leftElbow; 
    } else if (strcmp( jointName, "left_shoulder") == 0) {
        j = skeleton.leftElbow; 
    } else if (strcmp( jointName, "left_hip") == 0) {
        j = skeleton.leftHip; 
    } else if (strcmp( jointName, "right_hand") == 0) {
        j = skeleton.rightHand; 
    } else if (strcmp( jointName, "right_elbow") == 0) {
        j = skeleton.rightElbow; 
    } else if (strcmp( jointName, "right_shoulder") == 0) {
        j = skeleton.rightElbow; 
    } else if (strcmp( jointName, "right_hip") == 0) {
        j = skeleton.rightHip; 
    } else {
        j = Joint();
    }

    jsJoint->Set(String::NewSymbol("x"), Number::New( j.xPos ));
    jsJoint->Set(String::NewSymbol("y"), Number::New( j.yPos ));
    jsJoint->Set(String::NewSymbol("z"), Number::New( j.zPos ));
    jsJoint->Set(String::NewSymbol("xRotation"), Number::New( j.xRotation ));
    jsJoint->Set(String::NewSymbol("yRotation"), Number::New( j.yRotation ));
    jsJoint->Set(String::NewSymbol("zRotation"), Number::New( j.zRotation ));
    jsJoint->Set(String::NewSymbol("positionConfidence"), Number::New( j.positionConfidence ));

    if (j.type == nite::JOINT_LEFT_HAND || j.type == nite::JOINT_RIGHT_HAND) {
       jsJoint->Set(String::NewSymbol("percentExtended"), Number::New(j.percentExtended)); 
    }

    jsJoint->Set(String::NewSymbol("active"), Number::New( j.isActive ));

    return jsJoint;
}

/**
 * map skeleton to internal skeleyon stet
 *
 * Skeleton skeleton
 * nite::Skeleton niteskeleton
 */
void mapSkeleton(Skeleton &skeleton, nite::Skeleton niteskeleton) {
    mapJointFromSkeleton(skeleton.leftHand, niteskeleton);
    mapJointFromSkeleton(skeleton.leftElbow, niteskeleton);
    mapJointFromSkeleton(skeleton.leftShoulder, niteskeleton);
    mapJointFromSkeleton(skeleton.rightHand, niteskeleton);
    mapJointFromSkeleton(skeleton.rightElbow, niteskeleton);
    mapJointFromSkeleton(skeleton.rightShoulder, niteskeleton);
    mapJointFromSkeleton(skeleton.torso, niteskeleton);
    mapJointFromSkeleton(skeleton.head, niteskeleton);
    mapJointFromSkeleton(skeleton.leftHip, niteskeleton);
    mapJointFromSkeleton(skeleton.rightHip, niteskeleton);
}

/**
 * map joint properties from same joint on skeleton
 *
 * Joint j
 * Skeleton s
 */
void mapJointFromSkeleton(Joint &j, nite::Skeleton s) {
    j.xPos = (int) s.getJoint( (nite::JointType) j.type).getPosition().x;
    j.yPos = (int) s.getJoint( (nite::JointType) j.type).getPosition().y;
    j.zPos = (int) s.getJoint( (nite::JointType) j.type).getPosition().z;

    const nite::Quaternion &o = s.getJoint( (nite::JointType) j.type).getOrientation();

    j.xRotation = atan2(2*o.y*o.w-2*o.x*o.z , 1 - 2*pow(o.y,2) - 2*pow(o.z, 2)) * (180/PI);
    j.yRotation = asin(2*o.x*o.y + 2*o.z*o.w) * (180/PI);
    j.zRotation = atan2(2*o.x*o.w-2*o.y*o.z , 1 - 2*pow(o.x, 2) - 2*pow(o.z, 2)) * (180/PI);

    if (s.getJoint( (nite::JointType) j.type).getPositionConfidence() > .5) {
        j.isActive = true;
    } else {
        j.isActive = false;
    }
    j.positionConfidence = s.getJoint( (nite::JointType) j.type).getPositionConfidence();

    // calculate how far arm is extended for hands
    if ((nite::JointType) j.type == nite::JOINT_LEFT_HAND ) {
        float dist_lh2le = sqrt( pow(skeleton.leftHand.xPos - skeleton.leftElbow.xPos, 2) + pow(skeleton.leftHand.yPos- skeleton.leftElbow.yPos, 2) + pow(skeleton.leftHand.zPos - skeleton.leftElbow.zPos, 2) );
        float dist_lh2ls = sqrt( pow(skeleton.leftHand.xPos - skeleton.leftShoulder.xPos, 2) + pow(skeleton.leftHand.yPos - skeleton.leftShoulder.yPos, 2) + pow(skeleton.leftHand.zPos - skeleton.leftShoulder.zPos, 2) );
        float dist_le2ls = sqrt( pow(skeleton.leftElbow.xPos - skeleton.leftShoulder.xPos, 2) + pow(skeleton.leftElbow.yPos - skeleton.leftShoulder.yPos, 2) + pow(skeleton.leftElbow.zPos - skeleton.leftShoulder.zPos, 2) );
        float left_percentExtended = (int)( dist_lh2ls/(dist_le2ls + dist_lh2le) *100);
        j.percentExtended = left_percentExtended;
    }

    if ((nite::JointType) j.type == nite::JOINT_RIGHT_HAND ) {
        float dist_rh2re = sqrt( pow(skeleton.rightHand.xPos - skeleton.rightElbow.xPos, 2) + pow(skeleton.rightHand.yPos- skeleton.rightElbow.yPos, 2) + pow(skeleton.rightHand.zPos - skeleton.rightElbow.zPos, 2) );
        float dist_rh2rs = sqrt( pow(skeleton.rightHand.xPos - skeleton.rightShoulder.xPos, 2) + pow(skeleton.rightHand.yPos - skeleton.rightShoulder.yPos, 2) + pow(skeleton.rightHand.zPos - skeleton.rightShoulder.zPos, 2) );
        float dist_re2rs = sqrt( pow(skeleton.rightElbow.xPos - skeleton.rightShoulder.xPos, 2) + pow(skeleton.rightElbow.yPos - skeleton.rightShoulder.yPos, 2) + pow(skeleton.rightElbow.zPos - skeleton.rightShoulder.zPos, 2) );
        float right_percentExtended = (int) dist_rh2rs/(dist_re2rs + dist_rh2re) *100;
        j.percentExtended = right_percentExtended;
    }
}

/**
 * update and event out on state of user and skeleton tracking
 */
void updateUserState(const nite::UserData& user)
{
    if (user.isNew()) {
        eventIDToSend = NEW_USER;
        async.data = (void*) &eventIDToSend;
        uv_async_send(&async);
    } else if (!isUserVisible && user.isVisible() && user.getId() == 1) {
        isUserVisible = true;
        eventIDToSend = USER_IS_VISIBLE;
        async.data = (void*) &eventIDToSend;
        uv_async_send(&async);
    } else if (isUserVisible && !user.isVisible() && user.getId() == 1) {
        isUserVisible = false;
        eventIDToSend = USER_IS_OUT_OF_SCENE;
        async.data = (void*) &eventIDToSend;
        uv_async_send(&async);
    } else if (user.isLost()) {
        // why doesn't this fire?
        // maybe two users are needed?
    }

    g_visibleUsers[user.getId()] = user.isVisible();

    if(g_skeletonStates[user.getId()] != user.getSkeleton().getState())
    {
        switch(g_skeletonStates[user.getId()] = user.getSkeleton().getState())
        {
            case nite::SKELETON_NONE:
                if (isSkeletonTracking) {
                    isSkeletonTracking = false;
                    isSkeletonCalibrating = false;
                    eventIDToSend = SKELETON_STOPPED_TRACKING;
                    async.data = (void*) &eventIDToSend;
                    uv_async_send(&async);
                }
                break;
            case nite::SKELETON_CALIBRATING:
                if (!isSkeletonCalibrating) {
                    isSkeletonCalibrating = true;
                    eventIDToSend = SKELETON_CALIBRATING;
                    async.data = (void*) &eventIDToSend;
                    uv_async_send(&async);
                }
                break;
            case nite::SKELETON_TRACKED:
                if (!isSkeletonTracking) {
                    isSkeletonTracking = true;
                    isSkeletonCalibrating = false;
                    eventIDToSend = SKELETON_TRACKING;
                    async.data = (void*) &eventIDToSend;
                    uv_async_send(&async);
                }
                break;
            case nite::SKELETON_CALIBRATION_ERROR_NOT_IN_POSE:
            case nite::SKELETON_CALIBRATION_ERROR_HANDS:
            case nite::SKELETON_CALIBRATION_ERROR_LEGS:
            case nite::SKELETON_CALIBRATION_ERROR_HEAD:
            case nite::SKELETON_CALIBRATION_ERROR_TORSO:
                eventIDToSend = SKELETON_CALIBRATION_FAILED;
                async.data = (void*) &eventIDToSend;
                uv_async_send(&async);
                isSkeletonTracking = false;
                isSkeletonCalibrating = false;
                break;
        }
    }
}

/* Module Declaration */
NODE_MODULE(nuimotion, init)
