#include "WaypointDef.h"

WORD GetWaypointFingerprinterData(BYTE iMapID)
{
	for (int i = 0; i < D2X_WAYPOINT_COUNT; i++)
	{
		if (D2X_WAYPOINTS[i].iMapID == iMapID)
			return D2X_WAYPOINTS[i].wFingerprinterData;
	}
	return 0;
}