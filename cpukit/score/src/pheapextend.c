/**
 *  COPYRIGHT (c) 1989-2007.
 *  On-Line Applications Research Corporation (OAR).
 *
 *  The license and distribution terms for this file may be
 *  found in the file LICENSE in this distribution or at
 *  http://www.rtems.com/license/LICENSE.
 *
 *  $Id$
 */

#if HAVE_CONFIG_H
#include "config.h"
#endif

#include <rtems/system.h>
#include <rtems/score/protectedheap.h>

bool _Protected_heap_Extend(
  Heap_Control *the_heap,
  void         *starting_address,
  intptr_t      size
)
{
  Heap_Extend_status status;
  intptr_t           amount_extended;

  _RTEMS_Lock_allocator();
    status = _Heap_Extend(the_heap, starting_address, size, &amount_extended);
  _RTEMS_Unlock_allocator();
  return (status == HEAP_EXTEND_SUCCESSFUL);
}

