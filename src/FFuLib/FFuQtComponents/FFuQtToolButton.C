// SPDX-FileCopyrightText: 2023 SAP SE
//
// SPDX-License-Identifier: Apache-2.0
//
// This file is part of FEDEM - https://openfedem.org
////////////////////////////////////////////////////////////////////////////////

#include "FFuLib/FFuAuxClasses/FFuQtAuxClasses/FFuaQtPixmapCache.H"
#include "FFuLib/FFuAuxClasses/FFuaCmdItem.H"
#include "FFaLib/FFaDynCalls/FFaDynCB.H"
#include "FFuLib/FFuQtComponents/FFuQtPopUpMenu.H"
#include "FFuLib/FFuQtComponents/FFuQtToolButton.H"


FFuQtToolButton::FFuQtToolButton(QWidget* parent, FFuaCmdItem* cmd)
  : QToolButton(parent)
{
  this->setButtonWidget(this);

  FFuaCmdHeaderItem* header = dynamic_cast<FFuaCmdHeaderItem*>(cmdItem = cmd);
  if (header) // header item
  {
    const std::vector<FFuaCmdItem*>& children = header->getChildren();
    if (children.empty()) return; // a header item without children does nothing

    // set active command item
    this->setActiveCmdItem(cmd->hasIcon() ? cmd : children.front());

    // set popup menu
    FFuQtPopUpMenu* popup = new FFuQtPopUpMenu(dynamic_cast<QWidget*>(this));
    popup->setCommonCB(FFaDynCB1M(FFuQtToolButton,this,onPopUpSelected,FFuaCmdItem*));
    this->setMenu(popup);
    for (FFuaCmdItem* child : children)
      popup->insertCmdItem(child);
  }
  else // regular item
    this->setActiveCmdItem(cmd);

  if (cmd->getMenuButtonPopupMode())
    this->setPopupMode(QToolButton::MenuButtonPopup);
  else
    this->setPopupMode(QToolButton::InstantPopup);

  QObject::connect(this, SIGNAL(pressed()),     this, SLOT(arm()));
  QObject::connect(this, SIGNAL(released()),    this, SLOT(unarm()));
  QObject::connect(this, SIGNAL(clicked()),     this, SLOT(activate()));
  QObject::connect(this, SIGNAL(toggled(bool)), this, SLOT(toggle(bool)));
}


void FFuQtToolButton::updateButton(bool sensitivity)
{
  FFuaCmdHeaderItem* header = dynamic_cast<FFuaCmdHeaderItem*>(cmdItem);
  if (header)
    if (FFuQtPopUpMenu* popup = dynamic_cast<FFuQtPopUpMenu*>(this->menu()); popup)
      for (FFuaCmdItem* child : header->getChildren())
        if (child)
          popup->updateCmdItem(child,sensitivity);

  if (!activeCmdItem)
    return;
  else if (sensitivity)
    this->setSensitivity(activeCmdItem->getSensitivity());
  else if (activeCmdItem->getToggleAble())
    this->setToggle(activeCmdItem->getToggled());
}


void FFuQtToolButton::setActiveCmdItem(FFuaCmdItem* cmd)
{
  activeCmdItem = cmd;

  if (cmd->getBigIcon()) {
    QIcon ic(FFuaQtPixmapCache::getPixmap(cmd->getBigIcon()));
    if (cmd->getSmallIcon())
      ic.addPixmap(FFuaQtPixmapCache::getPixmap(cmd->getSmallIcon()));
    this->setIcon(ic);
  }
  else if (cmd->getSmallIcon()) {
    QIcon ic(FFuaQtPixmapCache::getPixmap(cmd->getSmallIcon()));
    if (cmd->getBigIcon())
      ic.addPixmap(FFuaQtPixmapCache::getPixmap(cmd->getBigIcon()));
    this->setIcon(ic);
  }
  else
    this->setIcon(QIcon());

  this->setText(cmd->getText().c_str());
  this->FFuQtButton::setToolTip(cmd->getToolTip().c_str());
  this->setSensitivity(cmd->getSensitivity());
  if (this->getToggleAble() && !cmd->getToggleAble())
    this->setToggle(false);
  this->setToggleAble(cmd->getToggleAble());
  if (cmd->getToggleAble())
    this->setToggle(cmd->getToggled());
}


void FFuQtToolButton::onPopUpSelected(FFuaCmdItem* cmd)
{
  if (this->popupMode() != QToolButton::InstantPopup)
    this->setActiveCmdItem(cmd);
}


void FFuQtToolButton::setToggleAble(bool able)
{
  this->setCheckable(able);
}


void FFuQtToolButton::setToggle(bool toggle)
{
  this->blockLibSignals(true);
  if (!this->isCheckable())
    this->setCheckable(true);
  this->setChecked(toggle);
  this->blockLibSignals(false);
}


void FFuQtToolButton::activate()
{
  if (!activeCmdItem || activeCmdItem->getToggleAble())
    return;

  this->callActivateCB();

  // activeCmdItem is not toggle item
  activeCmdItem->invokeActivatedCB();
}


void FFuQtToolButton::toggle(bool value)
{
  if (!activeCmdItem || !activeCmdItem->getToggleAble())
    return;

  if (activeCmdItem->getToggleBehaveAsRadio() && !value)
    this->setToggle(true);
  else
  {
    this->callToggleCB(value);
    // activeCmdItem is toggle item
    activeCmdItem->setToggled(value);
    activeCmdItem->invokeToggledCB(value);
  }
}
