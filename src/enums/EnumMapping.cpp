#include "../enums/GestureTypes.h"
#include "../enums/ErrorTypes.h"
#include "../enums/EventTypes.h"
#include "../enums/EnumMapping.h"
#include <string>

namespace EnumMapping {
    /**
     * map event ID to NodeJS String
     *
     * @param event ID
     * @event string
     */
    std::string mapEventToLabel(int event) {
        if (event >= 0 && event < 1000) {
            return mapGeneralEventToLabel(event);
        }

        if (event >= 1000 && event < 2000) {
            return mapGestureToLabel(event);
        }

        if (event >= 3000 && event < 4000) {
            return mapGestureStepToLabel(event);
        }

        if (event >= 4000 && event < 5000) {
            return mapJointToLabel(event);
        }

        if (event >= 9000 && event < 10000) {
            return mapErrorToLabel(event);
        }

        return "No known event type found";
    }

    /**
     * map event ID to NodeJS String
     *
     * @param event ID
     * @event string
     */
    std::string mapGeneralEventToLabel(int event) {
        switch (event) {
            case DEVICE_INITIALIZED:
                return "DEVICE_INITIALIZED";
            case DEVICE_ERROR:
                return "DEVICE_ERROR";
            case NEW_USER:
                return "NEW_USER";
            case USER_IS_VISIBLE:
                return "USER_IS_VISIBLE";
            case USER_IS_OUT_OF_SCENE:
                return "USER_IS_OUT_OF_SCENE";
            case USER_IS_LOST:
                return "USER_IS_LOST";
            case SKELETON_STOPPED_TRACKING:
                return "SKELETON_STOPPED_TRACKING"; 
            case SKELETON_TRACKING:
                return "SKELETON_TRACKING"; 
            case SKELETON_CALIBRATING:
                return "SKELETON_CALIBRATING"; 
            case SKELETON_CALIBRATION_FAILED:
                return "SKELETON_CALIBRATION_FAILED";

            default:
                return "No known event type found";
        }
    }

    /**
     * map joint ID to NodeJS String
     *
     * @param jotin ID
     * @return string
     */
    std::string mapJointToLabel(int event) {
        switch (event) {
            case HAND_RIGHT:
                return "RIGHT_HAND";
            case HAND_LEFT:
                return "LEFT_HAND";
            default:
                return "No hand found";
        }
    }

    /**
     * map joint name to int
     *
     * @param joint name
     * @return gesture id
     */
    int mapJointToLabel(std::string name) {
        if (name == "RIGHT_HAND") return HAND_RIGHT;
        if (name == "LEFT_HAND") return HAND_LEFT;
        return -1;
    }

    /**
     * map gesture step
     *
     * @param gesture step ID
     * @return string
     */
    std::string mapGestureStepToLabel(int event) {
        switch (event) {
            case GESTURE_STEP_START:
                return "GESTURE_START";
            case GESTURE_STEP_COMPLETE:
                return "GESTURE_COMPLETE";
            case GESTURE_STEP_CANCELLED:
                return "GESTURE_CANCELLED";
            default:
                return "No step found";
        }
    }

    /**
     * map hand name to int
     *
     * @param hand name
     * @return gesture id
     */
    int mapGestureStepToLabel(std::string name) {
        if (name == "GESTURE_START") return GESTURE_STEP_START;
        if (name == "GESTURE_COMPLETE") return GESTURE_STEP_COMPLETE;
        if (name == "GESTURE_CANCELLED") return GESTURE_STEP_CANCELLED;
        return -1;
    }

    /**
     * map gesture ID to NodeJS String
     *
     * @param gesture ID
     * @event string
     */
    std::string mapGestureToLabel(int event) {
        switch (event) {
            case SWIPE_LEFT:
                return "SWIPE_LEFT";
            case SWIPE_RIGHT:
                return "SWIPE_RIGHT";
            case SWIPE_UP:
                return "SWIPE_UP";
            case SWIPE_DOWN:
                return "SWIPE_DOWN";
            case WAVE_HAND:
                return "WAVE_HAND";
            default:
                return "No known event type found";
        }
    }

    /**
     * map gesture name to int
     *
     * @param gesture name
     * @event gesture id
     */
    int mapGestureToLabel(std::string name) {
        if (name == "SWIPE_GESTURE") return GESTURE_CATEGORY_SWIPE;
        if (name == "WAVE_GESTURE") return GESTURE_CATEGORY_WAVE;
        if (name == "SWIPE_LEFT") return SWIPE_LEFT;
        if (name == "SWIPE_RIGHT") return SWIPE_RIGHT;
        if (name == "SWIPE_UP") return SWIPE_UP;
        if (name == "SWIPE_DOWN") return SWIPE_DOWN;
        if (name == "WAVE_HAND") return WAVE_HAND;
        return -1;
    }

    /**
     * map error ID to NodeJS String
     *
     * @param error ID
     * @event string
     */
    std::string mapErrorToLabel(int event) {
        switch (event) {
            case ERROR_GENERAL_DEVICE_ERROR:
                return "ERROR_GENERAL_DEVICE_ERROR";
            case ERROR_NITE_INITIALIZATION:
                return "ERROR_NITE_INITIALIZATION";
            case ERROR_USER_TRACKER_CREATION:
                return "ERROR_USER_TRACKER_CREATION";
            default:
                return "No known event type found";
        }
    }
}