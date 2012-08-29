#include <iostream>
#include <sstream>
#include <vector>
#include <algorithm>

#include "sinrlib.h"
#include "naive.h"
#include "backoff.h"
#include "backoffack.h"

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
    int tries, N_start = 1000, N_end = 1000, N_step = 1, S_start = 6, S_end = 6, S_step = 3;
    int C = 1, d = 5;
    double e = .2;
    //std::vector<sinr::algorithm> alg;

    if (argc < 3)
    {
        std::cout << "Usage: sinr tries N_start,N_end,N_step S_start,S_end,S_step\n";
        return 1;
    }


    for (int N = N_start; N <= N_end; N += N_step)
    {
        for (int S = S_start; S <= S_end; S += S_step)
        {
            std::vector<int> results;
            results.reserve(tries);

            std::istringstream(argv[1]) >> tries;
            for (; tries > 0; tries--)
            {
                sinr::uniform_model model(2.5, 1, 1 - e);
                model.generate(N, S);
                sinr::simulation sim(model);
                backoffack_algorithm alg;
                int result;
                result = sim.run(alg);
                if (result < 0)
                    std::cout << "F" << std::flush;
                else
                {
                    std::cout << "." << std::flush;
                    results.push_back(result);
                }
            }
            std::cout << '\n' << N << ' ' << S << ' '
                << avg(results) << ' ' << stdv(results) << std::endl;
        }
    }

    return 0;
}

/*def main():

    alg = [ #algorithms.DensityUnknownAlgorithm(config, e, C, d, d), \
            algorithms.DensityKnownAlgorithm(config, e, C, d), \
            #algorithms.BackoffAlgorithm(config), \
            algorithms.BackoffAckAlgorithm(config), \
            ]

    for N in range(N_start, N_end + 1, N_step):
        S = S_start
        while S <= S_end:
            results = {}
            diams = []
            for algorithm in alg:
                results[algorithm] = []
            for _ in range(tries):
                #model = sinrlib.UniformModel(config, N, S, 1 - e)
                #model = sinrlib.SocialModel(config, N, S, e, 0.1, 1 - e)
                model = sinrlib.GadgetModel(config, N, int(S), 1 - e)
                #model = sinrlib.Gadget2Model(config, N, S, e, 1 - e)
                #model.show()

                diameter = model.diameter()
                diams.append(diameter)
                for algorithm in alg:
                    try:
                        simulation = sinrlib.Simulation(model, lambda: sinrlib.ConstNoise(1.0))
                        r = simulation.run(algorithm, 100000)
                        results[algorithm].append(r)
                        sys.stdout.write('.')
                        sys.stdout.flush()
                    except sinrlib.AlgorithmFailed:
                        print >> sys.stderr, 'algorithm', algorithm, 'failed for N = %s, S = %s' % (N, S)
                        #model.show()
                sys.stdout.write(',')
                sys.stdout.flush()
            print 


            for algorithm in results:
                res = results[algorithm]
                avg = float(sum(res)) / len(res)
                std = math.sqrt(sum([(r - avg)**2 for r in res]) / len(res))
                avg_diam = float(sum(diams)) / len(diams)
    
                print algorithm.__class__.__name__, N, d, e, S, avg, std, avg_diam, avg / avg_diam

            S += S_step

*/
