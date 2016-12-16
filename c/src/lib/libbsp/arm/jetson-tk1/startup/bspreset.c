#include <bsp/bootcard.h>
#include <bsp.h>
#include <rtems.h>

void bsp_reset(void)
{
  printk("Jailhouse is not capable of resetting - wait\n");

  while (1) ;
}
