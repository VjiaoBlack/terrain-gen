
#pragma once

#include "TrMapData.hpp"

class TrMap;

// map
// is this too much memory? make sure to check
class TrColorMap : public TrMapData<uint32_t> {
public:
	TrColorMap(int rows, int cols) 
		: TrMapData<uint32_t>(rows, cols) {}

	~TrColorMap() {}

	void update(TrMap* map);  
};


