/**
 * TrData.cpp
 */

#include <sstream>

#include "TrData.hpp"

#include "../TrECS/TrComponents/TrFootprintComponent.hpp"
#include "../TrECS/TrComponents/TrGraphicsComponent.hpp"
#include "../TrECS/TrComponents/TrPhysicsComponent.hpp"
#include "../TrECS/TrComponents/TrPlanningComponent.hpp"

#include "../TrECS/TrEntityTypes/TrActorEntityType.hpp"
#include "../TrECS/TrEntityTypes/TrBuildingEntityType.hpp"
#include "../TrECS/TrEntityTypes/TrPlantEntityType.hpp"

map<string, TrEntityType*> TrData::m_entityTypes = map<string, TrEntityType*>();
map<string, TrFormula*> TrData::m_formulas = map<string, TrFormula*>();
map<string, TrItem*> TrData::m_items = map<string, TrItem*>();

inline static void trim(string& str) {
  int start = 0;
  int end = str.size() - 1;

  while (str[start] == ' ' && start < str.size()) {
    start++;
  }
  while (str[end] == ' ' && end >= 0) {
    end--;
  }

  str = str.substr(start, end - start + 1);
}

inline static SDL_Color parseColor(const string& color) {
  unsigned char r, g, b;
  stringstream ss;
  ss << std::hex << color.substr(1, 2);
  ss >> r;
  ss.clear();
  ss << std::hex << color.substr(3, 2);
  ss >> g;
  ss.clear();
  ss << std::hex << color.substr(5, 2);
  ss >> b;
  ss.clear();

  return (SDL_Color){0xFF, r, g, b};
}

inline static pair<int, int> parseSize(const string& size) {
  size_t pos = size.find('x');

  return make_pair(stoi(size.substr(0, pos)), stoi(size.substr(pos + 1)));
}

inline static pair<int, string> parseIngredient(const string& ingredient) {
  int num;
  string name;

  stringstream ss;
  ss << ingredient;
  ss >> num;
  getline(ss, name);
  trim(name);

  return make_pair(num, name);
}

inline static vector<pair<int, string>> parseIngredientList(
    const string& ingredientList) {
  vector<pair<int, string>> ans;

  size_t pos, lastPos = 0;

  do {
    pos = ingredientList.substr(lastPos).find(';');

    if (pos == string::npos) {
      ans.push_back(parseIngredient(ingredientList.substr(lastPos)));
    } else {
      ans.push_back(parseIngredient(ingredientList.substr(lastPos, pos)));
    }

    lastPos += pos + 1;
  } while (pos != string::npos);

  return ans;
}

inline static vector<string> parseTagList(const string& tagList) {}

void TrData::loadData() {
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

        TrGraphicsComponent* graphics =
            new TrGraphicsComponent(parseColor(color));
        TrPhysicsComponent* physics = new TrPhysicsComponent(0, 0);
        TrPlanningComponent* planning = new TrPlanningComponent();

        TrData::m_entityTypes[name] =
            new TrActorEntityType(graphics, physics, planning);
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
        TrGraphicsComponent* graphics =
            new TrGraphicsComponent((SDL_Color){0, 0, 0, 0});
        pair<int, int> footprintSize(1, 1);
        if (size.size() > 0) {
          footprintSize = parseSize(size);
        }
        TrFootprintComponent* footprint = new TrFootprintComponent(
            footprintSize.first, footprintSize.second, NULL);

        TrData::m_entityTypes[name] =
            new TrBuildingEntityType(graphics, footprint);
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
        TrGraphicsComponent* graphics =
            new TrGraphicsComponent((SDL_Color){0, 0, 0, 0});
        pair<int, int> footprintSize(1, 1);
        if (size.size() > 0) {
          footprintSize = parseSize(size);
        }
        TrFootprintComponent* footprint = new TrFootprintComponent(
            footprintSize.first, footprintSize.second, NULL);

        TrData::m_entityTypes[name] =
            new TrPlantEntityType(graphics, footprint);
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
        TrData::m_items[name] = new TrItem(name);
      }
    }

    // read Manufactured Products
    {
      io::CSVReader<3> in("data/Items - Manufactured Products.csv");
      in.read_header(io::ignore_extra_column, "name", "tags", "attributes");

      while (in.read_row(name, tags, attributes)) {
        // do stuff with the data
        cout << name << endl;
        TrData::m_items[name] = new TrItem(name);
      }
    }

    // read Produce
    {
      io::CSVReader<3> in("data/Items - Produce.csv");
      in.read_header(io::ignore_extra_column, "name", "tags", "attributes");

      while (in.read_row(name, tags, attributes)) {
        // do stuff with the data
        cout << name << endl;
        TrData::m_items[name] = new TrItem(name);
      }
    }

    // read Raw Materials
    {
      io::CSVReader<3> in("data/Items - Raw Materials.csv");
      in.read_header(io::ignore_extra_column, "name", "tags", "attributes");

      while (in.read_row(name, tags, attributes)) {
        // do stuff with the data
        cout << name << endl;
        TrData::m_items[name] = new TrItem(name);
      }
    }
  }

  // load Formulas
  {
    string labor;
    string inputs;
    string outputs;
    string byproducts;

    // read Formulas - Items
    {
      io::CSVReader<4> in("data/Formulas - Items.csv");
      in.read_header(io::ignore_extra_column, "labor", "inputs", "outputs",
                     "byproducts");

      while (in.read_row(labor, inputs, outputs, byproducts)) {
        // do stuff with the data
        cout << outputs << endl;
        parseIngredientList(inputs);
        parseIngredientList(outputs);
        parseIngredientList(byproducts);
      }
    }

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
  }
}

void TrData::deleteData() {}
