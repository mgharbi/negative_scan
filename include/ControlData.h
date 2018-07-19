#pragma once

#include <vector>

using std::vector;

class ControlData
{
public:
  ControlData() : gamma(3, 1.0f), wp(3, 1.0f) {};

  vector<float> gamma;
  vector<float> wp;
};
