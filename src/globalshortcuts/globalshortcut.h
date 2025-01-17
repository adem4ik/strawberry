/*
 * Strawberry Music Player
 * Copyright 2018-2021, Jonas Kvinge <jonas@jkvinge.net>
 *
 * Strawberry is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Strawberry is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Strawberry.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#ifndef GLOBALSHORTCUT_H
#define GLOBALSHORTCUT_H

#include "config.h"

#include <QtGlobal>
#include <QObject>
#include <QAbstractNativeEventFilter>
#include <QKeySequence>
#include <QPair>
#include <QVector>
#include <QHash>
#include <QByteArray>
#include <QString>

class GlobalShortcutsBackend;

class GlobalShortcut : public QObject, QAbstractNativeEventFilter {
  Q_OBJECT

 public:
  explicit GlobalShortcut(QObject *parent = nullptr);
  explicit GlobalShortcut(const QKeySequence &shortcut, GlobalShortcutsBackend *backend, QObject *parent = nullptr);
  ~GlobalShortcut() override;

  GlobalShortcutsBackend *backend() const { return backend_; }
  QKeySequence shortcut() const { return shortcut_; }

  bool setShortcut(const QKeySequence &shortcut);
  bool unsetShortcut();

 signals:
  void activated();

 private:

  static void activateShortcut(quint32 native_key, quint32 native_mods);

  static int nativeModifiers(Qt::KeyboardModifiers qt_mods);
  static int nativeKeycode(Qt::Key qt_keycode);

  static bool registerShortcut(int native_key, int native_mods);
  static bool unregisterShortcut(int native_key, int native_mods);

#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
  bool nativeEventFilter(const QByteArray &eventtype, void *message, qintptr *result) override;
#else
  bool nativeEventFilter(const QByteArray &eventtype, void *message, long *result) override;
#endif

  static GlobalShortcut *initialized_;
  static QHash<QPair<quint32, quint32>, GlobalShortcut*> internal_shortcuts_;
  static const QVector<quint32> mask_modifiers_;

  GlobalShortcutsBackend *backend_;
  QKeySequence shortcut_;
  Qt::Key qt_key_;
  Qt::KeyboardModifiers qt_mods_;
  int native_key_;
  int native_mods_;

};

#endif  // GLOBALSHORTCUT_H
