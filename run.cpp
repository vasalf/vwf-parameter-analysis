#include "model/vwf_model.h"
#include "model/hydro.h"
#include "model/random_force.h"
#include "model/spheres_interaction.h"
#include "model/vect.h"
#include "params.h"
#include "run.h"
#include <string>
#include <sstream>
#include <random>
#include <algorithm>

run_results::run_results(std::vector<double> res) {
    stats = res;
}

double run_results::rating() const {
    std::vector<double> local_maximums;
    for (int i = 1; i < (int)stats.size() - 1; i++) {
        if (stats[i] > stats[i - 1] && stats[i] > stats[i + 1])
            local_maximums.push_back(stats[i]);
    }
    int ans = 0;
    for (int i = 1; i < (int)local_maximums.size() - 1; i++) {
        ans += (local_maximums[i] > local_maximums[i - 1] && local_maximums[i] > local_maximums[i + 1]);
    }
    return ans;
}

run_holder::run_holder() {}

std::vector<double> run_holder::run_once(std::vector<int> params) {
    vwf_model model;
    model.params = get_parameters_structure(params);

    std::stringstream position_filename;
    position_filename << "initial_coords_" << model.params.N << ".txt";
    model.read_positions(position_filename.str());

    model.forces.emplace_back(new spheres_interaction_force_unfold(model.params.N, rd(), model.pair_forces));
    model.forces.emplace_back(new hydro_vessel(model.mass_center, model.params.x_file, model.params.srate_file, model.params.v_file));
    model.forces.emplace_back(new random_force(rd()));

    std::vector<double> ret;

    for (int s = 0; s < model.params.ns; s++) {
        model.iterate();
        if (s % model.params.nfac == 0) {
            ret.push_back(0);
            for (int i = 0; i < model.params.N; i++) {
                for (int j = 0; j < model.params.N; j++) {
                    ret.back() = std::max(ret.back(), vect(model.position[i], model.position[j]).length());
                }
            }
        }
    }

    return ret;
}

run_results run_holder::run(int rep, std::vector<int> params) {
    std::vector<double> ret;
    for (int i = 0; i < rep; i++) {
        std::vector<double> a = run_once(params);
        if (ret.empty())
            ret.resize(a.size());
        for (int j = 0; j < (int)a.size(); j++)
            ret[j] += a[j];
    }
    for (double& u : ret)
        u /= (double)rep;
    return run_results(ret);
}


