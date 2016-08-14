#ifndef FORCE_H_
#define FORCE_H_

#include <vector>
#include "vect.h"
#include "params.h"

class force {
  public:
    virtual std::vector<vect> get_velocity_increment(__attribute__((unused)) const parameters& params, __attribute__((unused)) const std::vector<vect>& position) {
        return std::vector<vect>();
    }
};

#endif // FORCE_H_
