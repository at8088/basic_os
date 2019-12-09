#include <cpu.h>
#include <inttypes.h>
#include "console.h"
#include <stdbool.h>
#include "affiche_console.h"
#include <stdio.h>

extern void init_actif_proc();
extern void traitant_IT_32();
extern void init_traitant_IT(uint32_t num_IT, void (*traitant)(void));
extern void reglage_freq();
extern void masque_IRQ(uint8_t num_IRQ , bool masque);
extern void idle();
extern void init_procs();

void kernel_start(void)
{
    efface_ecran();
    // init interruptions
    init_traitant_IT(32,traitant_IT_32);
    reglage_freq();
    masque_IRQ(0,0);
    //init proc
    init_procs();
    init_actif_proc();
    idle();
    while (1) {
        hlt();
    }
}



