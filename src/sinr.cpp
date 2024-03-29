#include <cstdlib>
#include <ctime>
#include <iostream>
#include <sstream>
#include <vector>
#include <algorithm>
#include <memory>

#include "sinrlib.h"
#include "naive.h"
#include "antibackoff.h"
#include "backoff.h"
#include "backoffack.h"
#include "densityknown.h"
#include "densityunknown.h"

double avg(const std::vector<int> results)
{
    double avg = 0;

    for (std::vector<int>::const_iterator it = results.begin();
            it != results.end(); it++)
    {
        avg += *it;
    }

    return avg / results.size();
}

double stdv(const std::vector<int> results)
{
    double stdv = 0;
    double a = avg(results);

    for (std::vector<int>::const_iterator it = results.begin();
            it != results.end(); it++)
    {
        stdv += (a - *it) * (a - *it);
    }

    return std::sqrt(stdv / results.size());
}

int main(int argc, char **argv)
{
    int tries, N_start = 200, N_end = 200, N_step = 1, S_start = 2, S_end = 2, S_step = 3;
    int d = 10, dprim = 10;
    double C = 1, e = .2;
    char sep;
    std::vector<sinr::algorithm*> algs;

    if (argc < 5)
    {
        std::cout << "Usage: sinr dir tries N_start,N_end,N_step S_start,S_end,S_step\n";
        return 1;
    }

    std::istringstream(argv[2]) >> tries;
    std::istringstream(argv[3]) >> N_start >> sep >> N_end >> sep >> N_step;
    std::istringstream(argv[4]) >> S_start >> sep >> S_end >> sep >> S_step;

    std::srand(std::time(0));
    //std::srand(0);

    //algs.push_back(new naive_algorithm());
    algs.push_back(new backoffack_algorithm());
    //algs.push_back(new backoff_algorithm());
    //algs.push_back(new antibackoff_algorithm(e));
    algs.push_back(new density_known_algorithm(e, C, d));
    //algs.push_back(new density_unknown_algorithm(e, C, d, dprim));

    for (int N = N_start; N <= N_end; N += N_step)
    {
        for (int S = S_start; S <= S_end; S += S_step)
        {
            std::vector<std::vector<int> > results;
            std::vector<int> diameters;
            results.resize(algs.size());

            for (int t = 1; t <= tries; t++)
            {
                unsigned alg_index = 0;
                sinr::model model;
                std::ostringstream model_file;

                model_file << argv[1] << "/" << N << "/" << S << "/" << t;
                model.load(model_file.str().c_str());

                diameters.push_back(model.diameter());
                
                while (alg_index < algs.size())
                {
                    int result;

                    sinr::simulation sim(model);
                    result = sim.run(*algs[alg_index], 100000);
                    
                    if (result < 0)
                    {
                        std::cout << "F" << std::flush;
                    }
                    else
                    {
                        std::cout << "." << std::flush;
                        results[alg_index].push_back(result);
                        alg_index++;
                    }
                }
                std::cout << ",";
            }
            
            std::cout << '\n'; 

            for (unsigned i = 0; i < results.size(); i++)
            {
                double avg_res = avg(results[i]);
                double res_std = stdv(results[i]);
                double avg_diam = avg(diameters); 
                std::cout << algs[i]->name() << '\t' << N << '\t' << S << '\t'
                    << avg_res << '\t' << res_std << '\t'
                    << avg_diam << '\t' << avg_res / avg_diam << std::endl;
            }
        }
    }

    return 0;
}
