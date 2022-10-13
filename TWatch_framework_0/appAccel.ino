// Show the accelerometer working

void appAccel() {
  ttgo->bma->begin();
  ttgo->bma->enableAccel();
  ttgo->tft->fillScreen(TFT_BLACK);
  int16_t x, y;
  int16_t xpos, ypos;
  int16_t prevxpos = 0;
  int16_t prevypos = 0;

  Accel acc;

  while (!ttgo->getTouch(x, y)) {  // Wait for touch to exit

    ttgo->bma->getAccel(acc);
    xpos = acc.x;
    ypos = acc.y;
    ttgo->tft->fillCircle(xpos / 10 + 119, ypos / 10 + 119, 10, TFT_RED);  // draw dot
    delay(100);
    ttgo->tft->fillCircle(xpos / 10 + 119, ypos / 10 + 119, 10, TFT_BLACK);  // erase previous dot

    ttgo->tft->fillRect(80, 160, 200, 200, TFT_BLACK);  // limpa a tela para imprimir novos valores
    ttgo->tft->setCursor(80, 160);                      // define a posicao na tela em que o texto vai aparecer
    if (ypos > prevypos) {
      ttgo->tft->print("pra baixo");  // imprime texto na tela
    } else if (ypos < prevypos) {
      ttgo->tft->print("pra cima");
    }
    ttgo->tft->setCursor(80, 190);
    if (xpos > prevxpos) {
      ttgo->tft->print("direita");
    } else if (xpos < prevxpos) {
      ttgo->tft->print("esquerda");
    }
  }

  while (ttgo->getTouch(x, y)) {
  }  // Wait for release to return to the clock

  ttgo->tft->fillScreen(TFT_BLACK);  // Clear screen
}
