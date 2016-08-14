#include "hydro.h"
#include <iostream>
#include <fstream>
#include <cassert>
#include <algorithm>

hydro::hydro(vect & mc) : mass_center(mc) {}

std::vector<vect> hydro::get_velocity_increment(const parameters& params, const std::vector<vect>& position){	
	
	std::vector <vect> dv (position.size());
	for (int i = 0; i < (int)position.size(); i++){
		double dx = position[i].x - mass_center.x;
		double dy = position[i].y - mass_center.y;
		double dz = position[i].z - mass_center.z;
		
		dv[i].x = dx * params.erat + dz * params.srat;
		dv[i].y = dy * params.erat * (-1);
		dv[i].z = 0;

	}
	return dv;
}

hydro_vessel::hydro_vessel(vect & mc, std::string xs_filename, std::string srat_filename, std::string erat_filename) : mass_center(mc) {
    std::cout << xs_filename << std::endl; 
    std::ifstream xfile;
    xfile.open(xs_filename);
    assert(xfile.is_open());
    
    xfile >> start_x >> end_x;
    start_x *= 1e8;
    end_x *= 1e8;
    dx = end_x - start_x;
    while (xfile >> end_x)
        end_x *= 1e8;

    xfile.close();

    std::ifstream srat_file;
    srat_file.open(srat_filename);
    assert(srat_file.is_open());

    double t;
    while (srat_file >> t)
        srat_at_x.push_back(t / 1e4);
    
    srat_file.close();

    max_srat = srat_at_x[srat_at_x.size() - 2];

    std::ifstream erat_file;
    erat_file.open(erat_filename);
    assert(erat_file.is_open());

    double prev;
    erat_file >> prev;
    prev *= 1e4;
    vcs = prev;
    while (erat_file >> t) {
        t *= 1e4;
        erat_at_x.push_back((t - prev) / dx);
        prev = t;
    }
    erat_at_x.push_back(0);

    erat_file.close();

    assert(srat_at_x.size() == erat_at_x.size());
}


std::vector<vect> hydro_vessel::get_velocity_increment(const parameters& params, const std::vector<vect>& position) {
    double erat, srat;
    std::vector<vect> ret(params.N);
  
    int j = (mass_center.x) / dx; 
    j = std::min(j, (int)srat_at_x.size() - 1);

    erat = erat_at_x[j];
    srat = srat_at_x[j];
    //std::cout << srat << " " << erat << std::endl;

    if (mass_center.x - start_x > end_x)
        srat = max_srat;

    vcs += vcs * erat * params.dt;

    for (int i = 0; i < params.N; i++) {
        vect dv(mass_center, position[i]);
        ret[i].x = vcs + dv.x * erat + dv.z * srat;
        ret[i].y = -dv.y * erat;
    }

    return ret;
}
