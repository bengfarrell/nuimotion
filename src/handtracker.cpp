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
#include "handtracker.h"

#include "../Common/NiteSampleUtilities.h"

using namespace v8;

static const int NEW_USER = 1;
static const int USER_IS_VISIBLE = 2;
static const int USER_IS_OUT_OF_SCENE = 3;
static const int USER_IS_LOST = 4;
static const int SKELETON_STOPPED_TRACKING = 5;
static const int SKELETON_CALIBRATING = 6;
static const int SKELETON_TRACKING = 7;
static const int SKELETON_CALIBRATION_FAILED = 8;
static const int DEVICE_INITIALIZED = 100;
static const int DEVICE_ERROR = 101;

#define MAX_USERS 1

bool g_visibleUsers[MAX_USERS] = {false};
nite::SkeletonState g_skeletonStates[MAX_USERS] = {nite::SKELETON_NONE};

/**
 * entry point for plugin
 *
 * @param plugin target
 */
void init(Handle<Object> target) {
    fprintf(stderr, "Hand Tracker \n");
    target->Set(String::NewSymbol("init"),
        FunctionTemplate::New(initialize)->GetFunction());
    target->Set(String::NewSymbol("close"),
        FunctionTemplate::New(close)->GetFunction());
    target->Set(String::NewSymbol("getHands"),
        FunctionTemplate::New(getHands)->GetFunction());

    context_obj = Persistent<Object>::New(Object::New()); 
    target->Set(String::New("context"), context_obj); 
}


Handle<Value> getHands(const Arguments& args) {
    float dist_lh2le = sqrt( pow(joint_leftHand.xPos - joint_leftElbow.xPos, 2) + pow(joint_leftHand.yPos- joint_leftElbow.yPos, 2) + pow(joint_leftHand.zPos - joint_leftElbow.zPos, 2) );
    float dist_lh2ls = sqrt( pow(joint_leftHand.xPos - joint_leftShoulder.xPos, 2) + pow(joint_leftHand.yPos - joint_leftShoulder.yPos, 2) + pow(joint_leftHand.zPos - joint_leftShoulder.zPos, 2) );
    float dist_le2ls = sqrt( pow(joint_leftElbow.xPos - joint_leftShoulder.xPos, 2) + pow(joint_leftElbow.yPos - joint_leftShoulder.yPos, 2) + pow(joint_leftElbow.zPos - joint_leftShoulder.zPos, 2) );

    float dist_rh2re = sqrt( pow(joint_rightHand.xPos - joint_rightElbow.xPos, 2) + pow(joint_rightHand.yPos- joint_rightElbow.yPos, 2) + pow(joint_rightHand.zPos - joint_rightElbow.zPos, 2) );
    float dist_rh2rs = sqrt( pow(joint_rightHand.xPos - joint_rightShoulder.xPos, 2) + pow(joint_rightHand.yPos - joint_rightShoulder.yPos, 2) + pow(joint_rightHand.zPos - joint_rightShoulder.zPos, 2) );
    float dist_re2rs = sqrt( pow(joint_rightElbow.xPos - joint_rightShoulder.xPos, 2) + pow(joint_rightElbow.yPos - joint_rightShoulder.yPos, 2) + pow(joint_rightElbow.zPos - joint_rightShoulder.zPos, 2) );

    int left_percentExtended = (int) dist_lh2ls/(dist_le2ls + dist_lh2le) *100;
    int right_percentExtended = (int) dist_rh2rs/(dist_re2rs + dist_rh2re) *100;

    HandleScope scope;
    Local<Object> lHand = Object::New(); 
    lHand->Set(String::NewSymbol("x"), Number::New( joint_leftHand.xPos ));
    lHand->Set(String::NewSymbol("y"), Number::New( joint_leftHand.yPos ));
    lHand->Set(String::NewSymbol("z"), Number::New( joint_leftHand.zPos ));
    lHand->Set(String::NewSymbol("percentExtended"), Number::New(left_percentExtended));
    lHand->Set(String::NewSymbol("active"), Number::New( joint_leftHand.isActive ));

    Local<Object> rHand = Object::New(); 
    rHand->Set(String::NewSymbol("x"), Number::New( joint_rightHand.xPos ));
    rHand->Set(String::NewSymbol("y"), Number::New( joint_rightHand.yPos ));
    rHand->Set(String::NewSymbol("z"), Number::New( joint_rightHand.zPos ));
    rHand->Set(String::NewSymbol("percentExtended"), Number::New(right_percentExtended));
    rHand->Set(String::NewSymbol("active"), Number::New( joint_rightHand.isActive ));

    Local<Object> body = Object::New(); 
    body->Set(String::NewSymbol("x"), Number::New( joint_bodyCenter.xPos ));
    body->Set(String::NewSymbol("y"), Number::New( joint_bodyCenter.yPos ));
    body->Set(String::NewSymbol("z"), Number::New( joint_bodyCenter.zPos ));
    body->Set(String::NewSymbol("active"), Number::New( joint_bodyCenter.isActive ));

    Local<Object> obj = Object::New(); 
    obj->Set(String::NewSymbol("right_hand"), rHand);  
    obj->Set(String::NewSymbol("left_hand"), lHand);    
    obj->Set(String::NewSymbol("body_center"), body);    
    return scope.Close(obj);
}


