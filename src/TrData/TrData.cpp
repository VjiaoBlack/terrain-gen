/**
 * TrData.cpp
 */

#include <sstream>

#include "TrData.hpp"

#include <regex>

map<string, std::unique_ptr<TrActorEntityType>> TrData::m_actorTypes =
    map<string, std::unique_ptr<TrActorEntityType>>();

map<string, std::unique_ptr<TrBuildingEntityType>> TrData::m_buildingTypes =
    map<string, std::unique_ptr<TrBuildingEntityType>>();

map<string, std::unique_ptr<TrPlantEntityType>> TrData::m_plantTypes =
    map<string, std::unique_ptr<TrPlantEntityType>>();

map<string, std::unique_ptr<TrFormula>> TrData::m_formulas =
    map<string, std::unique_ptr<TrFormula>>();

map<string, std::unique_ptr<TrItem>> TrData::m_items =
    map<string, std::unique_ptr<TrItem>>();

inline static void trim(string &str) {
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

inline static SDL_Color parseColor(const string &color) {
  int r, g, b;
  stringstream ss;

  ss << color.substr(1, 2);
  ss >> std::hex >> r;
  ss.clear();
  ss << color.substr(3, 2);
  ss >> std::hex >> g;
  ss.clear();
  ss << color.substr(5, 2);
  ss >> std::hex >> b;
  ss.clear();

  return (SDL_Color) {(uint8_t) r, (uint8_t) g, (uint8_t) b, 0xFF};
}

inline static pair<int, int> parseSize(const string &size) {
  size_t pos = size.find(';');

  return make_pair(stoi(size.substr(0, pos)), stoi(size.substr(pos + 1)));
}

inline static pair<int, string> parseIngredient(const string &ingredient) {
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
    const string &ingredientList) {
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

inline static vector<string> parseList(const string &tagList) {
  vector<string> ret;
  static const regex tagRgx("([^;]*);?");
  smatch result;

  auto curPos(tagList.cbegin());
  if (regex_search(curPos, tagList.cend(), result, tagRgx)) {
    curPos += result.position() + result.length();
    // TODO: is this right? Why don't we push the first result?
    ret.push_back(result[1]);
    while (regex_search(curPos, tagList.cend(), result, tagRgx) &&
        string(result[1]).size() > 0) {

      ret.push_back(result[1]);
      curPos += result.position() + result.length();
    }
  }

  return ret;
}

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
        TrGraphicsComponent graphics = TrGraphicsComponent(parseColor(color));
        auto sizePair = parseSize(size);
        auto physics = TrPhysicsComponent(sizePair.first, sizePair.second);
        auto planning = TrPlanningComponent();

        vector<char> footprintVec(sizePair.first * sizePair.second);
        auto footprintData = parseList(footprint);

        for (int r = 0; r < sizePair.first; r++) {
          for (int c = 0; c < sizePair.second; c++) {
            footprintVec[r * sizePair.second + c] = footprintData[r][c];
          }
        }

        auto footprintComponent = TrFootprintComponent(
            sizePair.first, sizePair.second, move(footprintVec));

        auto actionComponent = TrActionComponent();

        TrData::m_actorTypes[name] =
            std::make_unique<TrActorEntityType>(name, graphics, physics, planning,
                                               footprintComponent,
                                  actionComponent);
      }
    }

    // read Buildings
    {
      io::CSVReader<4> in("data/Entities - Buildings.csv");
      in.read_header(io::ignore_extra_column, "name", "color", "size",
                     "footprint");

      while (in.read_row(name, color, size, footprint)) {
        // do stuff with the data
        TrGraphicsComponent graphics =
            TrGraphicsComponent(parseColor(color));
        auto sizePair = parseSize(size);

        vector<char> footprintVec(sizePair.first * sizePair.second);
        auto footprintData = parseList(footprint);

        for (int r = 0; r < sizePair.first; r++) {
          for (int c = 0; c < sizePair.second; c++) {
            footprintVec[r * sizePair.second + c] = footprintData[r][c];
          }
        }

        auto footprintComponent = TrFootprintComponent(
            sizePair.first, sizePair.second, move(footprintVec));

        TrData::m_buildingTypes[name] =
            make_unique<TrBuildingEntityType>(name, graphics, footprintComponent);
      }
    }

    // read Plants
    {
      io::CSVReader<4> in("data/Entities - Plants.csv");
      in.read_header(io::ignore_extra_column, "name", "color", "size",
                     "footprint");

      while (in.read_row(name, color, size, footprint)) {
        // do stuff with the data
        TrGraphicsComponent graphics =
            TrGraphicsComponent(parseColor(color));
        pair<int, int> footprintSize = parseSize(size);

        auto footprintComponent = TrFootprintComponent(
            footprintSize.first, footprintSize.second);

        TrData::m_plantTypes[name] =
            std::make_unique<TrPlantEntityType>(name, graphics, footprintComponent);
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
        TrData::m_items[name] = std::make_unique<TrItem>(name);
      }
    }

    // read Manufactured Products
    {
      io::CSVReader<3> in("data/Items - Manufactured Products.csv");
      in.read_header(io::ignore_extra_column, "name", "tags", "attributes");

      while (in.read_row(name, tags, attributes)) {
        // do stuff with the data
        TrData::m_items[name] = std::make_unique<TrItem>(name);
      }
    }

    // read Produce
    {
      io::CSVReader<3> in("data/Items - Produce.csv");
      in.read_header(io::ignore_extra_column, "name", "tags", "attributes");

      while (in.read_row(name, tags, attributes)) {
        // do stuff with the data
        TrData::m_items[name] = std::make_unique<TrItem>(name);
      }
    }

    // read Raw Materials
    {
      io::CSVReader<3> in("data/Items - Raw Materials.csv");
      in.read_header(io::ignore_extra_column, "name", "tags", "attributes");

      while (in.read_row(name, tags, attributes)) {
        // do stuff with the data
        TrData::m_items[name] = std::make_unique<TrItem>(name);
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
      }
    }
  }
}

void TrData::deleteData() {}
