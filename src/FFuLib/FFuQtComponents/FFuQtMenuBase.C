// SPDX-FileCopyrightText: 2023 SAP SE
//
// SPDX-License-Identifier: Apache-2.0
//
// This file is part of FEDEM - https://openfedem.org
////////////////////////////////////////////////////////////////////////////////

#include "FFuLib/FFuQtComponents/FFuQtMenuBase.H"
#include "FFuLib/FFuQtComponents/FFuQtPopUpMenu.H"
#include "FFuLib/FFuAuxClasses/FFuaCmdItem.H"


bool FFuQtMenuBase::insertCmdItem(FFuaCmdItem* item)
{
  int id = 0;
  FFuaCmdHeaderItem* header = dynamic_cast<FFuaCmdHeaderItem*>(item);
  if (header) {
    FFuPopUpMenu* popup = new FFuQtPopUpMenu(dynamic_cast<QWidget*>(this));
    if ((id = this->basicNewItem(item,popup)) > 0)
    {
      this->popups[id] = popup;
      for (FFuaCmdItem* child : header->getChildren())
        popup->insertCmdItem(child);
    }
  }
  else if (item)
    id = this->basicNewItem(item);
  else
    this->insertSeparator();

  if (id > 0)
    this->cmdItems[id] = item;

  return id >= 0;
}


void FFuQtMenuBase::updateCmdItem(FFuaCmdItem* item, bool sensitivity)
{
  FFuaCmdHeaderItem* header = dynamic_cast<FFuaCmdHeaderItem*>(item);
  if (header) {
    if (FFuPopUpMenu* popup = this->findPopup(this->findCmdItem(item)); popup)
      for (FFuaCmdItem* child : header->getChildren())
        if (child)
          popup->updateCmdItem(child,sensitivity);
  }
  else if (sensitivity)
    this->setItemSensitivity(this->findCmdItem(item), item->getSensitivity());
  else if (item->getToggleAble())
    this->setItemToggled(this->findCmdItem(item), item->getToggled());
}
