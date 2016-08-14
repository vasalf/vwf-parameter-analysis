#include <vector>
#include <string>
#include "vect.h"
#include "force.h"

class hydro: public force {
  public:
    std::vector<vect> get_velocity_increment(const parameters& params, const std::vector<vect>& position);
    hydro(vect & mc);
    vect & mass_center;
};

class hydro_vessel: public force {
    double start_x;
    double dx;
    double end_x;
    double max_srat;
    double vcs;
    std::vector<double> srat_at_x;
    std::vector<double> erat_at_x;
    vect & mass_center;
  public:
    hydro_vessel(vect & mc, std::string xs_filename, std::string srat_filename, std::string erat_filename);
    std::vector<vect> get_velocity_increment(const parameters& params, const std::vector<vect>& position);
};
