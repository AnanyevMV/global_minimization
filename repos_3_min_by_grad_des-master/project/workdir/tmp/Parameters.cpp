#include "Data.h"
#include "Math.h"
#include "Methods.h"

struct Parameters {
	Vector adam_params;
	Vector rmsprop_params;

	Real adam_beta1, adam_beta2, adam_gamma; //!!!@ Заменить на ссылки (?) @!!!
	Real rmsprop_beta, rmsprop_gamma;		 //

	Parameters() {
		adam_params = { 0.9, 0.99, 0.1 };
		rmsprop_params = { 0.9, 0.1 };

		this->adam_beta1 = 0.9;
		this->adam_beta2 = 0.99;
		this->adam_gamma = 0.1;
		this->rmsprop_beta = 0.9;
		this->rmsprop_gamma = 0.1;
	}

	Vector nGrad(IterationData(*method)(Function f, Vector startPoint), Vector params, Function f, const Real h) {
		int n = (int)params.size();
		Vector grad(n);

		for (int i = 0; i < n; i++) {
			params[i] = params[i] + h;
			grad[i] = sNorm(method, f);
			params[i] = params[i] - 2 * h;
			grad[i] = (grad[i] - sNorm(method, f))/2*h;
			params[i] = params[i] + h;
		}
		return grad;
	}

	Real sNorm(IterationData(*method)(Function f, Vector startPoint), Function f) {		
        Real snorm = 0;
		snorm = snorm + norm(method(f, x1, 0.000001).x_curr - x_rmin);
	}

	void calcParams() {
		adam_params = adam_params - nGrad(adam, adam_params, f25);
		rmsprop_params = rmsprop_params - nGrad(rmsprop, rmsprop_params, f25);
	}
};
