#include <node.h>
#include <v8.h>
#include <stdio.h>
#include <stdlib.h>
#include <string>

#include "NiTE.h"
#include "../enums/Skeleton.h"
#include "../enums/Joint.h"
#include "../enums/GestureTypes.h"
 #include "../enums/ErrorTypes.h"
  #include "../enums/EventTypes.h"
#include "../gestures/GestureRecognizer.h"

#include "../Common/NiteSampleUtilities.h"

using namespace v8;

/* thread loop */
uv_loop_t *loop;

/* async thread ref */
uv_async_t async;

/* context for sending NodeJS callbacks */
Persistent<Object> context_obj;

/** user tracker */
nite::UserTracker userTracker;

/** event ID to send - weird how it needs to be global or the pointers get all screwed up, bah humbug C++*/
int eventIDToSend;

/** gesture ID to send - weird how it needs to be global or the pointers get all screwed up, bah humbug C++*/
int gestureIDToSend;

/** boolean that indicates if worker thread should keep running */
bool keepWorkerRunning;

/** is user visible */
bool isUserVisible;

/** is user in scene */
bool isUserInScene;

/** is skeleton tracking */
bool isSkeletonTracking;

/** is skeleton calibrating */
bool isSkeletonCalibrating;

/** skeleton */
Skeleton skeleton;

/** gesture recognizer */
GestureRecognizer gst;

void frameWorker(uv_work_t *req);
void onFrameWorkerThreadComplete(uv_work_t* req);
void sendEventToNode(int eventType);
void sendEventFromThreadToNode(uv_async_t *handle, int status);
void updateUserState(const nite::UserData& user);
void mapJointFromSkeleton(Joint &j, nite::Skeleton s);
void mapSkeleton(Skeleton &skeleton, nite::Skeleton niteskeleton);
Local<String> getEventLabel(int event);
Local<Object> mapJointToNodeObject(char *jointName);

/* NodeJS Methods */
Handle<Value> initialize(const Arguments& args);
Handle<Value> close(const Arguments& args);
Handle<Value> setListener(const Arguments& args);
Handle<Value> getJoints(const Arguments& args);
Handle<Value> addGestureListener(const Arguments& args);
Handle<Value> removeGestureListener(const Arguments& args);