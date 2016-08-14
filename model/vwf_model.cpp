#include "vwf_model.h"
#include <fstream>
#include <cassert>

vwf_model::vwf_model() {}

void vwf_model::read_positions(std::string filename) {
    std::ifstream fin(filename.c_str());
    position.resize(params.N);
    for (int i = 0; i < params.N; i++) {
        fin >> position[i].x >> position[i].y >> position[i].z;
    }
    fin.close();
    recalc_mass_center();
}

void vwf_model::recalc_mass_center() {
    mass_center = vect();
    for (int i = 0; i < params.N; i++)
        mass_center = mass_center + position[i];
    mass_center = (1.0 / params.N) * mass_center;
}


// Checks whether vect coordinates are not nan
static void verify_vect(const vect& v) {
    assert(v.x == v.x);
    assert(v.y == v.y);
    assert(v.z == v.z);
}

void vwf_model::iterate() {
    std::vector<vect> velocities(params.N);
    for (auto& f : forces) {
        std::vector<vect> deltas = f->get_velocity_increment(params, position);
        for (int i = 0; i < params.N; i++) {
            verify_vect(deltas[i]);
            velocities[i] = velocities[i] + deltas[i];
        }
    }
    for (int i = 0; i < params.N; i++) {
        position[i] = position[i] + params.dt * velocities[i];
    }
    recalc_mass_center();
}

void vwf_model::write_to_file(std::ofstream & fout, int it) {
    fout << std::endl << "* " << it << std::endl;
    for (int i = 0; i < params.N; i++) {
        fout << position[i].x << " " << position[i].y << " " << position[i].z << std::endl;
    }
}

void vwf_model::write_pair_forces(std::ofstream &fout, int it)
{
    fout << std::endl << "* " << it << std::endl;
    for (int i = 0; i < params.N - 1; i++) {
        fout << pair_forces[i] << std::endl;
    }
}
