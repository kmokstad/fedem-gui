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
  if (dynamic_cast<FFuaCmdSeparatorItem*>(item))
    this->addSeparator();
  else
    this->addWidget(new FFuQtToolButton(this,item));
}
//----------------------------------------------------------------------------

void FFuQtToolBar::updateCmdItem(FFuaCmdItem* item, bool sensitivity)
{
  if (dynamic_cast<FFuaCmdSeparatorItem*>(item)) return;
  
  QList<QToolButton*> buttons = this->findChildren<QToolButton*>();
  FFuToolButton* toolButton = NULL;
  for (QToolButton* obj : buttons)
    if ((toolButton = dynamic_cast<FFuToolButton*>(obj)))
      if (item == NULL || item == toolButton->getCmdItem())
        toolButton->updateButton(sensitivity);
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
