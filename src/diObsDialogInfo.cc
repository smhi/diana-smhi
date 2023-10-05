/*
  Diana - A Free Meteorological Visualisation Tool

  Copyright (C) 2018 met.no

  Contact information:
  Norwegian Meteorological Institute
  Box 43 Blindern
  0313 OSLO
  NORWAY
  email: diana@met.no

  This file is part of Diana

  Diana is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation; either version 2 of the License, or
  (at your option) any later version.

  Diana is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with Diana; if not, write to the Free Software
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
*/

#include "diObsDialogInfo.h"

ObsDialogInfo::PlotType::PlotType()
    : misc(ObsDialogInfo::criteria)
{
}

void ObsDialogInfo::PlotType::addButton(const std::string& name, const std::string& tip, int low, int high)
{
  Button b;
  b.name = name;
  b.tooltip = tip;
  b.low = low;
  b.high = high;
  button.push_back(b);
}

void ObsDialogInfo::PlotType::addExtraParameterButtons()
{
  bool wind_speed = false;
  bool wind_direction = false;
  for (const ObsDialogInfo::Button& b : button) {
    if (b.name == "dd")
      wind_direction = true;
    if (b.name == "ff")
      wind_speed = true;
  }
  if (wind_speed && wind_direction) {
    ObsDialogInfo::Button b;
    b.name = "Wind";
    button.insert(button.begin(), b);
  }
}

ObsDialogInfo::Par::Par(const std::string& name_, const std::string& button_tip_)
    : name(name_)
    , type(ObsDialogInfo::ParType::pt_std)
    , symbol(-1)
    , precision(0)
    , button_tip(button_tip_)
    , button_low(-50)
    , button_high(50)
{
}

ObsDialogInfo::Par::Par(const std::string& name_, ParType type_, int symbol_, int precision_, const std::string& button_tip_, int button_low_, int button_high_)
    : name(name_)
    , type(type_)
    , symbol(symbol_)
    , precision(precision_)
    , button_tip(button_tip_)
    , button_low(button_low_)
    , button_high(button_high_)
{
}

void ObsDialogInfo::addPlotType(const ObsDialogInfo::PlotType& p, bool addIfNoReaders)
{
  if (addIfNoReaders || !p.readernames.empty())
    plottype.push_back(p);
}

// static
ObsDialogInfo::Misc ObsDialogInfo::miscFromText(const std::string& text)
{
  if (text == "dev_field_button")
    return dev_field_button;
  if (text == "tempPrecision")
    return tempPrecision;
  if (text == "unit_ms")
    return unit_ms;
  if (text == "orientation")
    return orientation;
  if (text == "parameterName")
    return parameterName;
  if (text == "popup")
    return popup;
  if (text == "qualityflag")
    return qualityflag;
  if (text == "wmoflag")
    return wmoflag;
  if (text == "markerboxVisible")
    return markerboxVisible;
  if (text == "criteria")
    return criteria;
  if (text == "asFieldButton")
    return asFieldButton;
  if (text == "leveldiffs")
    return leveldiffs;
  if (text == "show_VV_as_code")
    return show_VV_as_code;
  if (text == "plotundef")
    return plotundef;
  return none;
}
