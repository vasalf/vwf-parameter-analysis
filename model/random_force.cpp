#include "random_force.h"
#include "definition.h"
#include <fstream>
#include <cassert>
#include<iostream>




random_force::random_force(unsigned seed) : generator(seed), distribution(0.0, 1.0)
{
}


random_force::~random_force()
{
}

std::vector<vect> random_force::get_velocity_increment(const parameters& params, const std::vector<vect>& position)
{
	std::vector<vect> res;
    vect v;
#ifdef TEST_MODE
        std::ifstream ifstr;
        ifstr.open("./normal_distr_500k.txt");
        assert(ifstr.is_open());
        double dbuf;
        for (int i = 0; i < position.size(); ++i)
        {
	        v = vect(0, 0, 0);
            ifstr>>dbuf;
            v.x += dbuf*params.mno;
            ifstr>>dbuf;
            v.y += dbuf*params.mno;
            ifstr>>dbuf;
            v.z += dbuf*params.mno;
            res.push_back((1.0 / params.gamma)*v);
            assert(ifstr.good());
        }
        ifstr.close();
#else // TEST_MODE
	for (int i = 0; i < position.size(); ++i)
	{
	    v = vect(0, 0, 0);
		v.x += distribution(generator)*params.mno;
		v.y += distribution(generator)*params.mno;
		v.z += distribution(generator)*params.mno;
		res.push_back((1.0 / params.gamma)*v);
	}
#endif //TEST_MODE
	return res;
}
