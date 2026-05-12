// SPDX-FileCopyrightText: 2023 SAP SE
//
// SPDX-License-Identifier: Apache-2.0
//
// This file is part of FEDEM - https://openfedem.org
////////////////////////////////////////////////////////////////////////////////

#include "FFuLib/FFuQtComponents/FFuQtToolBar.H"
#include "FFuLib/FFuQtComponents/FFuQtToolButton.H"
#include "FFuLib/FFuAuxClasses/FFuaCmdItem.H"

//----------------------------------------------------------------------------

FFuQtToolBar::FFuQtToolBar(QWidget* parent) : QToolBar(parent)
{
  this->setWidget(this);
}
//----------------------------------------------------------------------------

void FFuQtToolBar::insertCmdItem(FFuaCmdItem* item)
{
  if (item)
    this->addWidget(new FFuQtToolButton(this,item));
  else
    this->addSeparator();
}
//----------------------------------------------------------------------------

void FFuQtToolBar::updateCmdItem(FFuaCmdItem* item, bool sensitivity)
{
  QList<QToolButton*> buttons = this->findChildren<QToolButton*>();
  for (QToolButton* obj : buttons)
    if (FFuToolButton* tButton = dynamic_cast<FFuToolButton*>(obj); tButton)
      if (!item || item == tButton->getCmdItem())
        tButton->updateButton(sensitivity);
}
//----------------------------------------------------------------------------

void FFuQtToolBar::clearBar()
{
  this->clear();
}
//----------------------------------------------------------------------------

void FFuQtToolBar::setBarLabel(const std::string& label)
{
  this->setWindowTitle(label.c_str());
}
//----------------------------------------------------------------------------

void FFuQtToolBar::setBarOrientation(int orientation)
{
  this->setOrientation((Qt::Orientation)orientation);
}
//----------------------------------------------------------------------------
