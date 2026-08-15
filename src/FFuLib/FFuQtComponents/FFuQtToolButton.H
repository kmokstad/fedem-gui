// SPDX-FileCopyrightText: 2023 SAP SE
//
// SPDX-License-Identifier: Apache-2.0
//
// This file is part of FEDEM - https://openfedem.org
////////////////////////////////////////////////////////////////////////////////

#ifndef FFU_QT_TOOL_BUTTON_H
#define FFU_QT_TOOL_BUTTON_H

#include <QToolButton>

#include "FFuLib/FFuQtComponents/FFuQtButton.H"
#include "FFuLib/FFuToolButton.H"


class FFuQtToolButton : public QToolButton, public FFuQtButton,
                        virtual public FFuToolButton
{
  Q_OBJECT

public:
  // Create a toolbutton based on a regular cmd item or a cmd header item.
  // The latter creates a toolbutton popupmenu which is reparented to this.
  // One can only have one level of popups in the toolbutton
  // if the activeCmdItem management should work correctly.
  // The cmdItem (regular or header) is book-kept externally and must not
  // be deleted as long as its toolbutton exists.
  FFuQtToolButton(QWidget* parent, FFuaCmdItem* cmdItem);

  virtual void updateButton(bool sensitivity);

  virtual void setToggleAble(bool able);

private:
  void setActiveCmdItem(FFuaCmdItem* cmdItem);
  void onPopUpSelected(FFuaCmdItem* cmdItem);
  void setToggle(bool toggle);

private slots:
  void arm() { this->callArmCB(); }
  void unarm() { this->callUnarmCB(); }
  void activate();
  void toggle(bool value);
};

#endif
