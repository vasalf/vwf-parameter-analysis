#include "spheres_interaction.h"
#include "params.h"
#include <cassert>
#include <cmath>
#include <iostream>
#include "spheres_interaction.h"
#include "params.h"
#include <cassert>
#include <cmath>
#include <iostream>

spheres_interaction_force::spheres_interaction_force(std::vector<double>& pf) : pair_forces(pf) {}

std::vector<vect> spheres_interaction_force::get_velocity_increment(const parameters& params, const std::vector<vect>& position) {
    std::vector<vect> forces(params.N);
    pair_forces.resize(params.N);

    for (int i = 0; i < params.N; i++) {
        for (int j = i + 1; j < params.N; j++) {
            vect dv(position[j], position[i]);
            double l = dv.length();

            assert(l > 1e-9); // NEVER compare doubles on equality!
            if (j == i + 1 || l < params.d) {
                vect fpr = (params.kspr * (l - params.d) / l) * dv;
                forces[i] = forces[i] + (-1) * fpr;
                forces[j] = forces[j] + fpr;
                if(j == i + 1)
                    pair_forces[i] = fpr.length();
            } else if (params.d < l && l < 2 * params.d) {
                double u = std::exp(-((l - params.d) / params.d) * params.alpha);
                double u2 = 2 * params.alpha * params.kvdw * (u * (1 - u) / (l * params.d));

                vect fpr = u2 * dv;

                forces[i] = forces[i] + (-1) * fpr;
                forces[j] = forces[j] + fpr;
            }
        }
    }

    std::vector<vect> ret(params.N);
    for (int i = 0; i < params.N; i++) {
        ret[i] = (1 / params.gamma) * forces[i];
    }

    return ret;
}

std::vector<std::tuple<vect, int, int> > spheres_interaction_force::get_close_forces(const parameters& params, const std::vector<vect>& position) {
    std::vector<std::tuple<vect, int, int> > ret;

    for (int i = 0; i < params.N; i++) {
        for (int j = i + 2; j < params.N; j++) {
            vect dv(position[j], position[i]);
            double l = dv.length();

            assert(l > 1e-9); // NEVER compare doubles on equality!
            if (params.d < l && l < 2 * params.d) {
                double u = std::exp(-((l - params.d) / params.d) * params.alpha);
                double u2 = 2 * params.alpha * params.kvdw * (u * (1 - u) / (l * params.d));

                vect fpr = u2 * dv;

                ret.push_back(std::make_tuple(fpr, i, j));
            }
        }
    }

    return ret;
}

spheres_interaction_force_unfold::spheres_interaction_force_unfold(int n, int seed, std::vector<double>& pf) :
    eng(seed), dist(0, 1), is_unfolded(2 * n, false), pair_forces(pf) {}

bool spheres_interaction_force_unfold::gen_event(double probability) {
    return dist(eng) < probability; 
}

std::vector<vect> spheres_interaction_force_unfold::get_velocity_increment(const parameters& params, const std::vector<vect>& position) {
    std::vector<vect> forces(params.N);
    pair_forces.resize(params.N);
    std::vector<bool> new_is_unfolded(2 * params.N);

    for (int i = 0; i < params.N; i++) {
        for (int j = i + 1; j < params.N; j++) {
            vect dv(position[j], position[i]);
            double l = dv.length();

            assert(l > 1e-9); // NEVER compare doubles on equality!
            if (j == i + 1 || l < params.d) {
                double cur_d = params.d;
                if (is_unfolded[2 * i + 1])
                    cur_d += params.dd;
                if (is_unfolded[2 * j])
                    cur_d += params.dd; 

                vect fpr = (params.kspr * (l - cur_d) / l) * dv;

                forces[i] = forces[i] + (-1) * fpr;
                forces[j] = forces[j] + fpr;
                if(j == i + 1)
                    pair_forces[i] = fpr.length();
                
                if (!is_unfolded[2 * i + 1]) {
                    new_is_unfolded[2 * i + 1] = gen_event(params.pu * params.dt * std::exp(fpr.length() / params.f0u));
                    if (new_is_unfolded[2 * i + 1]) {
                        std::cerr << "unfold " << fpr.length() << std::endl;
                    }
                    new_is_unfolded[2 * j] = gen_event(params.pu * params.dt * std::exp(fpr.length() / params.f0u));
                    if (new_is_unfolded[2 * j]) {
                        std::cerr << "unfold " << fpr.length() << std::endl;
                    }
                } else {
                    new_is_unfolded[2 * i + 1] = gen_event(1 - params.pf * params.dt * std::exp(-std::pow(fpr.length(), 2) / params.f0f));
                    if (!new_is_unfolded[2 * i + 1]) {
                        std::cerr << "refold " << fpr.length() << std::endl;
                    }
                    new_is_unfolded[2 * j] = gen_event(1 - params.pf * params.dt * std::exp(-std::pow(fpr.length(), 2) / params.f0f));
                    if (!new_is_unfolded[2 * j])
                        std::cerr << "refold " << fpr.length() << std::endl;
                }

            } else if (params.d < l && l < 2 * params.d) {
                double cur_d = params.d;
                
                double u = std::exp(-((l - cur_d) / cur_d) * params.alpha);
                double u2 = 2 * params.alpha * params.kvdw * (u * (1 - u) / (l * cur_d));

                vect fpr = u2 * dv;

                forces[i] = forces[i] + (-1) * fpr;
                forces[j] = forces[j] + fpr;
            }
        }
    }

    is_unfolded = new_is_unfolded;

    std::vector<vect> ret(params.N);
    for (int i = 0; i < params.N; i++) {
        ret[i] = (1 / params.gamma) * forces[i];
    }
    
    return ret;
}

