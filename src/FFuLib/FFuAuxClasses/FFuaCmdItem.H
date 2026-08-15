// SPDX-FileCopyrightText: 2023 SAP SE
//
// SPDX-License-Identifier: Apache-2.0
//
// This file is part of FEDEM - https://openfedem.org
////////////////////////////////////////////////////////////////////////////////

#ifndef FFUA_CMD_ITEM_H
#define FFUA_CMD_ITEM_H

#include <string>
#include <vector>

#include "FFaLib/FFaDynCalls/FFaDynCB.H"
#include "FFaLib/FFaPatterns/FFaInitialisation.H"


class FFuaCmdItem
{
public:
  // A FFuaCmdItem subject to static management must have a cmdItemId string
  // otherwise it escapes the static management
  explicit FFuaCmdItem(const std::string& cmdItemId);
  FFuaCmdItem() { this->initVars(); }

  // A FFuaCmdItem must not be deleted as long as it is in use
  virtual ~FFuaCmdItem();

  static FFuaCmdItem* getCmdItem(const std::string& cmdItemId);
  static void enableCmdLogging(bool doEnable);

  const std::string& getCmdItemId() const { return cmdItemId; }

  // callbacks
  // activation/toggle
  // if toggleable you get a ToggledCB otherwise you get an ActivatedCB
  void setActivatedCB(const FFaDynCB0& dynCB) {
    toggleAble = false;
    activatedCB = dynCB;
  }
  void invokeActivatedCB();

  void setToggledCB(const FFaDynCB1<bool>& dynCB, bool behaveAsRadio = false) {
    toggleAble = behaveAsRadio ? 2 : 1;
    toggledCB = dynCB;
  }
  void invokeToggledCB(bool toggled);

  void setGetSensitivityCB(const FFaDynCB1<bool&>& cb) { getSensitivityCB = cb; }
  // If you dont set this callback you have to initiate the toggle status (setToggled)
  // You should set this cb if possible, ie if the db stores the toggle statuses
  void setGetToggledCB(const FFaDynCB1<bool&>& cb) { getToggledCB = cb; }

  // item attributes
  // icons are used in front of popupmenu text entries and on toolbuttons
  // small should be < 23 pix
  // popupmenues do always require a small icon, if not it a bigone is scaled
  void setBigIcon(const char** pixMap) { bigIcon = pixMap; }
  const char** getBigIcon() const { return bigIcon; }

  void setSmallIcon(const char** pixMap) { smallIcon = pixMap; }
  const char** getSmallIcon() const { return smallIcon; }

  bool hasIcon() const { return bigIcon || smallIcon; }

  // pixmaps are used as a substitute for the text on popup menu entries
  void setPixmap(const char** pixMap) { pixmap = pixMap; }
  const char** getPixmap() const { return pixmap; }

  void setText(const std::string& txt) { text = txt; }
  const std::string& getText() const { return text; }

  void setToolTip(const std::string& tooltip) { toolTip = tooltip; }
  const std::string& getToolTip() const { return toolTip; }

  // see FFuaKeyCode
  void setAccelKey(int accelkey) { accelKey = accelkey; }
  int getAccelKey() const { return accelKey; }

  bool getSensitivity();

  bool getToggleAble() const { return toggleAble; }
  bool getToggleBehaveAsRadio() const { return toggleAble == 2; }

  void setToggled(bool toggle) { toggled = toggle; }
  bool getToggled();

  void setMenuButtonPopupMode(bool val) { menuButtonPopupMode = val; }
  bool getMenuButtonPopupMode() const { return menuButtonPopupMode; }

  // FFaInitializer init - used for the cmdItemMap;
  static void init();

protected:
  void initVars();

private:
  std::string cmdItemId;

  FFaDynCB0        activatedCB;
  FFaDynCB1<bool>  toggledCB;
  FFaDynCB1<bool&> getSensitivityCB;
  FFaDynCB1<bool&> getToggledCB;

  const char** bigIcon;
  const char** smallIcon;
  const char** pixmap;
  std::string text;
  std::string toolTip;
  int  accelKey;
  char toggleAble;
  bool toggled;
  bool menuButtonPopupMode;
};
/////////////////////////////////////////////////////////////////////////////

class FFuaCmdHeaderItem : public FFuaCmdItem
{
public:
  // A FFuaCmdHeaderItem is NOT book-kept (in static management)
  explicit FFuaCmdHeaderItem(const std::string&) = delete;
  FFuaCmdHeaderItem() { this->initVars(); }

  void push_back(FFuaCmdItem* child) { myChildren.push_back(child); }
  void clear() { myChildren.clear(); }

  const std::vector<FFuaCmdItem*>& getChildren() const { return myChildren; }

private:
  std::vector<FFuaCmdItem*> myChildren; //!< Leaf FFuaCmdItem objects
};
/////////////////////////////////////////////////////////////////////////////

static FFaInitialisation<FFuaCmdItem> cmdItemInit;

#endif
