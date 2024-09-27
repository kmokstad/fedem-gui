<!---
  SPDX-FileCopyrightText: 2023 SAP SE

  SPDX-License-Identifier: Apache-2.0

  This file is part of FEDEM - https://openfedem.org
--->

# Open FEDEM Changelog

## [fedem-8.0.5] (2024-09-27)

### :rocket: Added

- The Channel field is updated to contain the column label for
  poly-line-from-file functions, also if the file used is a two-colum file.
  If the file have no headings, "n/a" is displayed in the Channel field.
- The built GUI will now enable the Windpower features, if the installation
  contains the AeroDyn binary and associated dynamis solver (not part of the package)
- The Users Guide now contains chapters on Windpower and Marine modeling.

### :bug: Fixed

- Issue https://github.com/openfedem/fedem-foundation/issues/11 :
  Previewing poly-line-from-file functions may cause crash or wrong results
  if the definition domain is smaller than the evaluation domain.
- Issue https://github.com/openfedem/fedem-gui/issues/24 :
  Modelling prismatic/cylindric joints by selecting existing triads does not work properly.
- Issue https://github.com/openfedem/fedem-gui/issues/23 :
  Using a time interval for a graph plotting simulation results have some round-off errors.
- Issue https://github.com/openfedem/fedem-gui/issues/20 :
  Multi-select does not work when updating joint DOF properties.
- Issue https://github.com/openfedem/fedem-gui/issues/17 :
  The Fourier Analysis and Differntiate/Integrate toggles on curves do not work properly.
- Issue https://github.com/openfedem/fedem-gui/issues/15 :
  The Additional Solver Options field for the Dynamics Solver does not work always.

## [fedem-8.0.4] (2024-07-12)

### :bug: Fixed

- Issue https://github.com/openfedem/fedem-gui/issues/13 :
  Dynamics solver does not start if the model uses single-channel input functions.

## [fedem-8.0.3] (2024-06-13)

### :rocket: Added

- Support for L-profiles from PBEAML-records in Nastran bulk data files.
- Account for non-symmetric beam cross sections from Nastran bulk data files.
- A progress bar in the bottom of the main GUI running while creating
  the visualization model of the FE parts.
- The content of the View control toolbar in the GUI will depend on whether
  Modeler view, at least one Graph view, or both are open.
- The links to external web-sites in the Fedem.chm file are now opened
  in the default web-browser.

### :bug: Fixed

- Nastran bulk data files with PBEAM records with an empty or blank line
  immediately after the SO-field is not parsed correctly.
- Nastran bulk data files containing case control statements before the start
  of the bulk data is not parsed correctly.
- Parsing of Nastran bulk data files containing CBEAM records where the 7th
  field contains a text string fails.
- Multiple SPC (or SPC1) entries in a Nastran bulk data file for the same node
  is not handled correctly.
- Issue https://github.com/openfedem/fedem-gui/issues/1 :
  Loading animations in the GUI with the "Load line contours" toggle activated
  may cause crash.
- Issue https://github.com/openfedem/fedem-gui/issues/4 :
  The "Move to Center" command in the GUI does not work.

## [fedem-8.0.2] (2024-02-29)

### :rocket: Added

- A "Recent models" sub-menu is added in the "File" menu, from where (up to)
  the 10 last opened models in the current session can be reopened.
- If the model contains Strain Rosette elements which are used by Sensors,
  the "Perform strain rosette recovery during dynamics simulation" toggle
  is automatically enabled for FE Parts containing such Strain Rosette elements.

### :bug: Fixed

- If a template file is not available the default model name is not set and the
  current working directory is attempted cleaned with strange console messages.
- The command for opening the Users Guide is missing in the "Help" menu.
- The GUI crashes if selecting "Generic DB Object" from the "Mechanism" menu.
- The GUI crashes if an invalid expression with unbalanced parantheses
  is entered for Math Expression functions.
- The COM Type library for Fedem is missing.

## [fedem-8.0.1] (2024-01-25)

### :rocket: Added

- The "Marine" menu with hydrodynamics features is back in the GUI.

### :bug: Fixed

- Graph views show only the first point in each curve when plotting
  time history results.
- Curve plotting is deactivated and Curve/Graph properties are hidden.
- Incorrect default location of file browser on "Open" and "Save As...".
- False error messages on failure to delete files when doing "Save As...".

## fedem-8.0.0 (2023-12-21)

### :rocket: Added

- This is the first open source version of FEDEM,
  including binaries for GUI and solvers on Windows platform.

[fedem-8.0.1]: https://github.com/openfedem/fedem-gui/compare/fedem-8.0.0...fedem-8.0.1
[fedem-8.0.2]: https://github.com/openfedem/fedem-gui/compare/fedem-8.0.1...fedem-8.0.2
[fedem-8.0.3]: https://github.com/openfedem/fedem-gui/compare/fedem-8.0.2...fedem-8.0.3
[fedem-8.0.4]: https://github.com/openfedem/fedem-gui/compare/fedem-8.0.3...fedem-8.0.4
[fedem-8.0.5]: https://github.com/openfedem/fedem-gui/compare/fedem-8.0.4...fedem-8.0.5
