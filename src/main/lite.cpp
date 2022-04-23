// write a program which counts the number of base pairs in the secondary structure of a RNA sequence
// the sequence is given as a string
// An RNA molecule is a string B = b1b2 … bn; where each bi is a base pair
// An secondary structure on B is a set of pairs S = {(i , j)}, where 1 ≤ i , j ≤ n and satisfies the following rules.
// The ends of each pair are separated by at least 4 intervening bases i.e. if (i , j) ε S, then i < j - 4.
// The elements in each pair in S consist of either {A,U} or {C,G} (in either order).
// S is a matching: no base appears in more than one pair.
// (No knots) If (i,j) and (k,l) are two pairs in S, then we cannot have i < k < j < l .

// Approach:
// OPT(i,j) is the maximum number of base pairs in a secondary structure for bibi+1 … bj. OPT(i,j) = 0, if i≥j-4.
// In the optimal secondary structure on bibi+1 …bj
//      if j is not a member of any pair, use OPT(i,j-1).
//      if j pairs with some t < j - 4, knot condition yields two independent sub-problems! OPT(i,t -1) and OPT(t+1,j-1)
// recurrence relation:
// OPT(i,j) = max(OPT(i,j-1), max_{t < j-4} OPT(i,t-1) + OPT(t+1,j-1))
// t ranges from I to j-5 such that it is allowed to pair with j

// Pseudocode:
// n is the length of the sequence
// Intialize OPT(i,j) to 0 whenever i>=j-4
// for k = 5 to n - 1:
//      for i = 1,2,..., n - k:
//         set j = i + k
//         compute OPT(i,j) using recurrence relation
// return OPT(1,n)

#include <bits/stdc++.h>

// write an enum class to store a nucleic acid base

enum class Base : char
{
    A = 'A',
    C = 'C',
    G = 'G',
    U = 'U'
};

// bool correct_pair(std::string &s, int i, int j)
// {
//     if (s[i] == 'A' && s[j] == 'U')
//         return true;
//     if (s[i] == 'U' && s[j] == 'A')
//         return true;
//     if (s[i] == 'C' && s[j] == 'G')
//         return true;
//     if (s[i] == 'G' && s[j] == 'C')
//         return true;
//     return false;
// }

Base from_char(char c)
{
    switch (c)
    {
    case 'A':
        return Base::A;
    case 'C':
        return Base::C;
    case 'G':
        return Base::G;
    case 'U':
        return Base::U;
    default:
        return Base::A;
    }
}

class BasePair
{
    Base first;
    Base second;
    int firstIndex;
    int secondIndex;

public:
    BasePair() {}
    BasePair(Base a, Base b, int i, int j) : first(a), second(b), firstIndex(i), secondIndex(j) {}
    
    Base getFirst()
    {
        return first;
    }
    Base getSecond()
    {
        return second;
    }
    int getFirstIndex()
    {
        return firstIndex;
    }
    int getSecondIndex()
    {
        return secondIndex;
    }
};

void print_base(Base b)
{
    switch (b)
    {
    case Base::A:
        std::cout << "A";
        break;
    case Base::C:
        std::cout << "C";
        break;
    case Base::G:
        std::cout << "G";
        break;
    case Base::U:
        std::cout << "U";
        break;
    }
}



class SecondaryStructurePredictor
{
private:
    std::string sequence;
    std::vector<std::vector<int>> OPT;
    std::vector<std::vector<BasePair>> secondaryStructure;
    std::vector<BasePair> secondaryStruct;
    int n;

public:
    SecondaryStructurePredictor(std::string s)
    {
        sequence = s;
        n = sequence.length();
        OPT.resize(n + 1, std::vector<int>(n + 1, 0));
        secondaryStructure.resize(n + 1, std::vector<BasePair>(n + 1));
    }

    bool correct_pair(std::string &s, int i, int j)
{
    if (s[i] == 'A' && s[j] == 'U')
        return true;
    if (s[i] == 'U' && s[j] == 'A')
        return true;
    if (s[i] == 'C' && s[j] == 'G')
        return true;
    if (s[i] == 'G' && s[j] == 'C')
        return true;
    return false;
}
    

    void compute_OPT()
    {
        for (int k = 5; k < n; k++)
        {

            for (int i = 1; i <= n - k; i++)
            {
                int j = i + k;

                OPT[i][j] = OPT[i][j - 1];

                if (correct_pair(sequence, i - 1, j - 1))
                {
                    OPT[i][j] = std::max(OPT[i][j], 1 + OPT[i + 1][j - 1]);
                }

                for (int t = i + 1; t <= j - 5; t++)
                {
                    if (correct_pair(sequence, j - 1, t - 1))
                    {
                        OPT[i][j] = std::max(1 + OPT[i][t - 1] + OPT[t + 1][j - 1], OPT[i][j]);
                    }
                }
            }
        }
    }

    void compute_secondary_structure_helper(int i, int j)
    {
        if (i >= j)
            return;
        if (OPT[i][j] == OPT[i][j - 1])
        {
            compute_secondary_structure_helper(i, j - 1);
            return;
        }
        else if (OPT[i][j] == 1 + OPT[i + 1][j - 1] && correct_pair(sequence, i - 1, j - 1))
        {
            BasePair bp(from_char(sequence[i - 1]), from_char(sequence[j - 1]),i,j);

            secondaryStructure[i][j] = bp;
            secondaryStruct.push_back(bp);
            compute_secondary_structure_helper(i + 1, j - 1);
            return;
        }

        for (int t = i + 1; t <= j - 5; t++)
        {
            if (OPT[i][j] == 1 + OPT[i][t - 1] + OPT[t + 1][j - 1] && correct_pair(sequence, t - 1, j - 1))
            {
                // pair[t][j] = 1;
                BasePair bp(from_char(sequence[t - 1]), from_char(sequence[j - 1]),t,j);
                secondaryStructure[t][j] = bp;
                secondaryStruct.push_back(bp);
                compute_secondary_structure_helper(i, t - 1);
                compute_secondary_structure_helper(t + 1, j - 1);
                // get_pair(i, t - 1, s, OPT, pair);
                // get_pair(t + 1, j - 1, s, OPT, pair);
                return;
            }
        }
    }

