/*
    Process noyau only (ex:OS)
*/

unsigned long flags;
local_irq_save(flags);
/*interrupts are now disabled*/
Section_critique();

local_irq_restore(flags);
/*interrupts are restored to their previous state*/
