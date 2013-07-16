/** 
 * Activity using OpenNI to do depth tracking
 */

#include <node.h>
#include <v8.h>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <math.h>

#include "Depth.h"

using namespace v8;
using namespace openni;

/**
 * entry point for plugin
 *
 * @param plugin target
 */
void init(Handle<Object> target) {
    fprintf(stderr, "Nui Motion Depth Initialize \n");
    target->Set(String::NewSymbol("init"),
        FunctionTemplate::New(initialize)->GetFunction());
    target->Set(String::NewSymbol("close"),
        FunctionTemplate::New(close)->GetFunction());
    target->Set(String::NewSymbol("getDepthFrame"),
        FunctionTemplate::New(getDepthFrame)->GetFunction());
    target->Set(String::NewSymbol("getRGBFrame"),
        FunctionTemplate::New(getRGBFrame)->GetFunction());
    context_obj = Persistent<Object>::New(Object::New()); 
    target->Set(String::New("context"), context_obj);
}

/**
 * NodeJS Call to get depth frame
 * @param args
 * @return video frame
 */
Handle<Value> getDepthFrame(const Arguments& args) {
    HandleScope scope;

    Local<Object> obj = Object::New();
    obj->Set(String::NewSymbol("type"), String::NewSymbol("depth"));
    obj->Set(String::NewSymbol("width"), Number::New( dFrameWidth ));
    obj->Set(String::NewSymbol("height"), Number::New( dFrameHeight ));

    // Don't know how to convert this or pass to Node
    //obj->Set(String::NewSymbol("data"), String::NewSymbol(dFrame.getData()));

    return scope.Close(obj);
}

/**
 * NodeJS Call to get rgb frame
 * @param args
 * @return video frame
 */
Handle<Value> getRGBFrame(const Arguments& args) {
    HandleScope scope;

    Local<Object> obj = Object::New();
    obj->Set(String::NewSymbol("type"), String::NewSymbol("rgb"));
    obj->Set(String::NewSymbol("width"), Number::New( rgbFrameWidth ));
    obj->Set(String::NewSymbol("height"), Number::New( rgbFrameHeight ));

    // Don't know how to convert this or pass to Node
    //obj->Set(String::NewSymbol("data"), String::NewSymbol(rgbFrame.getData()));

    return scope.Close(obj);
}


/** 
 * shutdown/cleanup OpenNI
 * 
 * @param args (none used)
 */
Handle<Value> close(const Arguments& args) {
    HandleScope scope;
    fprintf(stderr, "Shutdown OpenNI\n");
    keepWorkerRunning = false;
    depth.stop();
    depth.destroy();
    device.close();
    OpenNI::shutdown();
    return scope.Close(Undefined());
}

/**
 * nodeJS method to intialize and start OpenNI/NiTE
 *
 * @param args (none - don't pass them in here)
 */
Handle<Value> initialize(const Arguments& args) {
    HandleScope scope;

    Status rc = OpenNI::initialize();
    if (rc != STATUS_OK)
    {
        fprintf(stderr, "Initialize failed\n%s\n", OpenNI::getExtendedError());
        return scope.Close(Undefined());
    }

    rc = device.open(ANY_DEVICE);
    if (rc != STATUS_OK)
    {
        fprintf(stderr, "Couldn't open device\n%s\n", OpenNI::getExtendedError());
        return scope.Close(Undefined());
    }

    if (device.getSensorInfo(SENSOR_DEPTH) != NULL)
    {
        rc = depth.create(device, SENSOR_DEPTH);
        if (rc != STATUS_OK)
        {
            fprintf(stderr, "Couldn't create depth stream\n%s\n", OpenNI::getExtendedError());
            return scope.Close(Undefined());
        }
    }

    if (device.getSensorInfo(SENSOR_COLOR) != NULL)
    {
        rc = rgb.create(device, SENSOR_COLOR);
        if (rc != STATUS_OK)
        {
            fprintf(stderr, "Couldn't create rgb stream\n%s\n", OpenNI::getExtendedError());
            return scope.Close(Undefined());
        }
    }

    rc = depth.start();
    if (rc != STATUS_OK)
    {
        fprintf(stderr, "Couldn't start the depth stream\n%s\n", OpenNI::getExtendedError());
        return scope.Close(Undefined());
    }

    rc = rgb.start();
    if (rc != STATUS_OK)
    {
        fprintf(stderr, "Couldn't start the color stream\n%s\n", OpenNI::getExtendedError());
        return scope.Close(Undefined());
    }

    keepWorkerRunning = true;

    uv_work_t *req = new uv_work_t;
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
    fprintf(stderr, "OpenNI Processing Complete\n");
    uv_close((uv_handle_t*) &async, NULL);
} 

/**
 * process frames in separate thread
 *
 * @param request thread 
 */
void frameWorker(uv_work_t *req) {
    while (keepWorkerRunning) {
        Status rc = depth.readFrame(&dFrame);
        if (rc != STATUS_OK)
        {
            fprintf(stderr, "Wait failed\n");
            continue;
        }

        if (dFrame.getVideoMode().getPixelFormat() != PIXEL_FORMAT_DEPTH_1_MM && dFrame.getVideoMode().getPixelFormat() != PIXEL_FORMAT_DEPTH_100_UM)
        {
            fprintf(stderr, "Unexpected frame format\n");
            continue;
        }

        rc = rgb.readFrame(&rgbFrame);
        if (rc != STATUS_OK)
        {
            fprintf(stderr, "Wait failed\n");
            continue;
        }

        dFrameHeight = dFrame.getHeight();
        dFrameWidth = dFrame.getWidth();
        rgbFrameHeight = rgbFrame.getHeight();
        rgbFrameWidth = rgbFrame.getWidth();

        // don't know what to do here - would probably like to throw dFrame and rgbFrame data into
        // some kind of data buffer that can be sent over to Node.js
        //DepthPixel* pDepth = (DepthPixel*)dFrame.getData();
    }
}

/* Module Declaration */
NODE_MODULE(nuimotion_depth, init)
