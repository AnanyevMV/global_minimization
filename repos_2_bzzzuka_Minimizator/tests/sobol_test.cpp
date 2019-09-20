#define CATCH_CONFIG_MAIN

#include "catch.hpp"
#include "sobolseqgenerator.h"
#include <algorithm>

bool test_repeat_onecoordinate(uint32_t N_, std::vector<Real> points){
    bool res = true;
    for (int i = 0; i < N_ - 1; i++){
        if (points[i] == points[i + 1]){
            res = false;
        }
    }
    return res;
}

bool test_repeat_onedim(uint32_t N_, uint32_t D_, std::string dir_file){
    std::vector<std::vector<Real>> points(D_, std::vector<Real>(N_, 0));
    SobolSeqGenerator net;

    net.Init(N_, D_, dir_file);

    // Storing points in columns instead of rows
    for (int i = 0; i < N_; i++) {
        auto net_point = net.GeneratePoint().coordinate;
        for (int j = 0; j < D_; j++) {
            points[j][i] = net_point[j];
        }
    }

    bool res = true;
    for (int i = 0; i < D_; i++){
        // Sort each row (vector of i-th coordinates of each point)
        sort(points[i].begin(), points[i].end());
        res = test_repeat_onecoordinate(N_,points[i]);
    }
    return res;
}

bool test_repeat_alldims(uint32_t N_, uint32_t D_, std::string dir_file) {
    bool res = true;
    for (int i = 1; i <= D_; i++) {
        if (!(test_repeat_onedim(N_, i, dir_file)){
            res = false;
        }
    }
    return res;
}

TEST_CASE( "Sobol sequence generator basic test (pass)", "[single-file]"){
    REQUIRE(test_repeat_alldims(2048,1111,"new-joe-kuo-6.21201.txt"))
}