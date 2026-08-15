// SPDX-FileCopyrightText: 2023 SAP SE
//
// SPDX-License-Identifier: Apache-2.0
//
// This file is part of FEDEM - https://openfedem.org
////////////////////////////////////////////////////////////////////////////////

#include <iostream>

#include "vpmUI/vpmUIComponents/FuiSNCurveSelector.H"
#include "FFuLib/FFuOptionMenu.H"
#include "FFpLib/FFpFatigue/FFpSNCurveLib.H"


void FuiSNCurveSelector::initWidgets()
{
  stdTypeMenu->setOptionSelectedCB(FFaDynCB1M(FuiSNCurveSelector,this,
                                              onStdValueChanged,int));
  curveTypeMenu->setOptionSelectedCB(FFaDynCB1M(FuiSNCurveSelector,this,
                                                onCurveValueChanged,int));
}


void FuiSNCurveSelector::setSensitivity(bool isSensitive)
{
  stdTypeMenu->setSensitivity(isSensitive);
  curveTypeMenu->setSensitivity(isSensitive);
}


void FuiSNCurveSelector::onStdValueChanged(int)
{
  std::string selectedCurve = curveTypeMenu->getSelectedOptionStr();
  this->populateCurveMenu(stdTypeMenu->getSelectedOptionStr());

  if (!curveTypeMenu->selectOption(selectedCurve))
    curveTypeMenu->selectOption(0);

  dataChangedCB.invoke();
}


std::string FuiSNCurveSelector::getSelectedStd() const
{
  return stdTypeMenu->getSelectedOptionStr();
}


std::string FuiSNCurveSelector::getSelectedCurve() const
{
  return curveTypeMenu->getSelectedOptionStr();
}


void FuiSNCurveSelector::getValues(int& stdIdx, int& curveIdx)
{
  stdIdx   = stdTypeMenu->getSelectedOption();
  curveIdx = curveTypeMenu->getSelectedOption();
}


void FuiSNCurveSelector::setValues(int stdIdx, int curveIdx)
{
  std::vector<std::string> curveStds;
  if (FFpSNCurveLib::allocated())
    FFpSNCurveLib::instance()->getCurveStds(curveStds);

  if (stdIdx >= static_cast<int>(curveStds.size()))
  {
    if (FFpSNCurveLib::allocated())
      std::cout <<"Warning: SN-curve library has changed since you last saved your model"
                <<"\n         stdIdx = "<< stdIdx << std::endl;
    stdIdx = 0;
  }

  stdTypeMenu->setOptions(curveStds);
  if (stdIdx >= 0)
    stdTypeMenu->selectOption(stdIdx);

  this->populateCurveMenu(stdTypeMenu->getSelectedOptionStr(),curveIdx);
}


void FuiSNCurveSelector::populateCurveMenu(const std::string& stdName,
					   int curveIdx)
{
  std::vector<std::string> curves;
  if (FFpSNCurveLib::allocated())
    FFpSNCurveLib::instance()->getCurveNames(curves,stdName);

  if (curveIdx >= static_cast<int>(curves.size()))
  {
    if (FFpSNCurveLib::allocated())
      std::cout <<"Warning: SN-curve library has changed since you last saved your model"
                <<"\n         curveIdx = "<< curveIdx << std::endl;
    curveIdx = 0;
  }

  curveTypeMenu->setOptions(curves);
  if (curveIdx >= 0)
    curveTypeMenu->selectOption(curveIdx);
}
