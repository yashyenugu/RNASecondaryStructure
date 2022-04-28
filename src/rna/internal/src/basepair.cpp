#include<internal/include/base.h>
#include<internal/include/basepair.h>

BasePair::BasePair() 
{
    
}

BasePair::BasePair(Base a, Base b, int i, int j) 
    : first(a)
    , second(b)
    , firstIndex(i)
    , secondIndex(j)
{

}

Base BasePair::getFirst()
{
    return first;
}

Base BasePair::getSecond()
{
    return second;
}

int BasePair::getFirstIndex()
{
    return firstIndex;
}

int BasePair::getSecondIndex()
{
    return secondIndex;
}