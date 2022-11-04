// https://gist.github.com/zekroTJA/00317b41aa69f38090071b6c8065272b

#include <algorithm>
#include <cmath>
#include <ctime>

#include "config.h"
#include "timer.h"
using namespace std;
struct Rgb;
struct Position;
void drawMap(Position &playerPos);
bool moved(int accelX, int accelY, Position &playerPos, bool canMove);
void memoryTime(int squares[][4], Position &playerPos);
bool printPlayerPos(int line, int collumn, bool erase = false);
void showDesiredColor(int squares[][4], Position &playerPos, int desiredColor);
bool checkPlayerAnswer(int squares[][4], Position &playerPos, int desiredColor);
void showValidSquares(int squares[][4], int desiredColor);

TTGOClass *ttgo;

struct Position {
  int x;
  int y;
};
struct Rgb {
  int r;
  int g;
  int b;
};

const int SQUARE_SIZE = 30;
const int INITIAL_POS_X = 45;
const int INITIAL_POS_Y = 70;
const int SQUARES_DISTANCE = 40;
const Rgb COLORS[8] = {
    {255, 255, 255},  // white
    {59, 130, 246},   // blue 500
    {22, 163, 74},    // green 600
    {161, 98, 7},     // orange
    {236, 72, 153},   // pink 500
    {185, 28, 28},    // red 700
    {163, 230, 53},   // lime 400
    {71, 85, 105},    // grey
};

void showHalfOfColors(bool randomPositions[], int squares[][4], bool toggle) {
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      if (randomPositions[i * 4 + j] == toggle) {
        drawSquare(squares[i][j], i, j);
      }
    }
  }
}

int getRandNumber(int min, int max) {
  int range = max - min + 1;
  int num = rand() % range + min;
  return num;
}

void fillSquares(int numberOfColors, int squares[][4]) {
  if (numberOfColors > 6) numberOfColors = 6;
  int colors[6] = {1, 2, 3, 4, 5, 6};
  int colorCounter[numberOfColors] = {0};
  int colorLimit = round(16.0 / numberOfColors);
  int randNumber;
  random_shuffle(colors, &colors[6]);

  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      do {
        randNumber = getRandNumber(0, numberOfColors - 1);
      } while (colorCounter[randNumber] == colorLimit);
      colorCounter[randNumber]++;
      squares[i][j] = colors[randNumber];
    }
  }
}

bool moved(int accelX, int accelY, Position &playerPos, bool canMove) {
  if (!canMove) return false;
  if (accelY > 180) {
    printPlayerPos(playerPos.x, playerPos.y, true);
    playerPos.y++;
    return true;
  }
  if (accelY < -180) {
    printPlayerPos(playerPos.x, playerPos.y, true);
    playerPos.y--;
    return true;
  }
  if (accelX < -180) {
    printPlayerPos(playerPos.x, playerPos.y, true);
    playerPos.x--;
    return true;
  }
  if (accelX > 180) {
    printPlayerPos(playerPos.x, playerPos.y, true);
    playerPos.x++;
    return true;
  }
  return false;
}

void drawMap(Position &playerPos) {
  int x = INITIAL_POS_X;
  int y = INITIAL_POS_Y;
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      ttgo->tft->fillRect(x + SQUARES_DISTANCE * j, y, SQUARE_SIZE, SQUARE_SIZE, ttgo->tft->color565(COLORS[7].r, COLORS[7].g, COLORS[7].b));
    }
    y += SQUARES_DISTANCE;
  }
  printPlayerPos(playerPos.x, playerPos.y);
}

void drawSquare(int colorIndex, int i, int j) {
  ttgo->tft->fillRect(INITIAL_POS_X + SQUARES_DISTANCE * j, INITIAL_POS_Y + SQUARES_DISTANCE * i, SQUARE_SIZE, SQUARE_SIZE, ttgo->tft->color565(COLORS[colorIndex].r, COLORS[colorIndex].g, COLORS[colorIndex].b));
}

// retorna false se a posicao eh invalida
bool printPlayerPos(int line, int collumn, bool erase) {
  if (!(line >= 0 && line <= 3 && collumn >= 0 && collumn <= 3)) {
    return false;
  }
  int x = INITIAL_POS_X + SQUARES_DISTANCE * line + 15;
  int y = INITIAL_POS_Y + SQUARES_DISTANCE * collumn + 15;
  if (erase) {
    ttgo->tft->fillCircle(x, y, 10, ttgo->tft->color565(COLORS[7].r, COLORS[7].g, COLORS[7].b));

  } else {
    ttgo->tft->fillCircle(x, y, 10, ttgo->tft->color565(COLORS[0].r, COLORS[0].g, COLORS[0].b));
  }
  return true;
}

