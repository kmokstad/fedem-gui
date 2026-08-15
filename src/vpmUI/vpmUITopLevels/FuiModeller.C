// SPDX-FileCopyrightText: 2023 SAP SE
//
// SPDX-License-Identifier: Apache-2.0
//
// This file is part of FEDEM - https://openfedem.org
////////////////////////////////////////////////////////////////////////////////

#include "vpmUI/vpmUITopLevels/FuiModeller.H"
#include "vpmUI/FuiModes.H"
#include "vpmUI/Fui.H"

#include "vpmUI/vpmUIComponents/FuiPlayPanel.H"
#include "vpmUI/vpmUIComponents/Fui3DPoint.H"


Fmd_SOURCE_INIT(FUI_MODELLER, FuiModeller, FFuMDIWindow);


FuiModeller::FuiModeller()
{
  Fmd_CONSTRUCTOR_INIT(FuiModeller);

  myViewer = NULL;
  my3DpointUI = NULL;
  myPlayPanel = NULL;
}


void FuiModeller::initWidgets()
{
  if (myViewer)
    myViewer->toBack();

  my3DpointUI->toFront();
  myPlayPanel->toFront();
  myPlayPanel->popDown();
}


void FuiModeller::mapAnimControls(bool yesOrNo)
{
  if (yesOrNo)
    this->myPlayPanel->popUp();
  else
    this->myPlayPanel->popDown();
}


bool FuiModeller::onClose()
{
  FuiModes::cancel();
  Fui::modellerUI(false,true);
  return false;
}
