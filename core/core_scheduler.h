/**
* @file core_scheduler.h
*
* core_scheduler
*
*   core_scheduler.
*
* @author Ricardo <tsao.ricardo@iac.com.tw>
*/

#pragma once

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#include "core_common.h"

#define SCHEDULER_DELAY_LIMIT	100

#define GET_SCHEDULER_LOCALS() {}
#define STATIC_UNIT_TESTED static

#if !defined(UNUSED)
#define UNUSED(x) (void)(x)
#endif

#if 1
#define MIN(a,b) \
  __extension__ ({ __typeof__ (a) _a = (a); \
  __typeof__ (b) _b = (b); \
  _a < _b ? _a : _b; })
#define MAX(a, b)  (((a) > (b)) ? (a) : (b)) 
#define ABS(x) \
  __extension__ ({ __typeof__ (x) _x = (x); \
  _x > 0 ? _x : -_x; })
#else

#endif

//#include "common/time.h"
// time difference, 32 bits always sufficient
typedef int32_t timeDelta_t;
// millisecond time
typedef uint32_t timeMs_t;
// microsecond time
#ifdef USE_64BIT_TIME
typedef uint64_t timeUs_t;
#define TIMEUS_MAX UINT64_MAX
#else
typedef uint64_t timeUs_t;
#define TIMEUS_MAX UINT32_MAX
#endif
//

typedef enum {
	TASK_PRIORITY_IDLE = 0,     // Disables dynamic scheduling, task is executed only if no other task is active this cycle
	TASK_PRIORITY_LOW = 1,
	TASK_PRIORITY_MEDIUM = 3,
	TASK_PRIORITY_MEDIUM_HIGH = 4,
	TASK_PRIORITY_HIGH = 5,
	TASK_PRIORITY_REALTIME = 6,
	TASK_PRIORITY_MAX = 255
} cfTaskPriority_e;

typedef struct {
	timeUs_t     maxExecutionTime;
	timeUs_t     totalExecutionTime;
	timeUs_t     averageExecutionTime;
} cfCheckFuncInfo_t;

typedef struct {
	const char * taskName;
	const char * subTaskName;
	bool         isEnabled;
	uint8_t      staticPriority;
	timeDelta_t  desiredPeriod;
	timeDelta_t  latestDeltaTime;
	timeUs_t     maxExecutionTime;
	timeUs_t     totalExecutionTime;
	timeUs_t     averageExecutionTime;
} cfTaskInfo_t;

typedef enum {
	/* Actual tasks */
	TASK_SYSTEM = 0,

	/* Count of real tasks */
	TASK_COUNT,

	/* Service task IDs */
	TASK_NONE = TASK_COUNT,
	TASK_SELF
} cfTaskId_e;

typedef struct {
	// Configuration
	const char * taskName;
	const char * subTaskName;
	bool(*checkFunc)(timeUs_t currentTimeUs, timeDelta_t currentDeltaTimeUs);
	void(*taskFunc)(timeUs_t currentTimeUs);
	timeDelta_t desiredPeriod;      // target period of execution
	const uint8_t staticPriority;   // dynamicPriority grows in steps of this size, shouldn't be zero

	// Scheduling
	uint16_t dynamicPriority;       // measurement of how old task was last executed, used to avoid task starvation
	uint16_t taskAgeCycles;
	timeDelta_t taskLatestDeltaTime;
	timeUs_t lastExecutedAt;        // last time of invocation
	timeUs_t lastSignaledAt;        // time of invocation event for event-driven tasks

#ifndef SKIP_TASK_STATISTICS
	// Statistics
	timeUs_t movingSumExecutionTime;  // moving sum over 32 samples
	timeUs_t maxExecutionTime;
	timeUs_t totalExecutionTime;    // total time consumed by task since boot
#endif
} cfTask_t;

#if 1
extern cfTask_t cfTasks[TASK_COUNT];
extern uint16_t averageSystemLoadPercent;
#else

#endif

void getCheckFuncInfo(cfCheckFuncInfo_t *checkFuncInfo);
void getTaskInfo(cfTaskId_e taskId, cfTaskInfo_t *taskInfo);
void rescheduleTask(cfTaskId_e taskId, uint32_t newPeriodMicros);
void setTaskEnabled(cfTaskId_e taskId, bool newEnabledState);
timeDelta_t getTaskDeltaTime(cfTaskId_e taskId);
void schedulerSetCalulateTaskStatistics(bool calculateTaskStatistics);
void schedulerResetTaskStatistics(cfTaskId_e taskId);

void schedulerInit(void);
void scheduler(void);
void taskSystem(timeUs_t currentTime);

#define LOAD_PERCENTAGE_ONE 100

#define isSystemOverloaded() (averageSystemLoadPercent >= LOAD_PERCENTAGE_ONE)

#ifdef __cplusplus
} /* end extern "C" */
#endif /* __cplusplus */
