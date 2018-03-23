
#include "TrMoistureMap.hpp"
#include "TrMap.hpp"

void TrMoistureMap::update(TrMap* map) {
  // for (int j = 0; j < m_cols; j++) {
  //   for (int i = 0; i < m_rows; i++) {
  //     if (map->m_water->at(i,j) + m_height->at(i,j) < 0.458) {
  //       map->m_water->at(i,j) = 0.458 - m_height->at(i,j);
  //     }
  //   }
  // }

  // // Good Moisture Carried by Wind
  // // this->set(0.0);

  // for (int j = 0; j < m_cols; j++) {
  //   for (int i = 0; i < m_rows; i++) {
  //     this->at(i,j) *= 0.9; // random drying bullshit
  //     this->at(i,j) += map->m_water->at(i,j) * 3.0; // random water
  //     humidifying bullshit

  //     if (this->at(i,j) < 0) this->at(i,j) = 0;
  //     if (this->at(i,j) > 1.0) this->at(i,j) = 1.0;
  //   }
  // }

  // for (int j = 0; j < m_cols; j++) {
  //   for (int i = 0; i < m_rows; i++) {
  //     // propogate moisture forwards based on current WATER and MOISTURE
  //     this->at(i  ,j+1) += (this->at(i,j)*0.5) * (m_wind->at(i,j).x +
  //     m_wind->at(i,j).z * 0.25 + abs(m_wind->at(i,j).z) * 0.05) * 0.98;
  //     if (m_wind->at(i,j).y > 0.0) {
  //       this->at(i+1,j+1) += (this->at(i,j)*0.5) * m_wind->at(i,j).y * 0.98;
  //     } else {
  //       this->at(i-1,j+1) -= (this->at(i,j)*0.5) * m_wind->at(i,j).y * 0.98;
  //     }
  //     this->at(i,j) *= 0.5;

  //   }
  // }

  // this->boxBlur();
  // this->boxBlur();
  // this->boxBlur();
  // this->boxBlur();
  // this->boxBlur();

  // for (int i = 0; i < m_rows; i++) {
  //   for (int j = 0; j < m_cols; j++) {
  //     if (this->at(i,j) > 0.1) {
  //       map->m_vegetation->at(i,j) += 0.01;
  //     } else {
  //       map->m_vegetation->at(i,j) -= 0.01;
  //     }
  //   }
  // }

  // // Moisture: carried by WIND...
  // for (int i = 0; i < m_rows; i++) {
  //     for (int j = 0; j < m_cols; j++) {
  //         // if there is water
  //         if (map->m_water->at(i,j) > 0.002) {
  //             // increase moisture from previous tile
  //             // wait, which is the previous tile???
  //             this->at(i,j) = 1.0;
  //         } else {
  //             this->at(i,j) = this->at(i,j) * 0.35;
  //         }

  //         if (this->at(i,j) > 0.1 && this->at(i,j) < 0.9) {
  //             map->m_vegetation->at(i,j) += 0.01;
  //         } else {
  //             map->m_vegetation->at(i,j) -= 0.01;
  //         }

  //         if (this->at(i,j) < 0.0) this->at(i,j) = 0;
  //         if (this->at(i,j) > 1.0) this->at(i,j) = 1.0;
  //     }
  // }

  // for (int i = 0; i < m_rows; i++) {
  //     for (int j = 0; j < m_cols; j++) {
  //         this->at(i  ,j+1) += this->at(i,j) * 0.60 * m_wind->at(i,j).y;
  //         if (m_wind->at(i,j).x > 0.0) {
  //             this->at(i+1,j+1) += this->at(i,j) * 0.60 * m_wind->at(i,j).x;
  //         } else {
  //             this->at(i-1,j+1) -= this->at(i,j) * 0.60 * m_wind->at(i,j).x;
  //         }

  //         if (this->at(i,j) < 0) {
  //             this->at(i,j) = 0;
  //             printf("%f\n", m_wind->at(i,j).x);
  //         }
  //     }
  // }

  // this->boxBlur();

  // Regular moisture
  // okay set moisture to 1 if there is some water
  for (int i = 0; i < m_rows; i++) {
    for (int j = 0; j < m_cols; j++) {
      if (map->m_water->at(i, j) > 0.01) {
        this->at(i, j) = 1.0;
      } else {
        this->at(i, j) = this->at(i, j) * 0.50 + map->m_water->at(i, j) * 50.0;
      }

      if (this->at(i, j) > 0.1 && this->at(i, j) < 0.5) {
        map->m_vegetation->at(i, j) += 0.01;
      } else {
        map->m_vegetation->at(i, j) -= 0.01;
      }
    }
  }

  for (int i = 0; i < m_rows; i++) {
    for (int j = 0; j < m_cols; j++) {
      this->at(i, j + 1) += this->at(i, j) * 0.25;
      this->at(i + 1, j + 1) += this->at(i, j) * 0.125;
      this->at(i - 1, j + 1) += this->at(i, j) * 0.125;
    }
  }

  this->boxBlur();
}