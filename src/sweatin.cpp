#include <node.h>
#include <v8.h>
#include <stdio.h>
#include <stdlib.h>
#include <string>

#include "NiTE.h"
#include "sweatin.h"

#include "../Common/NiteSampleUtilities.h"

using namespace v8;

/**
 * entry point for plugin
 *
 * @param plugin target
 */
void init(Handle<Object> target) {
    target->Set(String::NewSymbol("init"),
        FunctionTemplate::New(initialize)->GetFunction());
    target->Set(String::NewSymbol("close"),
        FunctionTemplate::New(close)->GetFunction());
    context_obj = Persistent<Object>::New(Object::New()); 
    target->Set(String::New("context"), context_obj); 
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
    handTracker.startGestureDetection(nite::GESTURE_CLICK);
   // handTracker.startGestureDetection(nite::GESTURE_HAND_RAISE);
    fprintf(stderr, "Ready for Gestures\n");

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
        case nite::GESTURE_CLICK:
            gestureString = String::New("Gesture_Click");
            break;
        case nite::GESTURE_WAVE:
            gestureString = String::New("Gesture_Wave");
            break;
        case nite::GESTURE_HAND_RAISE:
            gestureString = String::New("Gesture_RaiseHand"); 
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
                nite::HandId newId;
                gst = gestures[i].getType();
                async.data = (void*) &gst;
                uv_async_send(&async);
            }
        }
    }
}


/* Module Declaration */
NODE_MODULE(sweatin, init)
