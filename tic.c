#include <inttypes.h>
#include <segment.h>
#include <cpu.h>
#include <stdbool.h>
#define FREQ  50
#define QUARTZ 0x1234DD 

extern void ordonnance_pre();
extern void outb(unsigned char value, unsigned short port);
extern unsigned char inb( unsigned short port);
extern void haut_droite(char *s);
extern int sprintf(char *s, const char *fmt, ...);
uint8_t compteur_IT = 0;
uint32_t  seconde =0;
uint8_t  minute =0;
uint8_t  heure =0;
char time[11];



// void tic_PIT(void){
//     outb(0x20,0x20);
//     compteur_IT++;
//     if(compteur_IT == 50){
//         if(seconde == 59){
//             seconde =0;
//             if(minute == 59){
//                 minute =0;
//                 if(heure == 23){
//                     heure =0;
//                 }else{
//                     heure++;
//                 }
//             }else{
//                 minute++;
//             }
//         }else{
//             seconde++;
//         }
//         compteur_IT =0 ;
//     }
//     sprintf(time,"%02d:%02d:%02d",heure,minute,seconde);
//     haut_droite(time);
// }
void tic_PIT(void){
    outb(0x20,0x20);
    compteur_IT++;
    if (compteur_IT == 50){
        seconde++;
        compteur_IT = 0;
    }
    ordonnance_pre();
}

unsigned getSecondes(){
    return seconde;
}


void init_traitant_IT(uint32_t num_IT, void (*traitant)(void)){
    uint32_t* case_IT= (uint32_t*) (0x1000 + 2*4*num_IT);  
    *case_IT |= (uint32_t)(traitant) & 0x0000FFFF;
    *case_IT |= (uint32_t) ((uint32_t)KERNEL_CS << 16);
    *(case_IT+1) |= (uint32_t) (0x8e00) & 0x0000FFFF;
    *(case_IT + 1) |= (uint32_t) ((uint32_t)traitant &  0xffff0000);
}

void reglage_freq(){
    outb(0x34,0x43);
    outb((QUARTZ / FREQ ) % 256 , 0x40);
    outb((QUARTZ / FREQ ) / 256, 0x40 );
}
void masque_IRQ(uint8_t num_IRQ , bool masque){
    uint8_t tab_masque;
    tab_masque=inb(0x21);
    if(masque){
        tab_masque |= 0x01 << num_IRQ;
    }else{
        tab_masque &= ~(0x01<<num_IRQ);
    }
    outb(tab_masque,0x21);
}


