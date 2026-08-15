// SPDX-FileCopyrightText: 2023 SAP SE
//
// SPDX-License-Identifier: Apache-2.0
//
// This file is part of FEDEM - https://openfedem.org
////////////////////////////////////////////////////////////////////////////////

#include "FFuLib/FFuAuxClasses/FFuaCmdItem.H"

#include <map>
#include <iostream>


namespace
{
  using CommandMap = std::map<std::string,FFuaCmdItem*>;

  CommandMap* cmdItemMap = NULL;
  bool weAreLoggingCmds = false;
}

//--------------------------------------------------------------------

void FFuaCmdItem::enableCmdLogging(bool doEnable)
{
  weAreLoggingCmds = doEnable;
}
//--------------------------------------------------------------------

void FFuaCmdItem::invokeActivatedCB()
{
  if (weAreLoggingCmds)
    std::cout << this->getCmdItemId() << std::endl;

  activatedCB.invoke();
}
//--------------------------------------------------------------------

void FFuaCmdItem::invokeToggledCB(bool toggle)
{
  if (weAreLoggingCmds)
    std::cout << this->getCmdItemId() << std::endl;

  toggledCB.invoke(toggle);
}
//--------------------------------------------------------------------

void FFuaCmdItem::init()
{
  if (!cmdItemMap)
    cmdItemMap = new CommandMap();
}
//----------------------------------------------------------------------------

FFuaCmdItem* FFuaCmdItem::getCmdItem(const std::string& itemId)
{
  if (itemId.empty() || !cmdItemMap)
    return NULL;

  CommandMap::const_iterator it = cmdItemMap->find(itemId);
  if (it != cmdItemMap->end())
    return it->second;

  std::cerr <<" *** FFuaCmdItem: \""<< itemId <<"\" does not exist"
            << std::endl;
  return NULL;
}
//----------------------------------------------------------------------------

FFuaCmdItem::FFuaCmdItem(const std::string& itemId)
{
  this->initVars();

  if (itemId.empty() || !cmdItemMap) return;

  cmdItemId = itemId;
  if (cmdItemMap->insert({itemId,this}).second)
    return; // unique command id

  std::cerr <<" *** FFuaCmdItem: \""<< itemId <<"\" already exists"<< std::endl;
  *(char*)0 = 'd'; // To make core
}
//----------------------------------------------------------------------------

FFuaCmdItem::~FFuaCmdItem()
{
  if (!cmdItemId.empty() && cmdItemMap) // subject to static management
    cmdItemMap->erase(cmdItemId);
}
//----------------------------------------------------------------------------

void FFuaCmdItem::initVars()
{
  bigIcon = smallIcon = pixmap = NULL;
  accelKey = 0;
  toggleAble = toggled = menuButtonPopupMode = false;
}
//----------------------------------------------------------------------------

bool FFuaCmdItem::getSensitivity()
{
  bool sensitivity = true;
  getSensitivityCB.invoke(sensitivity);
  return sensitivity;
}
//----------------------------------------------------------------------------

bool FFuaCmdItem::getToggled()
{
  bool toggle = toggled;
  getToggledCB.invoke(toggle);
  return toggle;
}
//----------------------------------------------------------------------------
