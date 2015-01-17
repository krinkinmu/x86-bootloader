#include "boot.h"
#include "string.h"

void initregs(struct biosregs *regs)
{
	memset(regs, 0, sizeof(*regs));
	regs->eflags |= X86_CF_FLAG;
	regs->ds = get_ds();
	regs->es = get_es();
}