    void compute_secondary_structure()
    {

        compute_secondary_structure_helper(1, n);
    }

    void print_OPT()
    {
        for (int i = 1; i <= n; i++)
        {
            for (int j = 1; j <= n; j++)
            {
                std::cout << OPT[i][j] << " ";
            }
            std::cout << std::endl;
        }
    }

    void print_secondary_structure()
    {
        for(auto &i : secondaryStruct)
        {
            print_base(i.getFirst());
            std::cout << "-";
            print_base(i.getSecond());
            std::cout << " pair at (" << i.getFirstIndex() << "," << i.getSecondIndex() << ") ";
            std::cout << std::endl;
        }
        std::cout << std::endl;
    }
};

void print_matrix(std::vector<std::vector<int>> &OPT, int k, int i, int j)
{
    std::cout << "*********************" << std::endl;
    std::cout << "Matrix when k = " << k << " and i = " << i << " and j = " << j << std::endl;
    std::cout << "  0 1 2 3 4 5 6 7 8 9" << std::endl;
    int n = OPT.size() - 1;
    for (int i = n; i >= 0; i--)
    {
        std::cout << i << " ";
        for (int j = 0; j <= n; j++)
        {
            std::cout << OPT[i][j] << " ";
        }
        std::cout << std::endl;
    }

    std::cout << "*********************" << std::endl;
}

// void get_pair(int i, int j, std::string &s, std::vector<std::vector<int>> &OPT, std::vector<std::vector<int>> &pair)
// {
//     // make pair[i][j] = 1 if (i,j) is a pair in the optimal structure
//     // make pair[i][j] = 0 otherwise

//     if (i >= j)
//         return;
//     if (OPT[i][j] == OPT[i][j - 1])
//     {
//         get_pair(i, j - 1, s, OPT, pair);
//         return;
//     }
//     else if (OPT[i][j] == 1 + OPT[i + 1][j - 1] && correct_pair(s, i - 1, j - 1))
//     {
//         pair[i][j] = 1;

//         get_pair(i + 1, j - 1, s, OPT, pair);
//         return;
//     }

//     for (int t = i + 1; t <= j - 5; t++)
//     {
//         if (OPT[i][j] == 1 + OPT[i][t - 1] + OPT[t + 1][j - 1] && correct_pair(s, t - 1, j - 1))
//         {
//             pair[t][j] = 1;
//             get_pair(i, t - 1, s, OPT, pair);
//             get_pair(t + 1, j - 1, s, OPT, pair);
//             return;
//         }
//     }
// }

int main()
{
    // std::string s = "ACCGGUAGU";
    // std::string s = "GGGUGUAGAAAAGUAAGGGAAACUCAAACCCCUUUCUACACCC";
    // int n = s.length();
    // std::vector<std::vector<int>> OPT(n + 1, std::vector<int>(n + 1, 0));
    // std::vector<std::vector<int>> pair(n + 1, std::vector<int>(n + 1, 0));

    // // print_matrix(OPT);

    // for (int k = 5; k < n; k++)
    // {

    //     for (int i = 1; i <= n - k; i++)
    //     {
    //         int j = i + k;

    //         OPT[i][j] = OPT[i][j - 1];

    //         if (correct_pair(s, i - 1, j - 1))
    //         {
    //             OPT[i][j] = std::max(OPT[i][j], 1 + OPT[i + 1][j - 1]);
    //         }

    //         for (int t = i + 1; t <= j - 5; t++)
    //         {
    //             if (correct_pair(s, j - 1, t - 1))
    //             {
    //                 OPT[i][j] = std::max(1 + OPT[i][t - 1] + OPT[t + 1][j - 1], OPT[i][j]);
    //             }
    //         }

    //         // print_matrix(OPT, k, i, j);
    //     }
    // }

    // std::cout << OPT[1][n] << std::endl;
    // get_pair(1, n, s, OPT, pair);

    // for (int i = 1; i <= n; i++)
    // {
    //     for (int j = 1; j <= n; j++)
    //     {
    //         if (pair[i][j] == 1)
    //         {
    //             std::cout << "(" << i << "," << j << ") is a pair" << std::endl;
    //             std::cout << "bases are " << s[i - 1] << " and " << s[j - 1] << std::endl;
    //         }
    //     }
    //     std::cout << std::endl;
    // }

    // for (int i = 1; i <= n; i++)
    // {
    //     for (int j = 1; j <= n; j++)
    //     {
    //         std::cout << pair[i][j] << " ";
    //     }
    //     std::cout << std::endl;
    // }

    // return 0;

    std::string s = "GGGUGUAGAAAAGUAAGGGAAACUCAAACCCCUUUCUACACCC";
    int n = s.length();

    SecondaryStructurePredictor ss(s);
    ss.compute_OPT();
    // ss.print_OPT();
    ss.compute_secondary_structure();
    ss.print_secondary_structure();
}