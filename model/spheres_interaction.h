#ifndef SPHERES_INTERACTION_H_
#define SPHERES_INTERACTION_H_

#include "force.h"
#include "params.h"
#include <random>
#include <tuple>

class spheres_interaction_force : public force {
    std::vector<double>& pair_forces;
  public:
    spheres_interaction_force(std::vector<double>& pf);
    std::vector<vect> get_velocity_increment(const parameters& params, const std::vector<vect>& position);
    std::vector<std::tuple<vect, int, int> > get_close_forces(const parameters& params, const std::vector<vect>& position);
};

class spheres_interaction_force_unfold : public force {
    std::mt19937 eng;
    std::uniform_real_distribution<> dist;
    bool gen_event(double probability);
    std::vector<double>& pair_forces;
  public:
    std::vector<bool> is_unfolded;
    spheres_interaction_force_unfold(int n, int seed, std::vector<double>& pf);
    std::vector<vect> get_velocity_increment(const parameters& params, const std::vector<vect>& position);
};

#endif // SPHERES_INTERACTION_H_
