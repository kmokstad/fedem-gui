// SPDX-FileCopyrightText: 2023 SAP SE
//
// SPDX-License-Identifier: Apache-2.0
//
// This file is part of FEDEM - https://openfedem.org
////////////////////////////////////////////////////////////////////////////////

#include "vpmUI/vpmUITopLevels/FuiCtrlModeller.H"
#include "vpmUI/vpmUITopLevels/FuiCtrlGridAttributes.H"
#include "vpmUI/Fui.H"


Fmd_SOURCE_INIT(FUI_CTRLMODELLER, FuiCtrlModeller, FFuMDIWindow);


FuiCtrlModeller::FuiCtrlModeller()
{
  Fmd_CONSTRUCTOR_INIT(FuiCtrlModeller);

  myViewer = NULL;
  myCtrlGridAttributes = NULL;
}


FuiCtrlModeller::~FuiCtrlModeller()
{
  delete myCtrlGridAttributes;
}


void FuiCtrlModeller::initWidgets()
{
  FFuUAExistenceHandler::invokeCreateUACB(this);
}


void FuiCtrlModeller::showGridUI(bool doShow)
{
  if (!myCtrlGridAttributes)
    return;

  if (doShow) {
    myCtrlGridAttributes->popUp();
    myCtrlGridAttributes->showTLSNormal();
  }
  else
    myCtrlGridAttributes->popDown();
}


bool FuiCtrlModeller::onClose()
{
  this->invokeFinishedCB();
  return false;
}
