#include "HotkeyListener.h"

HotkeyListener::HotkeyListener() {
  // Register the hotkeys
  if (this->RegisterHotkeys()) {
    std::cout << "Hotkeys registered." << '\n';
  } else {
    std::cout << "Failed to register hotkeys." << '\n';
  }
}

HotkeyListener::~HotkeyListener() {
  // Unregister the hotkey when the object is destroyed
  this->UnregisterHotkeys();
}

void HotkeyListener::RunMessageLoop() {
  // Message loop to receive hotkey events
  MSG msg = {0};
  while (GetMessage(&msg, NULL, 0, 0) != 0) {
    if (msg.message == WM_HOTKEY) {
      int id = LOWORD(msg.lParam);
      this->HandleHotkey(id);
    }
    TranslateMessage(&msg);
    DispatchMessage(&msg);
  }
}

void HotkeyListener::AddHotkey(int id, UINT modifier, UINT key) {
  HotkeyInfo hotkey;
  hotkey.id = id;
  hotkey.modifier = modifier;
  hotkey.key = key;
  this->hotkeys.push_back(hotkey);
}

bool HotkeyListener::RegisterHotkeys() {
  // Add your hotkey combinations here
  this->AddHotkey(1, MOD_CONTROL, VK_F1);
  this->AddHotkey(2, MOD_CONTROL, VK_F2);
  this->AddHotkey(3, MOD_CONTROL, VK_F3);

  for (const auto& hotkey : hotkeys) {
    if (!RegisterHotKey(NULL, hotkey.id, hotkey.modifier, hotkey.key)) {
      return false;
    }
  }

  return true;
}

void HotkeyListener::UnregisterHotkeys() {
  for (const auto& hotkey : hotkeys) {
    UnregisterHotKey(NULL, hotkey.id);
  }
}

void HotkeyListener::HandleHotkey(int id) {
  std::cout << "Hotkey pressed: ID " << id << '\n';
}