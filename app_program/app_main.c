#include <tk/tkernel.h>
#include <tm/tmonitor.h>

/* usermain関数 */
EXPORT INT usermain(void)
{
	tm_printf((UB*)"User program started\n");

	return 0;
}