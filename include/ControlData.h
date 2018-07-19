#pragma once

#include <vector>

using std::vector;

class ControlData
{
public:
  ControlData() : gamma(3, 1.0f), wp(3, 1.0f), exposure(1.0f),
  bp(0.0f), output_gamma(1.0f), grayscale(false), invert(false) {};

  vector<float> gamma;
  vector<float> wp;
  float exposure;
  float bp;
  float output_gamma;
  bool grayscale;
  bool invert;
};
