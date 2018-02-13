#pragma once 

#include "TrMapData.hpp"


class TrMap;


// map
// is this too much memory? make sure to check
class TrHeightMap : public TrMapData<double> {
public:

	TrHeightMap(int rows, int cols) 
		: TrMapData<double>(rows, cols) {}

	void update(TrMap* map); 
};

