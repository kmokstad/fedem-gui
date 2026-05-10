// SPDX-FileCopyrightText: 2023 SAP SE
//
// SPDX-License-Identifier: Apache-2.0
//
// This file is part of FEDEM - https://openfedem.org
////////////////////////////////////////////////////////////////////////////////

#ifndef FFU_QT_TOOL_BAR_H
#define FFU_QT_TOOL_BAR_H

#include <QToolBar>

#include "FFuLib/FFuToolBar.H"
#include "FFuLib/FFuQtBaseClasses/FFuQtComponentBase.H"


class FFuQtToolBar : public QToolBar, public FFuQtComponentBase,
                     virtual public FFuToolBar
{
public:
  FFuQtToolBar(QWidget* parent);

private:
  virtual void insertCmdItem(FFuaCmdItem* item);
  virtual void updateCmdItem(FFuaCmdItem* item, bool sensitivity);

  virtual void clearBar();
  virtual void setBarLabel(const std::string& label);
  virtual void setBarOrientation(int orientation);
};

#endif
