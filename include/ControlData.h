#pragma once

#include <vector>

using std::vector;

class ControlData
{
public:
  ControlData() : gamma(3, 1.0f), wp(3, 1.0f), exposure(1.0f),
  bp(0.0f), output_gamma(2.2f), grayscale(false), invert(true)
  {
    // gamma[0] = 1.90471932;
    // gamma[1] = 1.86499212;
    // gamma[2] = 1.6708782;
    // wp[0] = 0.7725490196078432;
    // wp[1] = 0.6588235294117647;
    // wp[2] = 0.5176470588235295;
// 1.90471932,  1.86499212,  1.6708782 |  0.52501174,  0.5361953 ,  0.59848765
  };

  vector<float> gamma;
  vector<float> wp;
  float exposure;
  float bp;
  float output_gamma;
  bool grayscale;
  bool invert;
};

// Ektar RGB gamma:
// 1.7966454 ,  1.75318663,  1.52816018
// inverse gamma
// 0.55659286,  0.57038993,  0.65438166
// bp:
// 1.0,   0.66,  0.52
//
// Portra160 RGB gamma:
// 1.90471932,  1.86499212,  1.6708782 |  0.52501174,  0.5361953 ,  0.59848765
//
// Portra400 RGB gamma:
//  1.84946131,  1.81149687,  1.61283489
