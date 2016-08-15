#include "params.h"
#include <vector>
#include <cstdio>
#include <cmath>

parameters get_parameters_structure(std::vector<int> v) {
    parameters params;
    
    int z;
    if (v[0] == 0)
        z = 10;
    else if (v[0] == 1)
        z = 20;
    else
        z = 50;

    int n = 40;

    int uspr = 50 * (v[2] + 1);

    double uvdw = 1 + 0.5 * v[3];

    int f0u = 1000 + 250 * v[4];

    double pu = 1e-3 + 3e-3 * v[5];

    double f0f = 1e6 + 5e5 * v[6];

    double pf = 0.1 + 0.225 * v[7];

    
    params.dt = 0.002;
    params.nfac = 1000;
    params.ns = 5000000;
    params.N = n;
    params.kT = 1.38 * 310;
    params.R = 5;
    params.d = 2 * params.R;
    params.uvdw = uvdw;
    params.uspr = uspr;
    params.alpha = 5.5;
    params.srat = 0 / 10000;
    params.erat = 300 / 10000;
    params.nu = 1;
    params.gamma = 6 * M_PI * params.nu * params.R;
    params.D = params.kT / params.gamma;
    params.pu = pu * 1e-4;
    params.pf = pf * 1e-4;
    params.dd = 5;
    params.f0u = f0u;
    params.f0f = f0f;
    params.mno = std::sqrt(2 * params.gamma * params.kT / params.dt);
    params.kspr = (params.uspr / (2 * params.R * params.R)) * params.kT;
    params.kvdw = params.uvdw * params.kT;

    char buf[100];

#define SUBST_NAME(a, format, ...)     \
    std::sprintf(buf, format, __VA_ARGS__); \
    a = buf;

    SUBST_NAME(params.x_file, "res/outp_x_%d.txt", z);
    SUBST_NAME(params.v_file, "res/outp_1_%d.txt", z);
    SUBST_NAME(params.srate_file, "res/outp_srate_%d.txt", z);

    return params;
}
