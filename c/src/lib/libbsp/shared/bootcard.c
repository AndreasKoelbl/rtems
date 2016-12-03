/**
 * @file
 *
 * @ingroup bsp_bootcard
 *
 * @brief Standard system startup.
 *
 *  This is the C entry point for ALL RTEMS BSPs.  It is invoked
 *  from the assembly language initialization file usually called
 *  start.S.  It provides the framework for the BSP initialization
 *  sequence.  For the basic flow of initialization see RTEMS C User's Guide,
 *  Initialization Manager.
 *
 *  This style of initialization ensures that the C++ global
 *  constructors are executed after RTEMS is initialized.
 *  Thanks to Chris Johns <cjohns@plessey.com.au> for the idea
 *  to move C++ global constructors into the first task.
 */

/*
 *  COPYRIGHT (c) 1989-2014.
 *  On-Line Applications Research Corporation (OAR).
 *
 *  The license and distribution terms for this file may be
 *  found in the file LICENSE in this distribution or at
 *  http://www.rtems.org/license/LICENSE.
 */

#include <bsp/bootcard.h>

#include <rtems.h>
#include <rtems/sysinit.h>

/*
 *  At most a single pointer to the cmdline for those target
 *  short on memory and not supporting a command line.
 */
const char *bsp_boot_cmdline;

RTEMS_SYSINIT_ITEM(
  bsp_work_area_initialize,
  RTEMS_SYSINIT_BSP_WORK_AREAS,
  RTEMS_SYSINIT_ORDER_MIDDLE
);

RTEMS_SYSINIT_ITEM(
  bsp_start,
  RTEMS_SYSINIT_BSP_START,
  RTEMS_SYSINIT_ORDER_MIDDLE
);

RTEMS_SYSINIT_ITEM(
  bsp_predriver_hook,
  RTEMS_SYSINIT_BSP_PRE_DRIVERS,
  RTEMS_SYSINIT_ORDER_MIDDLE
);

static void writechar(char out)
{
  register uint32_t num_res asm("r0") = 8;
	register uint32_t arg1 asm("r1") = out;

	asm volatile(
		".arch_extension virt\n\t"
		"hvc #0x4a48\n\t"
		: "=r" (num_res)
		: "r" (num_res), "r" (arg1)
		: "memory");
}

void swap(char *first, char *second)
{
  char temp = *first;
  *first = *second;
  *second = temp;
}

void reverse(char str[], int length)
{
    int start = 0;
    int end = length -1;
    while (start < end)
    {
        swap(*(str+start), *(str+end));
        start++;
        end--;
    }
}

// Implementation of itoa()
char* itoa(uint32_t num, char* str, uint32_t base)
{
    int i = 0;

    /* Handle 0 explicitely, otherwise empty string is printed for 0 */
    if (num == 0)
    {
        str[i++] = '0';
        str[i] = '\0';
        return str;
    }

    // In standard itoa(), negative numbers are handled only with
    // base 10. Otherwise numbers are considered unsigned.
    // Process individual digits
    while (num != 0)
    {
        int rem = num % base;
        str[i++] = (rem > 9)? (rem-10) + 'a' : rem + '0';
        num = num/base;
    }

    str[i] = '\0'; // Append string terminator

    // Reverse the string
    reverse(str, i);

    return str;
}

/*
 *  This is the initialization framework routine that weaves together
 *  calls to RTEMS and the BSP in the proper sequence to initialize
 *  the system while maximizing shared code and keeping BSP code in C
 *  as much as possible.
 */
void boot_card(
  const char *cmdline
)
{
  rtems_interrupt_level  bsp_isr_level;

  register uint32_t num_res asm("r0") = 8;

  register uint32_t stackpointer asm("sp");
  register uint32_t framepointer asm("fp");

  char target[32];
  char* result;
  /*
   *  Make sure interrupts are disabled.
   */
  (void) bsp_isr_level;
  rtems_interrupt_local_disable( bsp_isr_level );

  bsp_boot_cmdline = cmdline;

  writechar('I');

  result = itoa(stackpointer, target, 16);
  for (uint8_t j = 0; j < 32; j++)
  {
    writechar(target[j]);
  }

  result = itoa(framepointer, target, 16);
  for (uint8_t j = 0; j < 32; j++)
  {
    writechar(target[j]);
  }



  rtems_initialize_executive();

  /***************************************************************
   ***************************************************************
   *  APPLICATION RUNS NOW!!!  We will not return to here!!!     *
   ***************************************************************
   ***************************************************************/
}
