#pragma once 

#include "TrMapData.hpp"


class TrMap;

// map
// is this too much memory? make sure to check
class TrWaterMap : public TrMapData<double> {
public:
	TrMapData<double>* m_water_temp;
	TrWaterMap(int rows, int cols) 
		: TrMapData<double>(rows, cols)
		, m_water_temp(new TrMapData<double>(rows, cols)) {}

	
	void update(TrMap* map); 
};


