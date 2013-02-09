/** 
 * Activity using OpenNI/NiTE to track ONLY one hand
 * This hand must issue a wave gesture, and only then will it begin tracking
 * If hand is lost, user must wave again to regain tracking ability
 */

#include <node.h>
#include <v8.h>
#include <stdio.h>
#include <stdlib.h>
#include <string>

#include "NiTE.h"
#include "sweatin.h"

#include "../Common/NiteSampleUtilities.h"

using namespace v8;

/** if we are in hand cursor mode */
bool cursorMode = false;

int handPosX = -9999;
int handPosY = -9999;
int handPosZ = -9999;

static const int GESTURE_LOSTHAND = 101;

Handle<Value> getHand(const Arguments& args);

/**
 * entry point for plugin
 *
 * @param plugin target
 */
void init(Handle<Object> target) {
    fprintf(stderr, "Follow the Hand! \n");
    target->Set(String::NewSymbol("init"),
        FunctionTemplate::New(initialize)->GetFunction());
    target->Set(String::NewSymbol("close"),
        FunctionTemplate::New(close)->GetFunction());
    target->Set(String::NewSymbol("getHand"),
        FunctionTemplate::New(getHand)->GetFunction());

    context_obj = Persistent<Object>::New(Object::New()); 
    target->Set(String::New("context"), context_obj); 
}


Handle<Value> getHand(const Arguments& args) {
    HandleScope scope;
    Local<Object> obj = Object::New();
    obj->Set(String::NewSymbol("x"), Number::New( handPosX ));
    obj->Set(String::NewSymbol("y"), Number::New( handPosY ));
    obj->Set(String::NewSymbol("z"), Number::New( handPosZ ));
    return scope.Close(obj);
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

    keepWorkerRunning = false;
    fprintf(stderr, "Initialize Depth Camera\n");
    
    nite::Status niteRc;

    niteRc = nite::NiTE::initialize();
    if (niteRc != nite::STATUS_OK)
    {
        fprintf(stderr, "NiTE initialization failed\n");
        return scope.Close( Undefined() );
    }

    niteRc = handTracker.create();
    if (niteRc != nite::STATUS_OK)
    {
        fprintf(stderr, "Couldn't create user tracker\n");
        return scope.Close( Undefined() );
    }

    handTracker.startGestureDetection(nite::GESTURE_WAVE);
    fprintf(stderr, "Wave your hand to get started...\n");

    keepWorkerRunning = true;
    loop = uv_default_loop();

    uv_work_t req;
    uv_async_init(loop, &async, onMotionEvent);
    uv_queue_work(loop, &req, frameWorker, onFrameWorkerThreadComplete);
    uv_run(loop);

    return scope.Close(Undefined());
}


/**
 * on motion event found in frame processing thread
 *
 * @param async handle
 * @param status (?)
 */
void onMotionEvent(uv_async_t *handle, int status /*UNUSED*/) {
    int gesture = *((int*) handle->data);

    Local <String> gestureString;
    switch (gesture) {
        case nite::GESTURE_WAVE:
            gestureString = String::New("Gesture_Wave");
            break;
        case GESTURE_LOSTHAND:
            gestureString = String::New("Gesture_LostHand");
            break; 

    }
    Local<Value> args[] = { gestureString }; 
    node::MakeCallback(context_obj, "on", 1, args); 
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
    int gst;
    while (keepWorkerRunning) {
        nite::HandTrackerFrameRef handTrackerFrame;

        nite::Status niteRc;
        niteRc = handTracker.readFrame(&handTrackerFrame);
        if (niteRc != nite::STATUS_OK)
        {
            fprintf(stderr, "Get next frame failed\n");
        }

        const nite::Array<nite::GestureData>& gestures = handTrackerFrame.getGestures();
        for (int i = 0; i < gestures.getSize(); ++i)
        {
            if (gestures[i].isComplete())
            {
                if (gestures[i].getType() == nite::GESTURE_WAVE) {
                    nite::HandId newId;
                    handTracker.startHandTracking(gestures[i].getCurrentPosition(), &newId);
                }
                gst = gestures[i].getType();
                async.data = (void*) &gst;
                uv_async_send(&async);

            }
        }
        const nite::Array<nite::HandData>& hands = handTrackerFrame.getHands();
        for (int i = 0; i < hands.getSize(); ++i)
        {
            const nite::HandData& hand = hands[i];
            if (hand.isTracking())
            {
                cursorMode = true;
                handPosX = hand.getPosition().x;
                handPosY = hand.getPosition().y;
                handPosZ = hand.getPosition().z;
            }
        }

        if (hands.getSize() == 0) {
            handPosX = -9999;
            handPosY = -9999;
            handPosZ = -9999; 

            if (cursorMode == true) {
                cursorMode = false;
                gst = GESTURE_LOSTHAND;
                async.data = (void*) &gst;
                uv_async_send(&async);
            }
            cursorMode = false;
        }
    }
}


/* Module Declaration */
NODE_MODULE(followthehand, init)
