/**
 * @file
 *
 * @brief Function Destroys a read-write lock Attributes object 
 * @ingroup POSIXAPI
 */

/*
 *  COPYRIGHT (c) 1989-2007.
 *  On-Line Applications Research Corporation (OAR).
 *
 *  The license and distribution terms for this file may be
 *  found in the file LICENSE in this distribution or at
 *  http://www.rtems.com/license/LICENSE.
 */

#if HAVE_CONFIG_H
#include "config.h"
#endif

#include <pthread.h>
#include <errno.h>

#include <rtems/system.h>
#include <rtems/score/object.h>
#include <rtems/score/states.h>
#include <rtems/score/watchdog.h>
#include <rtems/posix/condimpl.h>
#include <rtems/posix/time.h>
#include <rtems/posix/muteximpl.h>

/*
 *  RWLock Initialization Attributes
 */

int pthread_rwlockattr_destroy(
  pthread_rwlockattr_t *attr
)
{
  if ( !attr || attr->is_initialized == false )
    return EINVAL;

  attr->is_initialized = false;
  return 0;
}
