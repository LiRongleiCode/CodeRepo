/*
   ==========================================================================

   Filename :        apptick.c

   Author :          Marco Constant (mconstant@stryker.comen)

   Public prefix :   tick

   Project No :      IMS-168

   Product:          IsoMax

   Creation date :   05/27/2020

   ==========================================================================

   Modifications
   -------------

   See header file for more informations

   ==========================================================================
*/

/** 
 *  \file      
 *  \ingroup apptck
 *  \brief Application Timer/Counter 
 *
 * This unit implements the system timer/counter.  It provides an initialization function tick_Init()
 * called by the main loop to initialize internal member.  The application shall define a real time
 * interrupt with a granularity of 0.1 ms.  That interrupt shall then call the tick_Event() function
 * to allow the unit to keep track of the time.
 * 
 * The unit provides a system time that starts at power on and is not reset.  It is used for long 
 * term timing functions such as the 4 hr limit from the start of therapy to reach the desired 
 * temperature.  These times are single events. The time at the start of the period is captured and 
 * stored and subsequently compared to the current time to determine if the desired amount of time 
 * has expired.  The base counter for this unit is a 32 bit word which keeps track of milliseconds. 
 * This gives it a rollover period of approximately 2 days.  If a longer period is desired it 
 * also provides a 32 bit minute counter which has a rollover period of approximately 8172 years.
 *
 * The application can call tick_GetTickCount() to initialize required timer/counter. Then to
 * evaluate the completion of a timer, the application calls tick_GetElapseTime() to get the
 * difference in millisecond since the timer/counter was initialized.  
 *
 * There is also helper functions to handle relative system date since the last reset of the MCU.
 * The unit does not handle real time calendar but relative time to the last reset.
 *
 * One more helper function (tick_WaitFor()) allow the application to block the process for a specific amount of time.
 * The application should be careful using this function because the application will block until
 * the delay is completed and it may cause the watchdog to reset if the delay is longer than the watchdog
 * timer.
 *
 * Note: When the file is use with MQX, these functions are wrappers of the equivalent MQX functions.
 *
 * <h2>Code Example</h2>
 *  
 *  

\code
// In this example the function DoSomething() will be called every 5 seconds.
dword dwTimer = 0;
dwTimer = tick_GetTickCount();

for (;;)
{
   if (tick_GetElapsedTime(dwTime) >= 5000)
   {
      DoSomething();
      dwTimer = tick_GetTickCount();
   }
}
\endcode
*/

#include "apptick.h"

/*! \brief Private member that keep the internal tick count.
 *
 *   Incremented by tick_Event().
 *   The function tick_Event() is called every 1 ms.  The #tick_TickCount is
 *   incremented by 1 every time event occur.  Therefore, the count is in milliseconds.
 */
static volatile timer_t tick_TickCount = 0;

/*! \brief Private member that is used to accelerate the internal tick counter for testing.
 */
//static timer_t tick_AddTickCount = 0;
            
/*! \brief Private member that keep the internal minutes count.
 *
 *  Incremented by tick_Event().
 *  The count is in minutes and represents the number of minutes since 2012-01-01 at 00:01.
 *  So one day will have 1440 counts.
 */
static volatile timer_t tick_CurrentDate = 0;

/*! \brief Private member that keep the internal seconds count.
 *
 *   Incremented by tick_Event().
 */
static volatile timer_t tick_SecCounter = 0;

/*!
 * \brief Event Counter called by the system interrupt timer
 *
 * This method is called by the interrupt every 0.1 ms.  
 *
 * Local static globals:
 * - #tick_TickCount
 * - #tick_SecCounter
 * - #tick_CurrentDate
 *
 */
void tick_Event( void )
{
    tick_TickCount++;
}

/*!
 * \brief Get Tick Count in milliseconds
 *
 * This function return the number of tick since the MCU was reset.  The reset
 * could be software (ex. watchdog) or hardware.
 *
 * Local static globals:
 * - #tick_TickCount
 *
 * \return Current tick count
 */
timer_t tick_GetTickCount(void)
{
    return tick_TickCount;
}


/*!
 * \brief Initialization of internal counters
 *
 * This function reset to 0 the internal counters.  Shall be called at the reset
 * of the MCU. When use in the context of MQX, the call to this
 * function is not necessary.  MQX will take care of the initialization.
 *
 * Local static globals:
 * - #tick_TenthTickCount
 * - #tick_TickCount
 * - #tick_SecCounter
 * - #tick_CurrentDate
 *
 */
void tick_Init(void)
{
    tick_TickCount = 0;
}

/*!
 * \brief Get Elapsed Time in milliseconds
 *
 * This function return the elapsed time in ms since \a dwFromTime was initialized.
 * A particular attention is taken in case the #tick_TickCount overflow the maximum
 * capability of the dword type.
 *
 * Local static globals:
 * - #tick_TickCount
 *
 * \param[in] fromTime Identifier the timer starting count (previously initialized with tick_GetTickCountMS()).
 * \return    Number of millisecond since the given timer was initialized.
 */
timer_t tick_GetElapsedTime(timer_t fromTime)
{
    timer_t result = 0;     // Function returned value corresponding to the elapsed time
    
    result = tick_GetTickCount();
    
    // Check for overflow value
    if (result < fromTime)
    {
        result += MAXVAL_UWORD32 - fromTime + 1;
    }
    else
    {
        result -= fromTime;
    }
    
    return result;
}





/*!
 * \brief Add Time to Tick Counter
 *
 * Add time to the tick counter to make the system to move faster in time.
 * Used for system test to accelerate some timer delay
 *
 * Local static globals:
 * - #tick_AddTickCount
 *
 * \param[in] addSeconds  Number of seconds to add to the tick counter
 *
 */


// **************************  END OF FILE **************************** //
