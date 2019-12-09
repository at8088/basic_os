#include <string.h>
#include <cpu.h>
#include <stdio.h>
#include "console.h"
#include <inttypes.h>
#include "affiche_console.h"

#define NAME_MAX_SIZE 30
#define STACK_SIZE 512
#define PROC_TABLE_SIZE 4

extern void ctx_sw(int*,int*);
extern unsigned getSecondes(); 
void idle(void);
void proc1(void);
void proc2(void);
void proc3(void);
void   ordonnance_pre();
void fin_processus();
int mon_pid();
char* mon_nom();


typedef enum STATE {
    ELU,
    ACTIVABLE,
    ENDORMI,
    MORT
} STATE;


typedef struct _proc {
    int pid;
    char Name[NAME_MAX_SIZE];
    STATE state;
    int Reg_Table [5];                  
    int Stack[STACK_SIZE];
    int Wake_Up_Time;
} PROC;

PROC Proc_Table[PROC_TABLE_SIZE];



void init_procs(){
    /*init idle*/
    Proc_Table[0].pid = 0;
    strncpy(Proc_Table[0].Name,"idle",NAME_MAX_SIZE);
    Proc_Table[0].state = ELU;
    /*init proc1*/
    Proc_Table[1].pid = 1;
    strncpy(Proc_Table[1].Name,"proc1",NAME_MAX_SIZE);
    Proc_Table[1].state = ACTIVABLE;
    Proc_Table[1].Reg_Table[1] = (int)(Proc_Table[1].Stack + STACK_SIZE-2);
    Proc_Table[1].Stack[STACK_SIZE-2] = (int)(proc1);
    Proc_Table[1].Stack[STACK_SIZE-1] = (int)(fin_processus);
    /*init proc2*/
    Proc_Table[2].pid = 2;
    strncpy(Proc_Table[2].Name,"proc2",NAME_MAX_SIZE);
    Proc_Table[2].state = ACTIVABLE;
    Proc_Table[2].Reg_Table[1] = (int)(Proc_Table[2].Stack + STACK_SIZE-2);
    Proc_Table[2].Stack[STACK_SIZE-2] = (int)(proc2);
    Proc_Table[2].Stack[STACK_SIZE-1] = (int)(fin_processus);
    /*init proc3*/
    Proc_Table[3].pid = 3;
    strncpy(Proc_Table[3].Name,"proc3",NAME_MAX_SIZE);
    Proc_Table[3].state = ACTIVABLE;
    Proc_Table[3].Reg_Table[1] = (int)(Proc_Table[3].Stack + STACK_SIZE-2);
    Proc_Table[3].Stack[STACK_SIZE-2] = (int)(proc3);
    Proc_Table[3].Stack[STACK_SIZE-1] = (int)(fin_processus);
}
PROC *actif ;

void init_actif_proc(){
    actif = &Proc_Table[0];
}
int getActifPid(){
    return actif->pid;
}

void dors(uint32_t nbr_secs){
    actif->state = ENDORMI;
    actif->Wake_Up_Time = getSecondes() + nbr_secs;
    ordonnance_pre();
}
void ordonnance_pre(){
    int pid_courant = actif->pid;
    actif = &(Proc_Table[(pid_courant+1)%PROC_TABLE_SIZE]);
    while ( (Proc_Table[actif->pid].state == ENDORMI && Proc_Table[actif->pid].Wake_Up_Time > getSecondes())
             || Proc_Table[actif->pid].state == MORT){
        actif = &(Proc_Table[(actif->pid +1)%PROC_TABLE_SIZE]);
    }
    actif->state = ELU ;
    ctx_sw(Proc_Table[pid_courant].Reg_Table,Proc_Table[actif->pid].Reg_Table);
}

void fin_processus(){
    actif->state = MORT;
    ordonnance_pre();
}

int mon_pid(){
    return actif->pid;
}

char* mon_nom(){
    return actif->Name;
}
void idle(void){
    while(1){
        sti();
        hlt();
        cli();
    }
}
void proc1(void){
    for (int i = 0 ; i < 3 ; i++){
        printf("[temps = %u ] processus = %s pid = %d \n", 
            getSecondes(), mon_nom(),mon_pid());
        dors(3);    
    }
    // fin_processus();
}

void proc2(void){
    for (int i = 0 ; i < 3 ; i++){
        printf("[temps = %u ] processus = %s pid = %d \n", 
            getSecondes(), mon_nom(),mon_pid());
        dors(4);
    }
    // fin_processus();
}

void proc3(void){
    for (int i = 0 ; i < 3 ; i++){
        printf("[temps = %u ] processus = %s pid = %d \n", 
            getSecondes(), mon_nom(),mon_pid());
        dors(7);
    }
    // fin_processus();
}