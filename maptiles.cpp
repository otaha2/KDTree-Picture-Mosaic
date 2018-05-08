/**
 * @file maptiles.cpp
 * Code for the maptiles function.
 */

#include <iostream>
#include <map>
#include "maptiles.h"

using namespace std;

Point<3> convertToLAB(HSLAPixel pixel) {
    Point<3> result(pixel.h/360, pixel.s, pixel.l);
    return result;
}

MosaicCanvas* mapTiles(SourceImage const& theSource,
                       vector<TileImage>& theTiles)
{
    
     //make new mosaicCanvas
     MosaicCanvas* output = new MosaicCanvas(theSource.getRows(), theSource.getColumns());

     //make a new KDTree and map with all the Image's average color points 
     vector<Point<3>> Images;
     Point<3> p;
     HSLAPixel pixel;
     map<Point<3>, int> tile_map;
     for(size_t i = 0; i < theTiles.size(); i++){
        pixel = theTiles[i].getAverageColor();
        Images.push_back(convertToLAB(pixel));
        tile_map[convertToLAB(theTiles[i].getAverageColor())] = i;
     }
     KDTree<3> tree(Images);

     //set the correct images
     for(int i = 0; i < theSource.getRows(); i++){
       for(int j = 0; j < theSource.getColumns(); j++){
         output->setTile(i, j, get_match_at_idx(tree, tile_map, theTiles, theSource, i, j));
       }
     }
     
    return output;
}

TileImage* get_match_at_idx(const KDTree<3>& tree,
                                  map<Point<3>, int> tile_avg_map,
                                  vector<TileImage>& theTiles,
                                  const SourceImage& theSource, int row,
                                  int col)
{
    // Create a tile which accurately represents the source region we'll be
    // using
    HSLAPixel avg = theSource.getRegionColor(row, col);
    Point<3> avgPoint = convertToLAB(avg);
    Point<3> nearestPoint = tree.findNearestNeighbor(avgPoint);

    // Check to ensure the point exists in the map
    map< Point<3>, int >::iterator it = tile_avg_map.find(nearestPoint);
    if (it == tile_avg_map.end())
        cerr << "Didn't find " << avgPoint << " / " << nearestPoint << endl;

    // Find the index
    int index = tile_avg_map[nearestPoint];
    return &theTiles[index];

}
