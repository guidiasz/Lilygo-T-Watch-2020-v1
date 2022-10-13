#include <stdlib.h>
#include <time.h>

void highlight(char side) {
  if (side == 'u') {
    ttgo->tft->fillCircle(120, 90, 15, TFT_WHITE);
    delay(500);
    ttgo->tft->fillCircle(120, 90, 15, TFT_GREEN);

  } else if (side == 'r') {
    ttgo->tft->fillCircle(150, 120, 15, TFT_WHITE);
    delay(500);
    ttgo->tft->fillCircle(150, 120, 15, TFT_YELLOW);

  } else if (side == 'l') {
    ttgo->tft->fillCircle(90, 120, 15, TFT_WHITE);
    delay(500);
    ttgo->tft->fillCircle(90, 120, 15, TFT_RED);

  } else if (side == 'd') {
    ttgo->tft->fillCircle(120, 150, 15, TFT_WHITE);
    delay(500);
    ttgo->tft->fillCircle(120, 150, 15, TFT_BLUE);
  }
}

void fillSequence(char sequence[], int size) {
  char sides[4] = {'u', 'd', 'l', 'r'};
  for (int i = 0; i < size; i++) {
    sequence[i] = sides[rand() % 4];
    while (sequence[i - 1] == sequence[i]) sequence[i] = sides[rand() % 4];
  }
}

void appGenius() {
  srand(time(NULL));
  ttgo->bma->begin();
  ttgo->bma->enableAccel();
  ttgo->tft->fillScreen(TFT_BLACK);
  int16_t x, y;
  int16_t xpos, ypos;
  int16_t prevxpos = 0;
  int16_t prevypos = 0;
  char sequence[50];
  char myMovements[50];
  int cont = 0;
  int round = 1;
  bool moved = false;
  Accel acc;

  ttgo->tft->fillCircle(120, 90, 15, TFT_GREEN);
  ttgo->tft->fillCircle(90, 120, 15, TFT_RED);
  ttgo->tft->fillCircle(150, 120, 15, TFT_YELLOW);
  ttgo->tft->fillCircle(120, 150, 15, TFT_BLUE);
  fillSequence(sequence, 50);

  while (!ttgo->getTouch(x, y)) {  // Wait for touch to exit

    ttgo->bma->getAccel(acc);
    xpos = acc.x;
    ypos = acc.y;
    // ttgo->tft->fillCircle(xpos / 10 + 119, ypos / 10 + 119, 10, TFT_RED);  // draw dot
    // delay(100);
    // ttgo->tft->fillCircle(xpos / 10 + 119, ypos / 10 + 119, 10, TFT_BLACK);  // erase previous dot

    // ttgo->tft->fillRect(80, 160, 200, 200, TFT_BLACK);  // limpa a tela para imprimir novos valores
    // ttgo->tft->setCursor(80, 160);                      // define a posicao na tela em que o texto vai aparecer
    // ttgo->tft->print(ypos);
    if (round - 1 == cont) {
      for (int i = 0; i < round; i++) {
        delay(200);
        highlight(sequence[i]);
      }
      cont = 0;
      round++;
    }

    if (ypos > 180) {
      myMovements[cont] = 'd';
      moved = true;
    } else if (ypos < -180) {
      myMovements[cont] = 'u';
      moved = true;
    } else if (xpos < -180) {
      myMovements[cont] = 'l';
      moved = true;
    } else if (xpos > 180) {
      myMovements[cont] = 'r';
      moved = true;
    }
    if (moved) {
      moved = false;
      highlight(myMovements[cont]);
      if (myMovements[cont] != sequence[cont]) {
        ttgo->tft->setCursor(80, 190);
        ttgo->tft->print("Você perdeu!");
        delay(4000);
        break;
      }
      cont++;
    }
    // if (ypos > prevypos) {
    //   highlight('d');
    // } else if (ypos < prevypos) {
    //   highlight('u');
    // }
    // ttgo->tft->setCursor(80, 190);
    // if (xpos > prevxpos) {
    //   ttgo->tft->print("direita");
    // } else if (xpos < prevxpos) {
    //   ttgo->tft->print("esquerda");
    // }
  }

  while (ttgo->getTouch(x, y)) {
  }  // Wait for release to return to the clock

  ttgo->tft->fillScreen(TFT_BLACK);  // Clear screen
}
