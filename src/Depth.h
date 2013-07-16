#include <node.h>
#include <v8.h>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <OpenNI.h>
#include "../Common/OniSampleUtilities.h"

using namespace v8;
using namespace openni;

/* thread loop */
uv_loop_t *loop;

/* async thread ref */
uv_async_t async;

/* context for sending NodeJS callbacks */
Persistent<Object> context_obj;

/** boolean that indicates if worker thread should keep running */
bool keepWorkerRunning;

/* depth tracker */
VideoStream depth;

/* rgb tracker */
VideoStream rgb;

/* the device */
Device device;

/* the depth frame */
VideoFrameRef dFrame;

/* the rgb frame */
VideoFrameRef rgbFrame;

/* frame height */
int dFrameHeight;

/* frame width */
int dFrameWidth;

/* frame height */
int rgbFrameHeight;

/* frame width */
int rgbFrameWidth;


void frameWorker(uv_work_t *req);
void onFrameWorkerThreadComplete(uv_work_t* req);

/* NodeJS Methods */
Handle<Value> initialize(const Arguments& args);
Handle<Value> close(const Arguments& args);
Handle<Value> getDepthFrame(const Arguments& args);
Handle<Value> getRGBFrame(const Arguments& args);