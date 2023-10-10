/*
  Diana - A Free Meteorological Visualisation Tool

  Copyright (C) 2013-2022 met.no

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

#include "diArea.h"
#include "util/geo_util.h"
#include "util/string_util.h"

#include <puTools/miString.h>
#include <puDatatypes/miCoordinates.h>

#define MILOGGER_CATEGORY "diField.Area"
#include "miLogger/miLogging.h"

Area::Area(){
}

Area::Area(const Projection& p, const Rectangle& r)
: proj(p), rect(r){
}

Area::Area(const std::string& n, const Projection& p,
     const Rectangle& r)
: proj(p), rect(r), name(n)
{
}

Area::Area(const Area &rhs)
{
  memberCopy(rhs);
}

Area::~Area(){
}

Area& Area::operator=(const Area &rhs)
{
  if (this == &rhs)
    return *this;

  memberCopy(rhs);
  return *this;
}

bool Area::operator==(const Area &rhs) const
{
  return ((proj == rhs.proj) && (rect == rhs.rect));
}

std::ostream& operator<<(std::ostream& output, const Area& a)
{
  return output << "name="<<a.name<<" "<<a.proj<<"  "<<a.rect;
//  return output << "P:"<<a.proj<<" R:"<<a.rect;
}

void Area::memberCopy(const Area& rhs)
{
  proj= rhs.proj;
  rect= rhs.rect;
  name= rhs.name;
}

bool Area::setAreaFromString(const std::string& areaString)
{
  // FIXME: this is VERY similar to PlotModule::prepareArea, the difference
  // is that this version does not look up name in MapAreaSetup

  if (areaString.empty())
    return false;

  const char key_name[] = "name";
  const char key_proj[] = "proj4string";
  const char key_WKT[] = "wktstring";
  const char key_rectangle[] = "rectangle";
  const char key_rect[] = "rect";

  std::string rectangleStr;
  std::string projStr;
  std::string wktStr;
  bool angles_deg = false;

  // split on blank, preserve ""
  const std::vector<std::string> tokens= miutil::split_protected(areaString, '"','"'," ",true);

  const int n = tokens.size();
  for (int i=0; i<n; i++){
    const std::vector<std::string> stokens= miutil::split(tokens[i], 1, "=");
    if (stokens.size() > 1) {
      const std::string key = miutil::to_lower(stokens[0]);
      if (key==key_name){
        name = stokens[1];
      } else if (key==key_proj){
        projStr = stokens[1];
        diutil::remove_quote(projStr);
      } else if (key==key_WKT){
        wktStr = stokens[1];
        diutil::remove_quote(wktStr);
      } else if (key==key_rectangle){
        rectangleStr = stokens[1];
        angles_deg = false;
      } else if (key == key_rect) {
        rectangleStr = stokens[1];
        angles_deg = true;
      }
    }
  }

  if (projStr.empty() && wktStr.empty()) {
    //Undefined projections (used in model/sat area)
    return true;
  }
  if (!projStr.empty())
    if (!proj.setFromString(projStr))
      return false;
  if(!wktStr.empty())
    if (!proj.setFromWKT(wktStr))
      return false;

  if (!rect.setRectangle(rectangleStr))
    return false;

  if (!angles_deg && proj.isDegree()) {
    // backward compatibility: when using proj4, rectangles were in radians; keep it like that for now
    diutil::convertRectToDegrees(rect);
  }
  return true;
}

std::string Area::getAreaString() const
{
  std::ostringstream ost;
  ost << "proj4string=\"" << proj.getProj4Definition() << "\""
      << " rect=" << rect.toString();
  return ost.str();
}

double Area::getDiagonalInMeters() const
{
  float x[2],y[2];
  x[0]=rect.x1;
  x[1]=rect.x2;
  y[0]=rect.y1;
  y[1]=rect.y2;
  proj.convertToGeographic(2,x,y);

  LonLat lowerLeft = LonLat::fromDegrees(x[0],y[0]);
  LonLat upperRight =LonLat::fromDegrees(x[1],y[1]);

  return lowerLeft.distanceTo(upperRight);
}

GridArea GridArea::scaled(int factor) const
{
  GridArea s(*this);
  s.resolutionX *= factor;
  s.resolutionY *= factor;
  s.nx /= factor;
  s.ny /= factor;
  return s;
}

bool GridArea::operator==(const GridArea& rhs) const
{
  return Area::operator==(rhs) && nx == rhs.nx && ny == rhs.ny && resolutionX == rhs.resolutionX && resolutionY == rhs.resolutionY;
}

std::ostream& operator<<(std::ostream& output, const GridArea& ga)
{
  output << "[GridArea P:" << ga.P() << " R: " << ga.R()
         << " n:" << ga.nx << "*" << ga.ny
         << " res:" << ga.resolutionX << "*" << ga.resolutionY << "]";
  return output;
}
