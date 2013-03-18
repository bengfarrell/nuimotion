#include "../enums/GestureTypes.h"
#include "../enums/ErrorTypes.h"
#include "../enums/EventTypes.h"
#include <string>

namespace EnumMapping
{
	/**
	 * map event ID to NodeJS String
	 *
	 * @param event ID
     * @event string
	 */
	std::string mapEventToLabel(int event);

	/**
	 * map event ID to NodeJS String
	 *
	 * @param event ID
     * @event string
	 */
	std::string mapGeneralEventToLabel(int event);

	/**
	 * map gesture ID to NodeJS String
	 *
	 * @param gesture ID
     * @event string
	 */
	std::string mapGestureToLabel(int event);

	/**
	 * map gesture name to int
	 *
	 * @param gesture name
     * @event gesture id
	 */
	int mapGestureToLabel(std::string name);

	/**
	 * map error ID to NodeJS String
	 *
	 * @param error ID
     * @event string
	 */
	std::string mapErrorToLabel(int event);
}