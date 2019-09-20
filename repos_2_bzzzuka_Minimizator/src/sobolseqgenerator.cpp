// Frances Y. Kuo
//
// Email: <f.kuo@unsw.edu.au>
// School of Mathematics and Statistics
// University of New South Wales
// Sydney NSW 2052, Australia
//
// Last updated: 21 October 2008
//
//   You may incorporate this source code into your own program
//   provided that you
//   1) acknowledge the copyright owner in your program and publication
//   2) notify the copyright owner by email
//   3) offer feedback regarding your experience with different direction numbers
//
//
// -----------------------------------------------------------------------------
// Licence pertaining to sobol.cc and the accompanying sets of direction numbers
// -----------------------------------------------------------------------------
// Copyright (c) 2008, Frances Y. Kuo and Stephen Joe
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:
//
//     * Redistributions of source code must retain the above copyright
//       notice, this list of conditions and the following disclaimer.
//
//     * Redistributions in binary form must reproduce the above copyright
//       notice, this list of conditions and the following disclaimer in the
//       documentation and/or other materials provided with the distribution.
//
//     * Neither the names of the copyright holders nor the names of the
//       University of New South Wales and the University of Waikato
//       and its contributors may be used to endorse or promote products derived
//       from this software without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS ``AS IS'' AND ANY
// EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
// WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
// DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDERS BE LIABLE FOR ANY
// DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
// (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
// LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
// ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
// (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
// SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
// -----------------------------------------------------------------------------

#include "sobolseqgenerator.h"

int SobolSeqGenerator::Init(uint32_t N_, uint32_t D_, std::string dir_file){
    int i,j,k;
    N = N_;
    D = D_;
    current_point_number = -1;

    std::ifstream infile(dir_file,std::ios::in);
    if (!infile) {
        std::cout << "Input file containing direction numbers cannot be found!" << std::endl;
        exit(1);
    }

    L = (unsigned)ceil(log((double)N)/log(2.0));
    for (i = 0; i < D; i++){
        V.emplace_back(L + 1);
        for (j = 0; j <= L; j++){
            V[i].push_back(0);
        }
    }

    char buffer[1000];
    infile.getline(buffer,1000,'\n');

    for (i = 1; i < D; i++){
        // Read parameters from file
        unsigned s, a;
        infile >> s >> s >> a;
        std::vector<unsigned> m;
        m.push_back(1);
        for (j = 1; j <= s; j++){
            unsigned m_;
            infile >> m_;
            m.push_back(m_);
        }

        // Compute direction numbers
        if (L <= s){
            for (j = 1; j <= L; j++){
                V[i][j] = m[j] << (32 - j);
            }
        }
        else {
            for (j = 1; j <= s; j++){
                V[i][j] = m[j] << (32 - j);
            }
            for (j = s + 1; j <= L; j++) {
                V[i][j] = V[i][j - s] ^ (V[i][j - s] >> s);
                for (k = 1; k <= s - 1; k++)
                    V[i][j] ^= (((a >> (s - 1 - k)) & 1) * V[i][j - k]);
            }
        }
    }
    infile.close();
    return 0;
}

PointReal SobolSeqGenerator::GeneratePoint(){
    if (N == 0 || D == 0){
        std::cout << "Wrong initialization! N = " << N << ", D = " << D << std::endl;
        return PointReal();
    }

    if(current_point_number == N - 1){
        std::cout << "All " << N << " points are already generated!" << std::endl;
        return PointReal();
    }

    current_point_number++;

    if(current_point_number == 0){
        //The 0-th point is (0, 0, ..., 0)
        last_generated_point = PointUnsigned(D, std::vector<unsigned>(D, 0));
        return PointReal(D, std::vector<Real>(D, 0));
    }

    PointUnsigned temp_point = PointUnsigned(D);

    C = 1;
    unsigned value = current_point_number - 1;
    while (value & 1) {
        value >>= 1;
        C++;
    }

    // Compute first dimension
    int v = 1 << (32 - C);
    last_generated_point.coordinate[0] = last_generated_point.coordinate[0] ^ v;

    // Compute the remaining dimensions
    for (int i = 1; i < D; i++){
        last_generated_point.coordinate[i] = last_generated_point.coordinate[i] ^ V[i][C];
    }

    PointReal final_result = PointReal(D);
    for (int i = 0; i < D; i++){
        final_result.coordinate[i] = (Real) last_generated_point.coordinate[i] / std::pow(Real(2.0), 32);
    }

    return final_result;
}
