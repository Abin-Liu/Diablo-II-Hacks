///////////////////////////////////////////////////////////////
// MotionJob.cpp
//
// Abin (abinn32@yahoo.com)
///////////////////////////////////////////////////////////////

#include "MotionJob.h"
#include "D2HackIt.h"
#include "me.h"
#include "Utilities\\StateTicker.h"
#include "GameMessage.h"
#include "Server20.h"

#define MOVE_OFFSET			5		// maximum offset allowed for walk/run
#define NPC_OFFSET			5		// maximum offset allowed for "run to npc"
#define TELEPORT_TICK		600		// Teleport checking interval
#define MOVE_TICK			300		// walk/run checking interval
#define MAX_JOB_RETRY		5

CLinkedList<MOTIONJOB, const MOTIONJOB&, NULL> g_aJobQueue;
MOTIONJOB g_prevJob = { 0 };	// Recent job
CStateTicker g_jobTicker;
int g_nJobRetry = 0;

void Detour(WORD x, WORD y);

void AddMotionJob(const MOTIONJOB& job)
{
	g_aJobQueue.Lock();
	g_aJobQueue.InsertRear(job);
	g_aJobQueue.Unlock();
}

void EXPORT CleanJobs()
{
	g_jobTicker.Clear();
	g_aJobQueue.Lock();
	g_aJobQueue.RemoveAll();
	g_aJobQueue.Unlock();
	::memset(&g_prevJob, 0, sizeof(MOTIONJOB));
	g_nJobRetry = 0;
}

void CompleteJobs(BOOL bSuccessful)
{
	CleanJobs();	
	SendThisPlayerMessage(PM_MOVECOMPLETE, (WPARAM)bSuccessful);
}

void EXPORT PauseJobs(BOOL bPause)
{
	if (bPause)
		g_jobTicker.Pause();
	else
		g_jobTicker.Resume();
}

BOOL IsStanding()
{
	return GetMode() == PLAYER_MODE_STAND_INTOWN || GetMode() == PLAYER_MODE_STAND_OUTTOWN;
}

void CALLBACK JobTimerTick(LPVOID lpParam, const volatile BYTE* lpStopFlag)
{
	if (!g_bGameJoined || NEED_ABORT(lpStopFlag))
		return;
	
	g_jobTicker.TickIt(100);
	if (!g_jobTicker.IsTickEnded())
		return;
	
	POINT mp = {0};
	switch (g_prevJob.nJobType)
	{
	case MJ_TELEPORT:
		if (GetDistanceFrom(g_prevJob.x, g_prevJob.y) <= MOVE_OFFSET)
		{
			StepDone();	
		}
		else
		{
			if (g_nJobRetry <= MAX_JOB_RETRY)
			{				
				g_nJobRetry++;
				DoFirstJob(TRUE);
				g_jobTicker.Update(MJ_TELEPORT, TELEPORT_TICK + g_nJobRetry * 100);
			}
			else
			{
				CompleteJobs(FALSE);
			}
		}
		
		break;

	case MJ_POSITION:
		if (!IsStanding())
			break;
		
		if (GetDistanceFrom(g_prevJob.x, g_prevJob.y) <= MOVE_OFFSET)
		{
			StepDone();	
		}
		else
		{
			if (g_nJobRetry <= MAX_JOB_RETRY * 2)
			{
				Detour(g_prevJob.x, g_prevJob.y);
				g_nJobRetry++;
				DoFirstJob(TRUE);
			}
			else
			{
				CompleteJobs(FALSE);
			}
		}
		
		break;
		
	case MJ_UNIT:
		if (!IsStanding())
			break;

		mp = GetUnitPosition(&g_prevJob.unit);
		if (GetDistanceFrom(mp.x, mp.y) <= MOVE_OFFSET)
		{
			StepDone();	
		}
		else
		{
			if (g_nJobRetry <= MAX_JOB_RETRY * 3)
			{
				Detour(g_prevJob.x, g_prevJob.y);
				g_nJobRetry++;
				DoFirstJob(TRUE);
			}
			else
			{
				CompleteJobs(FALSE);
			}
		}
		
		break;

	default:
		DoFirstJob(FALSE);
		break;
	}
}

BOOL DoFirstJob(BOOL bRedo)
{
	static POINT pt = { 0 };
	if (!bRedo)
	{
		g_nJobRetry = 0;
		g_aJobQueue.Lock();
		POSITION pos = g_aJobQueue.GetHeadPosition();
		if (pos == NULL)
		{
			g_aJobQueue.Unlock();
			return TRUE;
		}

		// there are jobs in queue
		g_prevJob = g_aJobQueue.GetNext(pos);
		g_aJobQueue.RemoveFront();
		g_aJobQueue.Unlock();
	}	

	BOOL bResult = FALSE;
	switch (g_prevJob.nJobType)
	{
	case MJ_POSITION:
		bResult = GoToPosition(g_prevJob.x, g_prevJob.y);
		if (bResult)
			g_jobTicker.Update(MJ_POSITION, MOVE_TICK);
		break;

	case MJ_UNIT:
		pt = GetUnitPosition(&g_prevJob.unit);
		bResult = GoToEntity(&g_prevJob.unit, (WORD)pt.x, (WORD)pt.y, TRUE);
		if (bResult)
			g_jobTicker.Update(MJ_UNIT, MOVE_TICK);
		break;

	case MJ_TELEPORT:
		bResult = CastOnMap(D2S_TELEPORT, g_prevJob.x, g_prevJob.y);
		if (bResult)
			g_jobTicker.Update(MJ_TELEPORT, TELEPORT_TICK);
		break;

	default:
		bResult = FALSE;
		break;
	}

	if (!bResult)
		CompleteJobs(FALSE);

	return bResult;
}

void StepDone()
{
	g_nJobRetry = 0;
	g_aJobQueue.Lock();
	BOOL bEmpty = g_aJobQueue.IsEmpty();
	g_aJobQueue.Unlock();

	if (bEmpty)
	{
		// no more jobs left, notify the application that job is completed
		CompleteJobs(TRUE);
	}
	else
	{
		// There are more jobs, do next job
		DoFirstJob(FALSE);
	}	
}

////////////////////////////////////////////////////////////////////////
// Detour
//----------------------------------------------------------------------
// Making an intelligent detour when the player is stuck. Small algorithm
// but extremely handy.
// 
// Abin (abinn32@yahoo.com)
////////////////////////////////////////////////////////////////////////
void Detour(WORD x, WORD y)
{	
	// When we are stuck somewhere, do a random move and the angles will 
	// be set to avoid the direction
	const int ANGLE = GetAngleFrom(x, y);

	// pull back 2 yards
	POINT ptPullBack = GetRandomPosition(2, 2, ANGLE + 180, ANGLE + 180);
	GoToPosition((WORD)ptPullBack.x, (WORD)ptPullBack.y, TRUE);

	Sleep(200); // wait for 200 ms to complete the pull-back

	// then move towards a good direction 3 yards
	POINT pt1 = GetRandomPosition(3, 3, ANGLE - 90, ANGLE - 90); // detour route 1
	POINT pt2 = GetRandomPosition(3, 3, ANGLE + 90, ANGLE + 90); // detour route 2

	if (::GetTickCount() % 2) // pick one good direction
		GoToPosition((WORD)pt1.x, (WORD)pt1.y, TRUE);
	else
		GoToPosition((WORD)pt2.x, (WORD)pt2.y, TRUE);

	Sleep(200); // wait for 200 ms to complete the detour
}



