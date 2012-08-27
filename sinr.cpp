#include <iostream>
#include <sstream>
#include <vector>

#include "sinrlib.h"
#include "naive.h"

int main(int argc, char **argv)
{
    sinr::config conf;
    int tries, N_start =2000, N_end = 2000, N_step = 1, S_start = 10, S_end = 10, S_step = 3;
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
            std::istringstream(argv[1]) >> tries;
            for (; tries > 0; tries--)
            {
                sinr::uniform_model model(conf, N_start, S_start, 1 - e);
                sinr::simulation sim(model);
                naive_algorithm alg;
                int result;
                result = sim.run(alg);
                std::cout << N << ' ' << S << ' ' << result << std::endl;
            }
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
