#pragma once

#include <internal/include/base.h>

/**
 * @brief A class to store information about base pairings
 *
 */
class BasePair
{
public:
    BasePair();
    BasePair(Base a, Base b, int i, int j);

    Base getFirst();
    Base getSecond();
    
    int getFirstIndex();
    int getSecondIndex();

private:
    /**
     * @brief The first base of the pair
     *
     */
    Base first;
    /**
     * @brief The second base of the pair
     *
     */
    Base second;
    /**
     * @brief The index of the first base
     *
     */
    int firstIndex;
    /**
     * @brief The index of the second base
     *
     */
    int secondIndex;
};