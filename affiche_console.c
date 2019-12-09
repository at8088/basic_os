#include <affiche_console.h>

extern void console_putbytes(const char *s, int len);

uint16_t *ptr_mem(uint32_t lig, uint32_t col){
    return (uint16_t*)(0xb8000 + 2 * (lig * 80 + col)) ;
}
void ecrit_car(uint32_t lig, uint32_t col, char c, char couleur){
    if(pos_col_cur < 79){
        uint16_t * p = ptr_mem(lig,col);
        *p =(uint16_t) c;
        *p |= ((uint16_t)couleur)<<8;
        pos_col_cur++;
    }else{
        pos_col_cur=0;
        uint16_t * p = ptr_mem(lig+1,pos_col_cur);
        *p =(uint16_t) c;
        *p |= ((uint16_t)couleur)<<8;
        pos_col_cur++;    
        pos_lig_cur++;
    }
    place_curseur(pos_lig_cur,pos_col_cur);    

}
void efface_ecran(void){
    char i,j;
    for(i = 0 ; i<25 ; i++){
        for(j=0; j<80;j++){
            ecrit_car(i,j,' ',15); /*15 == blanc */
        }
    } 
    place_curseur(0,0);
}
void place_curseur(uint32_t lig, uint32_t col){
    uint16_t pos = (uint16_t)(col + lig * 80);

    /*envoi partie basse */
    outb(0x0f,0x03d4);
    uint8_t p_basse = (uint8_t) pos ;
    outb(p_basse,0x03d5);

    /*envoi partie haute */
    outb(0x0e,0x03d4);
    uint8_t p_haute = (uint8_t) (pos>>8);
    outb(p_haute,0x03d5);
    pos_col_cur = col;
    pos_lig_cur = lig;
}

void traite_car(char c){
    if (pos_lig_cur > 23){
        defilement();
        pos_lig_cur = 23;
    }
    if (!isspace(c)){
        ecrit_car(pos_lig_cur,pos_col_cur,c,15);
    }
    else{
        switch(c){
            case ' ':
                if(pos_col_cur == 79){
                    pos_col_cur = 0;
                    pos_lig_cur++;
                    place_curseur(pos_lig_cur, pos_col_cur);
                    break;
                }else{
                    pos_col_cur++;
                    place_curseur(pos_lig_cur, pos_col_cur);
                    break;
                }
            case '\b':
                if(pos_col_cur!=0){
                    pos_col_cur--;
                    place_curseur(pos_lig_cur, pos_col_cur);
                    break;
                }else break;
            case '\t':
                if(pos_col_cur> 72){
                    pos_lig_cur++;
                    pos_col_cur=0;
                    place_curseur(pos_lig_cur,pos_col_cur);
                    break;
                }else{
                    pos_col_cur+=8;
                    place_curseur(pos_lig_cur,pos_col_cur);
                    break;
                }

            case '\n':
                pos_col_cur=0;
                pos_lig_cur++;
                place_curseur(pos_lig_cur, pos_col_cur);
                break;
            case '\f':
                efface_ecran();
                break;
            case '\r':
                place_curseur(pos_lig_cur, 0);
                pos_col_cur=0;
                break;
            default : break;
        }
    }
}

void defilement(void){
    memmove((uint16_t*)0xb8000,(uint16_t*)0xb80a0,sizeof(uint16_t)*25*80);
}


// séance 2
void haut_droite(char *s){
        uint16_t * p ;
    for(int i=0 ; i < strlen(s); i++){
        p=ptr_mem(0,79-strlen(s)+i);
        *p =(uint16_t) s[i];
        *p |= ((uint16_t)15)<<8;
    } 
}
