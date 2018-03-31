/**
 * TrData.cpp
 */

#include "TrData.hpp"

TrData::TrData() {
  // load Entities
  {
    string name;
    string color;
    string size;
    string footprint;
    // read Actors
    {
      io::CSVReader<4> in("data/Entities - Actors.csv");
      in.read_header(io::ignore_extra_column, "name", "color", "size",
                     "footprint");

      while (in.read_row(name, color, size, footprint)) {
        // do stuff with the data
        cout << name << endl;
      }
    }
  }
}

TrData& TrData::getInstance() {
  static TrData instance;
  return instance;
}