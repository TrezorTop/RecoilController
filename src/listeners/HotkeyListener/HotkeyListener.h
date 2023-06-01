#pragma once

#include <windows.h>

#include <iostream>
#include <vector>

class HotkeyListener {
 public:
  HotkeyListener();
  ~HotkeyListener();

  void RunMessageLoop();

 private:
  struct HotkeyInfo {
    int id;
    UINT modifier;
    UINT key;
  };

  MSG msg = {0};
  std::vector<HotkeyInfo> hotkeys;

  void AddHotkey(int id, UINT modifier, UINT key);
  bool RegisterHotkeys();
  void UnregisterHotkeys();
  void HandleHotkey(int id);
};
