#include <fstream>
#include "ConfigActions.h"
#include "Parameters.h"

#include "ConfigMain.h"

namespace operateconfig { class CConfig; }
namespace operateconfig { class File_not_found; }
namespace operateconfig { class Key_not_found; }

Real Parameters::Adam_beta1;
Real Parameters::Adam_beta2;
Real Parameters::Adam_gamma;

Real Parameters::RmsProp_beta;
Real Parameters::RmsProp_alpha;

Real Parameters::Adagrad_beta;

Real Parameters::AdaMax_beta1;
Real Parameters::AdaMax_beta2;
Real Parameters::AdaMax_gamma;

Real Parameters::Adadelta_beta;

Real Parameters::Nadam_beta1;
Real Parameters::Nadam_beta2;
Real Parameters::Nadam_gamma;

Real Parameters::AMSGrad_beta1;
Real Parameters::AMSGrad_beta2;
Real Parameters::AMSGrad_gamma;

void ReadConfig() {

	ConfigHandle.init("config.txt");

  Parameters::Adam_beta1 = ConfigHandle.read("Adam_beta1", Parameters::Adam_beta1);
  Parameters::Adam_beta2 = ConfigHandle.read("Adam_beta2", Parameters::Adam_beta2);
  Parameters::Adam_gamma = ConfigHandle.read("Adam_gamma", Parameters::Adam_gamma);

  Parameters::RmsProp_beta = ConfigHandle.read("RmsProp_beta", Parameters::RmsProp_beta);
  Parameters::RmsProp_alpha = ConfigHandle.read("RmsProp_alpha", Parameters::RmsProp_alpha);

  Parameters::Adagrad_beta = ConfigHandle.read("Adagrad_beta", Parameters::Adagrad_beta);

  Parameters::AdaMax_beta1 = ConfigHandle.read("AdaMax_beta1", Parameters::AdaMax_beta1);
  Parameters::AdaMax_beta2 = ConfigHandle.read("AdaMax_beta2", Parameters::AdaMax_beta2);
  Parameters::AdaMax_gamma = ConfigHandle.read("AdaMax_gamma", Parameters::AdaMax_gamma);

  Parameters::Adadelta_beta = ConfigHandle.read("Adadelta_beta", Parameters::Adadelta_beta);

  Parameters::Nadam_beta1 = ConfigHandle.read("Nadam_beta1", Parameters::Nadam_beta1);
  Parameters::Nadam_beta2 = ConfigHandle.read("Nadam_beta1", Parameters::Nadam_beta2);
  Parameters::Nadam_gamma = ConfigHandle.read("Nadam_beta1", Parameters::Nadam_gamma);

  Parameters::AMSGrad_beta1 = ConfigHandle.read("AMSGrad_beta1", Parameters::AMSGrad_beta1);
  Parameters::AMSGrad_beta2 = ConfigHandle.read("AMSGrad_beta2", Parameters::AMSGrad_beta2);
  Parameters::AMSGrad_gamma = ConfigHandle.read("AMSGrad_gamma", Parameters::AMSGrad_gamma);
}


void CreateConfig() {
  ofstream fout;
  fout.open("config.txt");

  fout << "Adam_beta1 = 0.84374857" << endl;
  fout << "Adam_beta2 = 0.89508866" << endl;
  fout << "Adam_gamma = 0.03470107" << endl;

  fout << "RmsProp_alpha = 0.063761147" << endl;
  fout << "RmsProp_beta = 0.93005474" << endl;

  fout << "Adagrad_beta = 0.9" << endl;

  fout << "AdaMax_beta1 = 0.9209677" << endl;
  fout << "AdaMax_beta2 = 0.95563197" << endl;
  fout << "AdaMax_gamma = 0.18211471" << endl;

  fout << "Adadelta_beta = 0.9" << endl;

  fout << "Nadam_beta1 = 0.88441989" << endl;
  fout << "Nadam_beta2 = 0.88441989" << endl;
  fout << "Nadam_gamma = 0.08381" << endl;

  fout << "AMSGrad_beta1 = 0.84374857" << endl;
  fout << "AMSGrad_beta2 = 0.895508866" << endl;
  fout << "AMSGrad_gamma = 0.03470107" << endl;

  fout.close();
}
