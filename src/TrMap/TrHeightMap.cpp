
#include "TrHeightMap.hpp"
#include "TrMap.hpp"

void TrHeightMap::update(TrMap* map) {
	for (int i = 0; i < m_rows; i++) {
		for (int j = 0; j < m_cols; j++) {
			m_terrace.at(i,j) = floor(this->at(i,j) * 31);
		}
	}
}