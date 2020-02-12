// SPDX-FileCopyrightText: 2023 SAP SE
//
// SPDX-License-Identifier: Apache-2.0
//
// This file is part of FEDEM - https://openfedem.org
////////////////////////////////////////////////////////////////////////////////

#ifndef FAP_CTRL_3D_OBJ_CMDS_H
#define FAP_CTRL_3D_OBJ_CMDS_H

#include "FapCmdsBase.H"
#include "FFaLib/FFaPatterns/FFaInitialisation.H"


class FapCtrl3DObjCmds : public FapCmdsBase
{
  static void init();

  // creation
  static void createInput();
  static void createOutput();

  static void createAmplifier();
  static void createPower();

  static void createComparator();
  static void createAdder();
  static void createMultiplyer();

  static void createIntegrator();
  static void createLimDerivator();

  static void createTimeDelay();
  static void createSampleHold();

  static void createLogicalSwitch();
  static void createLimitation();

  static void createDeadZone();
  static void createHysteresis();

  static void createPid();
  static void createPi();
  static void createPd();

  static void createPlimIlimD();
  static void createPIlimD();
  static void createPlimI();
  static void createPlimD();

  static void createRealPole();
  static void createCompConjPole();

  static void create1ordTF();
  static void create2ordTF();

  static void createBandStop();

  static void ctrlCreate(int elemType);

  // operation
  static void rotate();
  static void addPoint();
  static void removePoint();

  // sensitivity
  static void getSensitivity(bool& isSensitive);

  friend class FFaInitialisation<FapCtrl3DObjCmds>;
};

static FFaInitialisation<FapCtrl3DObjCmds> FapCtrl3DObjCmdsInit;

#endif
