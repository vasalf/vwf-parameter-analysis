#ifndef MGENETICS_H_
#define MGENETICS_H_

#include "run.h"
#include <random>

static const int NPARAM = 8;
static const int limits[] = {3, 1, 4, 5, 3, 4, 4, 5};

class genetics {
    int rep;
    int threads;
    int individuals;
    std::mt19937 rnd;
    int population_size;
    double mutation_probability;
    std::uniform_int_distribution<> crossingover_dist;
    std::uniform_real_distribution<> mutation_dist;
  public: 
    class individual {
      public:
        std::vector<int> vec;
        individual() {}
        individual(std::vector<int> _vec);
    };
  private:
    individual interbreeding(const individual& i1, const individual& i2);
    std::vector<run_results> threaded_run(std::vector<individual> &population);
  public:
    genetics(int _psz, int _rep, int _seed, int _threads, double _mp);
    void execute();
};

#endif //MGENETICS_H_
