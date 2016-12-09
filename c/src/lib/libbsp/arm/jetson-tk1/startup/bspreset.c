/*
 * Copyright (c) OTH Regensburg, 2017
 *
 * The license and distribution terms for this file may be
 * found in the file LICENSE in this distribution or at
 * http://www.rtems.org/license/LICENSE.
 */

#include <bsp/bootcard.h>
#include <bsp.h>
#include <rtems.h>

void bsp_reset( void )
{
  printk( "Jailhouse is not capable of resetting - wait\n" );

  while ( 1 ) ;
}
