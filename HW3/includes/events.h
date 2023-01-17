#ifndef EVENTS_H
#define EVENTS_H

#include <QKeyEvent>
#include <QMouseEvent>

struct KeyStatus {
  int key = -1;
  QKeyEvent::Type action = QKeyEvent::Type::KeyPress;

  bool held = false;
  bool down = false;
  bool released = true;

  KeyStatus() {
    held = false;
    down = false;
    released = false;
  }

  KeyStatus(int iKey, QKeyEvent::Type iAction) {
    action = iAction;
    key = iKey;
    if (action == QKeyEvent::Type::KeyPress) {
      down = true;
      released = false;
      held = true;
    }
    if (action == QKeyEvent::Type::KeyRelease) {
      down = false;
      released = true;
      held = false;
    }
  }
};

struct MouseButtonStatus {
  Qt::MouseButton button = Qt::MouseButton::NoButton;
  QMouseEvent::Type action = QMouseEvent::Type::None;

  bool up = false;
  bool down = false;
  bool held = false;

  MouseButtonStatus() {
    up = false;
    down = false;
    held = false;
  }
  MouseButtonStatus(Qt::MouseButton iButton, QMouseEvent::Type iAction)
      : button(iButton), action(iAction) {
    if (action == QMouseEvent::Type::MouseButtonPress) {
      down = true;
      up = false;
      held = false;
    }
    if (action == QMouseEvent::Type::MouseButtonRelease) {
      down = false;
      up = true;
      held = false;
      std::cout << "release" << std::endl;
    }
  }
};

#endif  // EVENTS_H
