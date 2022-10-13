void novoApp() {
  int16_t x, y;
  while (!ttgo->getTouch(x, y)) {  // se tocar no display, o codigo para de ser executado
    // seu codigo aqui
    ttgo->tft->print("hello world");
  }

  while (ttgo->getTouch(x, y)) {
  }                                  // Wait for release to return to the clock
  ttgo->tft->fillScreen(TFT_BLACK);  // Clear screen
}