/**
 * on device event
 *
 * @param event type
 */
void onDeviceEvent(int eventType) {
    Local <String> eventString;
    switch (eventType) {
        case DEVICE_INITIALIZED:
            eventString = String::New("DEVICE_INITIALIZED");
            break;
        case DEVICE_ERROR:
            eventString = String::New("DEVICE_ERROR");
            break;
        default:
            eventString = String::New("No known event type found");
            break;
    }
    Local<Value> args[] = { eventString }; 
    node::MakeCallback(context_obj, "on", 1, args);
}

/**
 * on tracking event found in frame processing thread
 * we message out to Node in here
 *
 * @param async handle
 * @param status (?)
 */
void onTrackingEvent(uv_async_t *handle, int status /*UNUSED*/) {
    int event = *((int*) handle->data);
    Local <String> eventString;
    switch (event) {
        case NEW_USER:
            eventString = String::New("NEW_USER");
            lastUserEventDispatched = currentUserState;
            break;
        case USER_IS_VISIBLE:
            eventString = String::New("USER_IS_VISIBLE");
            lastUserEventDispatched = currentUserState;
            break; 
        case USER_IS_OUT_OF_SCENE:
            eventString = String::New("USER_IS_OUT_OF_SCENE");
            lastUserEventDispatched = currentUserState;
            break; 
        case USER_IS_LOST:
            eventString = String::New("USER_IS_LOST");
            lastUserEventDispatched = currentUserState;
            break; 
        case SKELETON_STOPPED_TRACKING:
            eventString = String::New("SKELETON_STOPPED_TRACKING");
            lastSkeletalEventDispatched = currentSkeletonState;
            break; 
        case SKELETON_TRACKING:
            eventString = String::New("SKELETON_TRACKING");
            lastSkeletalEventDispatched = currentSkeletonState;
            break; 
        case SKELETON_CALIBRATING:
            eventString = String::New("SKELETON_CALIBRATING");
            lastSkeletalEventDispatched = currentSkeletonState;
            break; 
        case SKELETON_CALIBRATION_FAILED:
            eventString = String::New("SKELETON_CALIBRATION_FAILED");
            lastSkeletalEventDispatched = currentSkeletonState;
            break; 
        default:
            eventString = String::New("No known event type found");
            break; 
    }

    Local<Value> args[] = { eventString }; 
    node::MakeCallback(context_obj, "on", 1, args);
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
        onDeviceEvent(DEVICE_ERROR);
        return scope.Close( Undefined() );
    }

    niteRc = userTracker.create();
    if (niteRc != nite::STATUS_OK)
    {
        fprintf(stderr, "Couldn't create user tracker\n");
        onDeviceEvent(DEVICE_ERROR);
        return scope.Close(Undefined());
    }
    fprintf(stderr,"\nStart moving around to get detected...\n(PSI pose may be required for skeleton calibration, depending on the configuration)\n");

    keepWorkerRunning = true;
    joint_leftHand.isActive = false;
    joint_leftHand.type = nite::JOINT_LEFT_HAND;
    joint_leftElbow.isActive = false;
    joint_leftElbow.type = nite::JOINT_LEFT_ELBOW;
    joint_leftShoulder.isActive = false;
    joint_leftShoulder.type = nite::JOINT_LEFT_SHOULDER;

    joint_rightHand.isActive = false;
    joint_rightHand.type = nite::JOINT_RIGHT_HAND;
    joint_rightElbow.isActive = false;
    joint_rightElbow.type = nite::JOINT_RIGHT_ELBOW;
    joint_rightShoulder.type = nite::JOINT_RIGHT_SHOULDER;

    joint_bodyCenter.isActive = false;
    joint_bodyCenter.type = nite::JOINT_TORSO;

    onDeviceEvent(DEVICE_INITIALIZED);
    
    loop = uv_default_loop();

    uv_work_t req;
    uv_async_init(loop, &async, onTrackingEvent);
    uv_queue_work(loop, &req, frameWorker, onFrameWorkerThreadComplete);
    uv_run(loop);

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
    int userEventType = 0;
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

            updateUserState(user,userTrackerFrame.getTimestamp());
            if (user.isNew())
            {
                userTracker.startSkeletonTracking(user.getId());
                joint_leftHand.isActive = false;
                joint_leftElbow.isActive = false;
                joint_leftShoulder.isActive = false;
                joint_rightHand.isActive = false;
                joint_rightElbow.isActive = false;
                joint_rightShoulder.isActive = false;
                joint_bodyCenter.isActive = false;
            } 
            else if (user.getSkeleton().getState() == nite::SKELETON_TRACKED)
            {
                const nite::Skeleton &skeleton = user.getSkeleton();
                mapJointFromSkeleton(joint_leftHand, skeleton);
                mapJointFromSkeleton(joint_leftElbow, skeleton);
                mapJointFromSkeleton(joint_leftShoulder, skeleton);
                mapJointFromSkeleton(joint_rightHand, skeleton);
                mapJointFromSkeleton(joint_rightElbow, skeleton);
                mapJointFromSkeleton(joint_rightShoulder, skeleton);
                mapJointFromSkeleton(joint_bodyCenter, skeleton);
            }
        }
    }
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

    if (s.getJoint( (nite::JointType) j.type).getPositionConfidence() > .5) {
        j.isActive = true;
    } else {
        j.isActive = false;
    }
}

