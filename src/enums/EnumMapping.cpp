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
            case WAVE_LEFT:
                return "WAVE_LEFT";
            case WAVE_RIGHT:
                return "WAVE_RIGHT";
            case WAVE:
                return "WAVE";
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
        if (name == "WAVE_LEFT") return WAVE_LEFT;
        if (name == "WAVE_RIGHT") return WAVE_RIGHT;
        if (name == "WAVE") return WAVE;
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