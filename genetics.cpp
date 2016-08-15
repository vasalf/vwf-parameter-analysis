#include "genetics.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <algorithm>
#include <unistd.h>
#include <wait.h>
#include <sched.h>

static std::uniform_int_distribution<> parameter_dists[] = {
    std::uniform_int_distribution<>(0, limits[0] - 1),
    std::uniform_int_distribution<>(0, limits[1] - 1),
    std::uniform_int_distribution<>(0, limits[2] - 1),
    std::uniform_int_distribution<>(0, limits[3] - 1),
    std::uniform_int_distribution<>(0, limits[4] - 1),
    std::uniform_int_distribution<>(0, limits[5] - 1),
    std::uniform_int_distribution<>(0, limits[6] - 1),
    std::uniform_int_distribution<>(0, limits[7] - 1)
};

genetics::genetics(int _psz, int _rep, int _seed, int _threads, double _mp) : rnd(_seed), crossingover_dist(0, NPARAM - 1), mutation_dist(0, 1) {
    population_size = _psz;
    rep = _rep;
    mutation_probability = _mp;
    threads = _threads;
}

genetics::individual::individual(std::vector<int> _vec) : vec(_vec) {}

genetics::individual genetics::interbreeding(const genetics::individual& i1, const genetics::individual& i2) {
    int last_from_first = crossingover_dist(rnd);
    std::vector<int> res(NPARAM);
    for (int i = 0; i <= last_from_first; i++)
        res[i] = i1.vec[i];
    for (int i = last_from_first + 1; i < NPARAM; i++)
        res[i] = i2.vec[i];
    for (int mutate = 0; mutate < NPARAM; mutate++) {
        if (mutation_dist(rnd) < mutation_probability) {
            int replacement = parameter_dists[mutate](rnd);
            res[mutate] = replacement;
        }
    }
    return individual(res);
}

std::vector<run_results> genetics::threaded_run(std::vector<genetics::individual> &population) {
    std::vector<run_results> values;
    int st = 0;
    threads = std::min(threads, population_size);
    for (int th_no = 0; th_no < threads; th_no++) {
        if (fork() == 0) {
            run_holder rh;
            std::stringstream fname;
            fname << "/tmp/vwf_thread" << th_no;
            
            std::ofstream fout(fname.str());
            fout.precision(16);

            for (int i = st; i < std::min(population_size, st + population_size / threads); i++) {
                run_results rr = rh.run(rep, population[i].vec);
                fout << rr.stats.size() << std::endl;
                for (double u : rr.stats)
                    fout << u << " ";
                fout << std::endl;
            }
            
            fout.close();
            exit(0);
        }
        st += population_size / threads;
    }

    int status;
    for (int i = 0; i < threads; i++) {
        waitpid(-1, &status, 0);
    }
    
    st = 0;
    for (int th_no = 0; th_no < threads; th_no++) {
        std::stringstream fname;
        fname << "/tmp/vwf_thread" << th_no;
        std::ifstream fin(fname.str());

        for (int i = st; i < std::min(population_size, st + population_size / threads); i++) {
            int x;
            fin >> x;

            values.push_back(run_results(std::vector<double>(x, 0)));
            for (int j = 0; j < x; j++)
                fin >> values.back().stats[j];
        }
        st += population_size / threads;
    }
    return values;
}

void genetics::execute() {
    std::vector<individual> population(population_size);
    for (int i = 0; i < population_size; i++) {
        population[i].vec.resize(NPARAM);
        for (int j = 0; j < NPARAM; j++) {
            population[i].vec[j] = parameter_dists[j](rnd); 
        }
    }
    for (int generation = 1; ;generation++) {
        std::vector<run_results> vals = threaded_run(population);
        
        std::cout << "Generation #" << generation << ":" << std::endl;
        double mn = 1791791791;
        for (int i = 0; i < population_size; i++)
            mn = std::min(mn, vals[i].rating());
        std::cout << "best: " << mn << std::endl;
        std::cout << std::endl;

        std::vector<int> indices(population_size);
        for (int i = 0; i < population_size; i++)
            indices[i] = 0;
        std::sort(indices.begin(), indices.end(), [&](const int& i, const int& j) -> bool {return vals[i].rating() < vals[j].rating();});

        std::vector<individual> new_generation; // Here comes the young, the new generation
        while ((int)new_generation.size() < population_size) {
            for (int i = 0; i < population_size && (int)new_generation.size() < population_size; i++) {
                for (int j = 0; j < i && (int)new_generation.size() < population_size; j++) {
                    if (mutation_dist(rnd) * population_size < 4) {
                        new_generation.push_back(interbreeding(population[j], population[i]));
                    }
                }
            }
        }

        population = new_generation;
    }
}
