// Project identifier: 9504853406CBAC39EE89AA3AD238AA12CA198043

#ifndef P2RANDOM_H
#define P2RANDOM_H

#include <vector>
#include <string>

class P2random {
public:
    static void initialize(unsigned int seed, unsigned int maxDist, unsigned int maxSpeed, unsigned int maxHealth);
    static unsigned int rand();

    static std::string getNextZombieName();
    static unsigned int getNextZombieDistance();
    static unsigned int getNextZombieSpeed();
    static unsigned int getNextZombieHealth();

    //Don't need to read further than this, unless you want to learn about
    //Mersenne Twister implementation
private:
    static unsigned int zombieCounter;
    static unsigned int maxRandDist, maxRandSpeed, maxRandHealth;
    static std::vector<std::string> ZOMBIE_NAMES;
    static unsigned int getNextInt(unsigned int);

    /**
    * mt.h: Mersenne Twister header file
    *
    * Jason R. Blevins <jrblevin@sdf.lonestar.org>
    * Durham, March  7, 2007
    */

    /**
    * Mersenne Twister.
    *
    * M. Matsumoto and T. Nishimura, "Mersenne Twister: A
    * 623-dimensionally equidistributed uniform pseudorandom number
    * generator", ACM Trans. on Modeling and Computer Simulation Vol. 8,
    * No. 1, January pp.3-30 (1998).
    *
    * http://www.math.sci.hiroshima-u.ac.jp/~m-mat/MT/emt.html.
    */
    class MersenneTwister {
    public:
        MersenneTwister(void);
        ~MersenneTwister(void);

        // The copy constructor and operator=() should never be used.
        MersenneTwister(const MersenneTwister&) = delete;
        MersenneTwister &operator=(const MersenneTwister&) = delete;

        void init_genrand(unsigned int s);
        unsigned int genrand_unsigned_int(void);

    private:
        static const unsigned int N = 624;
        static const unsigned int M = 397;
        // constant vector a
        static const unsigned int MATRIX_A = 0x9908b0dfU;
        // most significant w-r bits
        static const unsigned int UPPER_MASK = 0x80000000U;
        // least significant r bits
        static const unsigned int LOWER_MASK = 0x7fffffffU;

        unsigned int *mt_;                  // the state vector
        unsigned int mti_;                  // mti == numStats+1 means mt not initialized
    };
    static MersenneTwister mt;
};

#endif 
