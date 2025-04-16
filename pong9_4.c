#include "LedControl.h"  //za MAX7219 led matrike

#include "Timer.h".

#include <math.h>







//za hitro spreminjanje lastnosti igre

#define HITROST 600  // nizja stevilka hitrejsa zoga

#define TLA 0

#define STROP 7

#define LEVA 0

#define DESNA 7

#define LOPAR_SIZE 2


//pins
#define TIPKA 2
#define DIN 12
#define CLK 11
#define CS 10
#define NUMBER_OF_DISPLAYS 1


// spremenljivke stanj
bool konecIgre = false; // gre na true ko je smrt
bool risanjeX = false;  // je true ko se riše X
unsigned long start; // spremenljivka za merjenje časa


LedControl lc = LedControl(DIN, CLK, CS, NUMBER_OF_DISPLAYS);
Timer t;



struct tezej {

  volatile int x, y, dx, dy;

} zoga, lopar;



volatile int izgubljenaIgra[] = { 0x00, 0x42, 0x24, 0x18, 0x18, 0x24, 0x42, 0x00 };

volatile int polje[] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };






void SetDisplayStrenght(int Svetlost) {

  lc.setIntensity(0, Svetlost);

  lc.shutdown(0, false);

  lc.clearDisplay(0);
}

void SetPozitionLopar(int pozition) {

  lopar.y = TLA;

  if (pozition == -2) random(LEVA, DESNA);

  else
    lopar.x = ((LEVA + DESNA) / 2) + int(LOPAR_SIZE / 2);  //start sredina
}

void SetPozitionZoga(int pozition) {

  zoga.y = STROP;
  if (pozition == -2) zoga.x = random(LEVA, DESNA);
  else
    zoga.x = pozition;
}

void SetAngleZoga(int pozition) {

  zoga.dx = 1;
  int vrziKovanec = random(0, 1);
  if (pozition == -2) (vrziKovanec) ? zoga.dy = -1 : zoga.dy = 1;
  else
    zoga.dy = pozition;
}







void odboj() {  //smer *-1


  if (zoga.x == LEVA || zoga.x == DESNA) zoga.dx = zoga.dx * -1;

  if (zoga.y == STROP) zoga.dy = zoga.dy * -1;

  if (zoga.y == TLA && zoga.x >= (lopar.x + 1 - LOPAR_SIZE) && zoga.x <= (lopar.x)) zoga.dy = zoga.dy * -1;
}

void preveriSmrt() {
  if (zoga.y == TLA && zoga.x < (lopar.x + 1 - LOPAR_SIZE) && zoga.x > (lopar.x)) {
    konecIgre = true;
    risanjeX = true;
    start = millis();
  }
}

//pocasi dela x
void risiX() {
    const int delay = 200;
    
    static int i = 0;

    if (millis() - start > delay) {
        lc.setRow(0, i, izgubljenaIgra[i]);
        start = millis();
        i++;

        if (i > STROP) {
            i = 0;
            risanjeX = false;
        }
    }
}


void cakajX() {
  const int delay = 200;
    if (millis() - start > delay * 70 || digitalRead(TIPKA)) { //x drzi gor se 14sekunde
        konecIgre = false;
        lc.clearDisplay(0);  //pol pa konc
    }
}

void cisti() {

  for (int vrstica = 0; vrstica <= STROP; vrstica++) polje[vrstica] = 0x00;
}

void risiPolje(int x, int y) {  //pisi 1 kjer je treba

  polje[STROP - y] = polje[STROP - y] || pow(2, (STROP - x));  //sešteje  staro in novo, na pravo vrstico
}

void risiLopar() {

  for (int i = LOPAR_SIZE; i > 0; i--) risiPolje(lopar.x + i - LOPAR_SIZE, TLA);
}

void refreshZasveti(int polje[]) {

  for (int i = 0; i <= STROP; i++) lc.setRow(0, i, polje[i]);
}





void korak() {

  zoga.x = zoga.x + zoga.dx;

  zoga.y = zoga.y + zoga.dy;
}

void premikLopar() {

  int rob1 = 0, rob2 = 7;

  if (digitalRead(2) && lopar.x > rob1) lopar.x = lopar.x - 1;

  if (digitalRead(3) && lopar.x < rob2) lopar.x = lopar.x + 1;
}








void setup() {
  randomSeed(analogRead(0));

  pinMode(TIPKA, INPUT);

  pinMode(2, INPUT);

  pinMode(3, INPUT);

  SetDisplayStrenght(8);

  SetPozitionLopar(-2);  //-2 je random

  SetPozitionZoga(-2);

  SetAngleZoga(-2);

  t.every(HITROST, korak);

  t.every(HITROST / 3, premikLopar);
}





void loop() {

    if (konecIgre) {
        if (risanjeX) {
            risiX();
        } else {
            cakajX();
        }
    } else {
        t.update();

        odboj();

        preveriSmrt();

        cisti();

        risiPolje(zoga.x, zoga.y);

        risiLopar();

        refreshZasveti(polje);
    }
}
