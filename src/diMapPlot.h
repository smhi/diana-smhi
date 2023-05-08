/*
 Diana - A Free Meteorological Visualisation Tool

 Copyright (C) 2006-2020 met.no

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
#ifndef diMapPlot_h
#define diMapPlot_h

#include "diFilledMap.h"
#include "diGlUtilities.h"
#include "diMapInfo.h"
#include "diPlot.h"
#include "diPlotCommand.h"
#include "diShapeObject.h"
#include "diUtilities.h"

#include <map>
#include <set>
#include <vector>

/**
 \brief Map layer plotting

 plots the map layer
 - simple coastlines plotting
 - filled land type (precalculated triangles)
 - lat/lon lines
 */
class MapPlot : public Plot
{
private:
  bool mapchanged; // redraw needed
  bool haspanned;
  MapInfo mapinfo;
  PlotOptions contopts; // contour options
  PlotOptions landopts; // land plot options
  PlotOptions lonopts;  // lon options
  PlotOptions latopts;  // lat options
  PlotOptions ffopts;   // frame options
  bool isactive[3];     // active data for zorder
  diutil::MapValueAnno_v value_annotations;

  DiGLCanvas* mCanvas;
  DiGLPainter::GLuint drawlist[3]; // openGL drawlists

  std::string bgcolourname_;

  static std::map<std::string, ShapeObject> shapemaps;
  static std::map<std::string, Area> shapeareas;

  /**
   * remove large jumps in a set of lines. Calls xyclip
   *
   * @param npos
   * @param x
   * @param y
   * @param xylim
   * @param jumplimit
   * @param plotanno
   * @param anno_position
   * @param anno
   */
  void clipPrimitiveLines(DiGLPainter* gl, int npos, float*, float*, const float xylim[4], float jumplimit, bool plotanno = false,
                          diutil::MapValuePosition anno_position = diutil::map_right, const std::string& anno = "");

  /**
   * plot a map from a Land4 formatted file
   * @param filename
   * @param
   * @param
   * @param
   * @param
   * @return
   */
  bool plotMapLand4(DiGLPainter* gl, const std::string&, const float[], const Linetype&, float, const Colour&);

  /**
   * Refactoring calculation of long lines
   * @brief
   *
   *
   */
  inline bool shouldPlotLongitudes(bool plot_lon, int ilon1, int ilon2);

  /**
   * Refactoring plotting of Longitudes
   * @brief
   */
  void plotLongitudes(DiGLPainter* gl, float& latitudeStep, int& ilat2, int& ilat1,
                      float& glat1, float& glat2, float& latmin, float& latmax,
                      float& glat, const int& ilon1, const int& ilon2, float& glon, float& longitudeStep,
                      const std::shared_ptr<const Transformation>& t_geo2map,
                      bool& lon_values, const float& jumplimit, diutil::MapValuePosition& lon_valuepos,
                      float& lon_fontsize, int& n, int& plotResolution,
                      bool& geo2xyError);

  /**
   * Refactoring calculation of Lati lines
   * @brief
   *
   *
   */
  inline bool shouldPlotLatitudes(bool plot_lat, int ilat1, int ilat2);

  /**
   * Refactoring plotting of Latitudes
   * @brief
   */
  void plotLatitudes(DiGLPainter* gl, float& latitudeStep, int& ilon1, int& ilon2,
                     float& glon1, float& glon2, float& lonmin, float& lonmax,
                     float& glat, int& ilat2, int& ilat1, float& glon, float& longitudeStep,
                     const std::shared_ptr<const Transformation>& t_geo2map,
                     bool& lat_values, const float& jumplimit, diutil::MapValuePosition& lat_valuepos,
                     float& lon_fontsize, int& n, int& plotResolution,
                     bool& geo2xyError);

  /**
   * Plot Lat/Lon lines with optional numbering
   * @param mapinfo
   * @param plot_lon
   * @param plot_lat
   * @param plotResolution
   * @return
   */
  bool plotGeoGrid(DiGLPainter* gl, const MapInfo& mapinfo, bool plot_lon, bool plot_lat, int plotResolution = 100);

  /**
   * plot a map from a simple text formatted file
   * @param filename
   * @return
   */
  bool plotLinesSimpleText(DiGLPainter* gl, const std::string& filename);

public:
  MapPlot();
  ~MapPlot();

  void setCanvas(DiCanvas* canvas) override;
  void changeProjection(const Area& mapArea, const Rectangle& plotSize, const diutil::PointI& physSize) override;
  std::string getEnabledStateKey() const override;
  /// plot map/lat/lon/frame in a specific zorder layer
  void plot(DiGLPainter* gl, PlotOrder porder) override;

  void plotMap(DiGLPainter* gl, int zorder);

  /// parse plotinfo
  bool prepare(const PlotCommand_cp&, bool ifequal = true);

  const std::string& bgcolourname() const { return bgcolourname_; }

private:
  static void referenceFilledMaps(const MapInfo& mi);
  static void dereferenceFilledMaps(const MapInfo& mi);

  static FilledMap* fetchFilledMap(const std::string& filename);

  typedef std::map<std::string, FilledMap> fmObjects_t;
  static fmObjects_t filledmapObjects; // filename -> map

  // filename -> reference count; separate from "filledmaps" because it may contain more elements
  typedef std::map<std::string, int> fmRefCounts_t;
  static fmRefCounts_t filledmapRefCounts;
};

#endif
