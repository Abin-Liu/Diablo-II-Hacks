///////////////////////////////////////////////////////////////
// MotionJob.cpp
//
// Abin (abinn32@yahoo.com)
///////////////////////////////////////////////////////////////

#include "MotionJob.h"
#include "D2HackIt.h"
#include "me.h"
#include "Utilities\\StateTicker.h"

#define MOVE_OFFSET			8		// maximum offset allowed for walk/run
#define NPC_OFFSET			8		// maximum offset allowed for "run to npc"
#define TELEPORT_TICK		600		// Teleport checking interval
#define MOVE_TICK			300		// walk/run checking interval

CLinkedList<MOTIONJOB, const MOTIONJOB&, NULL> g_aJobQueue;
MOTIONJOB g_prevJob = { 0 };	// Recent job
CStateTicker g_jobTicker;
int g_nJobRetry = 0;

void AddMotionJob(const MOTIONJOB& job)
{
	g_aJobQueue.InsertRear(job);
}

void OnJobTicker(DWORD dwTicker)
{
	g_jobTicker.TickIt(dwTicker);
}

void EXPORT CleanJobs()
{
	g_jobTicker.Clear();
	g_aJobQueue.RemoveAll();
	::memset(&g_prevJob, 0, sizeof(MOTIONJOB));
	g_nJobRetry = 0;
}

void CompleteJobs(BOOL bSuccessful)
{
	CleanJobs();	
	NotifyThisPlayerMessage(PM_MOVECOMPLETE, (WPARAM)bSuccessful);
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
	return GetMode() == MODE_STAND_INTOWN || GetMode() == MODE_STAND_OUTTOWN;
}

void DoJobProc()
{
	if (!g_jobTicker.IsTickEnded())
		return;
	
	MAPPOS mp = {0};
	switch (g_prevJob.nJobType)
	{
	case MJ_TELEPORT:
		if (GetDistanceFrom(g_prevJob.x, g_prevJob.y) <= MOVE_OFFSET)
		{
			StepDone();	
		}
		else
		{
			if (g_nJobRetry <= MAX_RETRY)
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
			if (g_nJobRetry <= MAX_RETRY * 3)
			{
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
			if (g_nJobRetry <= MAX_RETRY * 3)
			{
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
	if (!bRedo)
	{
		g_nJobRetry = 0;
		POSITION pos = g_aJobQueue.GetHeadPosition();
		if (pos == NULL)
			return TRUE;

		// there are jobs in queue
		g_prevJob = g_aJobQueue.GetNext(pos);
		g_aJobQueue.RemoveFront();
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
		bResult = GoToEntity(&g_prevJob.unit);
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
	if (g_aJobQueue.IsEmpty())
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



