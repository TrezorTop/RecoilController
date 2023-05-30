#include "MouseController.h"
void MouseController::MoveMouse(int deltaX, int deltaY) {
  INPUT input = {0};
  input.type = INPUT_MOUSE;
  input.mi.dwFlags = MOUSEEVENTF_MOVE;
  input.mi.dx = deltaX;
  input.mi.dy = deltaY;

  SendInput(1, &input, sizeof(INPUT));
}