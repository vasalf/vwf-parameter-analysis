#ifndef PARAMS_H_
#define PARAMS_H_

#include <string>

struct parameters {
    // Solver
    double dt;         // Time step
    long long ns;      // number of iterations to calculate
    long long nfac;    // data saving rate (to file)

    // Particles and environment
    int N;             // Number of interacting particles
    double kT;         // Thermal energy at 310 K or 37 C
    double R;          // Radius of spheres in 10^-8 meter
    double d;          // 2 * R

    // Interaction between spheres
    double uvdw;       // sphere-sphere interaction potential depth
    double uspr;       // stiffness of springs connecting spheres
    double alpha;      // How fast do forces decrease with distance between 2 interacting spheres

    // Hydrodynamics
    double srat;       // shear rate
    double erat;       // elongation rate

    // Interaction of sphere with solution (blood plasma)
    double nu;         // Viscosity of blood (plasma) at 37 C
    double gamma;      // Friction coefficient for sphere (dimer) in plasma
    double D;          // Diffusion constant

    // Folding
    double pu;         // unfolding rate constant
    double pf;         // folding rate constant
    double dd;         // folding string relaxation length for one domain
    double f0u;        // unfolding force scale
    double f0f;        // folding force scale

    double mno;
    double kspr;
    double kvdw;
// for flow chamber hydridynamics
    std::string x_file;
    std::string v_file;
    std::string srate_file;

    parameters();
    void read_from_file(std::string filename);
    void write_to_file(std::string filename);
};

#define RATE_NORM 10000

#endif // PARAMS_H_