/**
 * update and event out on state of user and skeleton tracking
 */
void updateUserState(const nite::UserData& user, unsigned long long ts)
{
    if (user.isNew()) {
        currentUserState = NEW_USER;
    } else if (user.isVisible() && !g_visibleUsers[user.getId()]) {
        currentUserState = USER_IS_VISIBLE;
    } else if (!user.isVisible() && g_visibleUsers[user.getId()]) {
        currentUserState = USER_IS_OUT_OF_SCENE;
    } else if (user.isLost()) {
        currentUserState = USER_IS_LOST;
    }

    if (currentUserState != 0 && lastUserEventDispatched != currentUserState) {
        async.data = (void*) &currentUserState;
        uv_async_send(&async);
    }

    g_visibleUsers[user.getId()] = user.isVisible();

    if(g_skeletonStates[user.getId()] != user.getSkeleton().getState())
    {
        switch(g_skeletonStates[user.getId()] = user.getSkeleton().getState())
        {
        case nite::SKELETON_NONE:
            currentSkeletonState = SKELETON_STOPPED_TRACKING;
            break;
        case nite::SKELETON_CALIBRATING:
            currentSkeletonState = SKELETON_CALIBRATING;
            break;
        case nite::SKELETON_TRACKED:
            currentSkeletonState = SKELETON_TRACKING;
            break;
        case nite::SKELETON_CALIBRATION_ERROR_NOT_IN_POSE:
        case nite::SKELETON_CALIBRATION_ERROR_HANDS:
        case nite::SKELETON_CALIBRATION_ERROR_LEGS:
        case nite::SKELETON_CALIBRATION_ERROR_HEAD:
        case nite::SKELETON_CALIBRATION_ERROR_TORSO:
            currentSkeletonState = SKELETON_CALIBRATION_FAILED;
            break;
        }
    }

    if (currentSkeletonState!= 0 && lastSkeletalEventDispatched != currentSkeletonState) {
        async.data = (void*) &currentSkeletonState;
        uv_async_send(&async);
    }
}



/* Module Declaration */
NODE_MODULE(handtracker, init)
