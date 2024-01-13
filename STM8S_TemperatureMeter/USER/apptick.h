/*
   ==========================================================================

   Filename :        tick.h

   Author :          Marco Constant (mconstant@stryker.com)

   Public prefix :   tick

   Project No :      IMS-166

   Product:          IsoAir

   Creation date :   2012/02/12

   ==========================================================================

   Modifications
   -------------

   Par   |   Date   | Version | Description
   ------+----------+---------+----------------------------------------------
   M.C.  | 02/15/12 | 1.0a    | Creation
   ==========================================================================
*/

/** \file
 *  \ingroup apptck
 *  \brief  Application Timer/Counter Header
 *
 *  This file declare public functions, defines and types used for Application Counter unit.
 *
 *  \sa apptick.c
 */

#ifndef APPTICK_H_
#define APPTICK_H_

#ifdef __cplusplus
extern "C" {
#endif


#ifndef MAXVAL_UWORD32
#define MAXVAL_UWORD32 0xFFFF    /**< Constant to define the maximum value of the counter to manager the overflow */
#endif

#define MAX_TICK_FOR_MINUTE    6000  /**< Constant to define the application counter value for 1 minutes (60 sec * 100 counts/sec  ) */
  
typedef unsigned int timer_t;          /**< define the type used for all timers as a 32 bits **/

// Function Prototypes
void     tick_Event( void );
void     tick_Init(void);
timer_t  tick_GetTickCount(void);
timer_t  tick_GetElapsedTime(timer_t fromTime);


#ifdef __cplusplus
}
#endif

#endif  /* APPTICK_H_ */

// **************************  END OF FILE **************************** //
