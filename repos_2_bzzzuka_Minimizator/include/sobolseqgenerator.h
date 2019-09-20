#pragma once

#include <string>
#include <cstdlib>
#include <cmath>
#include <iostream>
#include <iomanip>
#include <fstream>

#include "tmsnet.h"

typedef Point<uint32_t> PointUnsigned;

class SobolSeqGenerator : public TMSNet
{
public:
    int Init(uint32_t N_, uint32_t D_, std::string dir_file);
    PointReal GeneratePoint();

private:
    uint32_t N;
    uint32_t D;
    uint32_t current_point_number;
    uint32_t L;
    uint32_t C;

    //Params
    std::vector<std::vector<unsigned>> V;

    PointUnsigned last_generated_point;
};
