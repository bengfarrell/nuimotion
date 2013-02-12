#include <node.h>
#include <v8.h>
#include <stdio.h>
#include <stdlib.h>
#include <string>

#include "NiTE.h"

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

/** boolean that indicates if worker thread should keep running */
bool keepWorkerRunning;

/** Joint structure */
struct Joint {
    int xPos;
    int yPos;
    int zPos;
    int type;
    bool isActive;
};

/** left hand skeletal joint */
Joint joint_leftHand;

/** right hand skeletal joint */
Joint joint_rightHand;

/** last user event type dispatched */
int lastUserEventDispatched;

/** last skeletal event type dispatched */
int lastSkeletalEventDispatched;

/** current user state */
int currentUserState = 0;

/** current skeleton state */
int currentSkeletonState = 0;

void frameWorker(uv_work_t *req);
void onFrameWorkerThreadComplete(uv_work_t* req);
void onTrackingEvent(uv_async_t *handle, int status /*UNUSED*/);
void onDeviceEvent(int eventType); 
void updateUserState(const nite::UserData& user, unsigned long long ts);

/* NodeJS Methods */
Handle<Value> initialize(const Arguments& args);
Handle<Value> close(const Arguments& args);
Handle<Value> setListener(const Arguments& args);
Handle<Value> getHands(const Arguments& args);