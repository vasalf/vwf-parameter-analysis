#ifndef MRUN_H_
#define MRUN_H_

#include "model/vwf_model.h"
#include "model/hydro.h"
#include "model/random_force.h"
#include "model/spheres_interaction.h"
#include "model/vect.h"
#include "params.h"
#include <string>
#include <random>

class run_results {
    std::vector<double> stats;
  public:
    run_results(std::vector<double> res);
    double rating() const;
};

class run_holder {
    std::random_device rd;
    std::vector<double> run_once(std::vector<int> params);
  public:
    run_holder();
    run_results run(int rep, std::vector<int> params);
};

#endif // MRUN_H_
