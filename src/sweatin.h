#include <node.h>
#include <v8.h>
#include <stdio.h>
#include <stdlib.h>
#include <string>

#include "NiTE.h"

#include "../Common/NiteSampleUtilities.h"

using namespace v8;


/* hand tracker */
nite::HandTracker handTracker;

/* if thread should be locked and running */
bool keepWorkerRunning;

/* thread loop */
uv_loop_t *loop;

/* async thread ref */
uv_async_t async;

/* context for sending NodeJS callbacks */
Persistent<Object> context_obj;

/* NodeJS Methods */
void frameWorker(uv_work_t *req);
void onFrameWorkerThreadComplete(uv_work_t* req);
void onMotionEvent(uv_async_t *handle, int status /*UNUSED*/);
Handle<Value> initialize(const Arguments& args);
Handle<Value> close(const Arguments& args);
Handle<Value> setListener(const Arguments& args);
Handle<Value> testListener(const Arguments& args);