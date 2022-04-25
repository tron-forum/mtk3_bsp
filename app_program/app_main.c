#include <tk/tkernel.h>
#include <tm/tmonitor.h>

/* usermain関数 */
EXPORT INT usermain(void)
{
	tm_printf((UB*)"hello,world\n");

	return 0;
}