void memoryTime(int squares[][4], Position &playerPos) {
  bool randomPositions[16] = {true, true, true, true, true, true, true, true, false, false, false, false, false, false, false, false};
  random_shuffle(randomPositions, &randomPositions[16]);
  showHalfOfColors(randomPositions, squares, true);

  bool toggle = true;

  for (int i = 8; i >= 0; i--) {
    if (i % 2 == 0) {
      drawMap(playerPos);
      showHalfOfColors(randomPositions, squares, toggle);
      printPlayerPos(playerPos.x, playerPos.y);
      toggle = !toggle;
    }
    ttgo->tft->fillRect(0, 0, 300, 45, TFT_BLACK);
    ttgo->tft->setCursor(120, 20);
    ttgo->tft->print(i);
    delay(1000);
  }
  drawMap(playerPos);
  delay(200);
}

bool checkPlayerAnswer(int squares[][4], Position &playerPos, int desiredColor) {
  if (squares[playerPos.y][playerPos.x] != desiredColor) {
    ttgo->tft->fillRect(0, 0, 300, 45, TFT_BLACK);
    ttgo->tft->setCursor(40, 20);
    ttgo->tft->print("Você perdeu!");
    return false;
  }
  return true;
}

void showDesiredColor(int squares[][4], Position &playerPos, int desiredColor) {
  ttgo->tft->fillRect(120, 20, 25, 25, TFT_BLACK);
  ttgo->tft->fillCircle(120, 25, 10, ttgo->tft->color565(COLORS[desiredColor].r, COLORS[desiredColor].g, COLORS[desiredColor].b));
}

void showValidSquares(int squares[][4], int desiredColor) {
  ttgo->tft->fillScreen(TFT_BLACK);
  ttgo->tft->fillCircle(120, 25, 10, ttgo->tft->color565(COLORS[desiredColor].r, COLORS[desiredColor].g, COLORS[desiredColor].b));
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      if (squares[i][j] == desiredColor) {
        drawSquare(desiredColor, i, j);
      }
    }
  }
}

void refreshCounter(int &cont) {
  ttgo->tft->fillRect(80, 20, 25, 25, TFT_BLACK);
  ttgo->tft->setCursor(90, 20);
  ttgo->tft->print(cont);
  cont--;
}

void colorMemory() {
  srand(time(NULL));
  int16_t x, y;

  Position playerPos = {2, 2};
  int squares[4][4];
  int round = 1;
  int cont;
  int desiredColor;
  bool canMove = false;
  bool newRound = true;
  bool lose = false;
  ttgo->bma->begin();
  ttgo->bma->enableAccel();
  ttgo->tft->fillScreen(TFT_BLACK);

  Accel acc;

  Timer t1([&]() {
    if (cont == 0) {
      showValidSquares(squares, desiredColor);
      printPlayerPos(playerPos.x, playerPos.y);
      if (!checkPlayerAnswer(squares, playerPos, desiredColor)) {
        canMove = false;
        lose = true;
        return;
      }
      t1.stop();
      canMove = false;
      if (!lose) {
        delay(2000);
        ttgo->tft->fillScreen(TFT_BLACK);
        drawMap(playerPos);
        newRound = true;
        round++;
      }
      ttgo->tft->setTextColor(TFT_BLACK);
    }

    refreshCounter(cont);
  },
           1000);
  ttgo->tft->fillRect(0, 0, 300, 50, TFT_BLACK);
  ttgo->tft->setCursor(40, 20);
  ttgo->tft->print("por paulinha");
  ttgo->tft->setCursor(45, 50);
  ttgo->tft->print("e guilherme");
  delay(2000);
  ttgo->tft->fillScreen(TFT_BLACK);

  while (!ttgo->getTouch(x, y) && !lose) {  // se tocar no display, o codigo para de ser executado
    if (newRound) {
      ttgo->tft->setTextColor(TFT_YELLOW);
      newRound = false;

      fillSquares(2 * round, squares);
      drawMap(playerPos);

      ttgo->tft->fillRect(0, 0, 300, 50, TFT_BLACK);
      ttgo->tft->setCursor(40, 20);
      char str[10];
      sprintf(str, "Round %d", round);
      ttgo->tft->print(str);
      delay(2000);

      memoryTime(squares, playerPos);

      desiredColor = squares[getRandNumber(0, 3)][getRandNumber(0, 3)];
      showDesiredColor(squares, playerPos, desiredColor);

      ttgo->tft->setTextColor(TFT_WHITE);
      cont = 4;
      canMove = true;
      t1.start();
    }
    // if (lose) {
    //   return;
    // }
    ttgo->bma->getAccel(acc);
    delay(10);
    if (moved(acc.x, acc.y, playerPos, canMove)) {
      if (!printPlayerPos(playerPos.x, playerPos.y)) {
        ttgo->tft->fillRect(0, 0, 300, 45, TFT_BLACK);
        ttgo->tft->setCursor(40, 20);
        ttgo->tft->print("Você perdeu!");
        lose = true;
      }
      delay(500);
    }
  }

  while (ttgo->getTouch(x, y)) {
  }  // Wait for release to return to the clock
  delay(2000);
  ttgo->tft->fillScreen(TFT_BLACK);  // Clear screen
}
void setup() {
  Serial.begin(115200);

  ttgo = TTGOClass::getWatch();
  ttgo->begin();
  ttgo->openBL();
  colorMemory();
}
void loop() {
}