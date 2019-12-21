#pragma once
#include "Tools/math.hpp"
typedef struct Parameters
{
	Real Adam_beta1 = 0.8, Adam_beta2 = 0.55, Adam_gamma = 0.337;
	Real AdamW_beta1 = 0.91, AdamW_beta2 = 0.85, AdamW_gamma = 0.01;
	Real RmsProp_beta = 0.55, RmsProp_alpha = 0.75;
	Real Adagrad_beta = 0.94;
	Real AdaMax_beta1 = 0.94, AdaMax_beta2 = 0.94, AdaMax_gamma = 0.337;
	Real Adadelta_beta = 0.55;
	Real Nadam_beta1 = 0.55, Nadam_beta2 = 0.74, Nadam_gamma = 0.337;
	Real AMSGrad_beta1 = 0.89, AMSGrad_beta2 = 0.98, AMSGrad_gamma = 0.01;
	Real Nesterov_ro = 2.0, Nesterov_teta = 1.1, Nesterov_alfa = 1.0;
	
	/*
	old
	Real Adam_beta1 = 0.86296943, Adam_beta2 = 0.88791641, Adam_gamma = 0.07670128;
	Real RmsProp_beta = 0.93005474, RmsProp_alpha = 0.06376147;
	Real Adagrad_beta = 1.0;
	Real AdaMax_beta1 = 0.92096770, AdaMax_beta2 = 0.95563197, AdaMax_gamma = 0.18211471;
	Real Adadelta_beta = 0.999;
	Real Nadam_beta1 = 0.88441989, Nadam_beta2 = 0.88441989, Nadam_gamma = 0.08381000;
	Real AMSGrad_beta1 = 0.86296943, AMSGrad_beta2 = 0.88791641, AMSGrad_gamma = 0.07670128;
	Real Nesterov_ro = 2.0, Nesterov_teta = 1.1, Nesterov_alfa = 1.0;
	*/
} Params;

