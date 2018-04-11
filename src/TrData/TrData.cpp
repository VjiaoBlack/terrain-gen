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

    // read Buildings
    {
      io::CSVReader<4> in("data/Entities - Buildings.csv");
      in.read_header(io::ignore_extra_column, "name", "color", "size",
                     "footprint");

      while (in.read_row(name, color, size, footprint)) {
        // do stuff with the data
        cout << name << endl;
        // m_entities[name] = new TrBuildingEntity(
        //   TrGame* game,
        //   SDL_Rect rect,
        //   TrBuildingGraphicsComponent* graphics,
        //   TrFootprintComponent* footprint

        //   );
      }
    }

    // read Plants
    {
      io::CSVReader<4> in("data/Entities - Plants.csv");
      in.read_header(io::ignore_extra_column, "name", "color", "size",
                     "footprint");

      while (in.read_row(name, color, size, footprint)) {
        // do stuff with the data
        cout << name << endl;
      }
    }
  }

  // load Formulas
  {
    string labor;
    string inputs;
    string outputs;
    string byproducts;
    // read Buildings
    {
      io::CSVReader<4> in("data/Formulas - Buildings.csv");
      in.read_header(io::ignore_extra_column, "labor", "inputs", "outputs",
                     "byproducts");

      while (in.read_row(labor, inputs, outputs, byproducts)) {
        // do stuff with the data
        cout << outputs << endl;
      }
    }

    // read Items
    {
      io::CSVReader<4> in("data/Formulas - Items.csv");
      in.read_header(io::ignore_extra_column, "labor", "inputs", "outputs",
                     "byproducts");

      while (in.read_row(labor, inputs, outputs, byproducts)) {
        // do stuff with the data
        cout << outputs << endl;
      }
    }
  }

  // load Items
  {
    string name;
    string tags;
    string attributes;
    // read Animal Products
    {
      io::CSVReader<3> in("data/Items - Animal Products.csv");
      in.read_header(io::ignore_extra_column, "name", "tags", "attributes");

      while (in.read_row(name, tags, attributes)) {
        // do stuff with the data
        cout << name << endl;
      }
    }

    // read Manufactured Products
    {
      io::CSVReader<3> in("data/Items - Manufactured Products.csv");
      in.read_header(io::ignore_extra_column, "name", "tags", "attributes");

      while (in.read_row(name, tags, attributes)) {
        // do stuff with the data
        cout << name << endl;
      }
    }

    // read Produce
    {
      io::CSVReader<3> in("data/Items - Produce.csv");
      in.read_header(io::ignore_extra_column, "name", "tags", "attributes");

      while (in.read_row(name, tags, attributes)) {
        // do stuff with the data
        cout << name << endl;
      }
    }

    // read Raw Materials
    {
      io::CSVReader<3> in("data/Items - Raw Materials.csv");
      in.read_header(io::ignore_extra_column, "name", "tags", "attributes");

      while (in.read_row(name, tags, attributes)) {
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