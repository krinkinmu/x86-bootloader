#ifndef __PM_H__
#define __PM_H__

struct boot_params;

void enter_pm(unsigned long entry, struct boot_params *params);

#endif /*__PM_H__*/
