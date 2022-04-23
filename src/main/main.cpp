#include <bits/stdc++.h>

enum class Base : int
{
    A,
    C,
    G,
    U
};

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

/**
 * @brief A class to store information about base pairings
 *
 */
class BasePair
{
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

public:
    BasePair() {}
    /**
     * @brief Construct a new Base Pair object
     *
     * @param first The first base of the pair
     * @param second The second base of the pair
     * @param firstIndex The index of the first base
     * @param secondIndex The index of the second base
     */
    BasePair(Base a, Base b, int i, int j) : first(a), second(b), firstIndex(i), secondIndex(j) {}

    /**
     * @brief Get the First Base object
     *
     * @return Base
     */
    Base getFirst()
    {
        return first;
    }
    /**
     * @brief Get the Second Base object
     *
     * @return Base
     */
    Base getSecond()
    {
        return second;
    }
    /**
     * @brief Get the First Index object
     *
     * @return int
     */
    int getFirstIndex()
    {
        return firstIndex;
    }
    /**
     * @brief Get the Second Index object
     *
     * @return int
     */
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

/**
 * @brief A class to predict the secondary structure of a RNA sequence
 *
 */
class SecondaryStructurePredictor
{
private:
    /**
     * @brief The string of the sequence of the RNA
     *
     */
    std::string sequence;
    /**
     * @brief Stores the results of DP computation
     *
     */
    std::vector<std::vector<int>> OPT;

    std::vector<std::vector<BasePair>> secondaryStructure;
    /**
     * @brief Stores the information about the base pairs
     *
     */
    std::vector<BasePair> secondaryStruct;
    /**
     * @brief The length of the sequence
     *
     */
    int n;

public:
    /**
     * @brief Construct a new Secondary Structure Predictor object
     *
     * @param sequence The string of the sequence of the RNA
     */
    SecondaryStructurePredictor(std::string s)
    {
        sequence = s;
        n = sequence.length();
        OPT.resize(n + 1, std::vector<int>(n + 1, 0));
    }

    /**
     * @brief Outputs whether the pair of bases can form a base pair
     *
     * @param s The string of the sequence of RNA
     * @param i The index of the first base
     * @param j The index of the second base
     *
     * @return true If the pair of bases can form a base pair
     * @return false If the pair of bases cannot form a base pair
     */
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

    /**
     * @brief Computes the DP table
     * 
     * @return void
     * 
     */
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

    /**
     * @brief Recursive function which finds the base pairs in the optimal secondary structure
     * 
     * @returns void
     * 
     */
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
            BasePair bp(from_char(sequence[i - 1]), from_char(sequence[j - 1]), i, j);

            secondaryStruct.push_back(bp);
            compute_secondary_structure_helper(i + 1, j - 1);
            return;
        }

        for (int t = i + 1; t <= j - 5; t++)
        {
            if (OPT[i][j] == 1 + OPT[i][t - 1] + OPT[t + 1][j - 1] && correct_pair(sequence, t - 1, j - 1))
            {

                BasePair bp(from_char(sequence[t - 1]), from_char(sequence[j - 1]), t, j);

                secondaryStruct.push_back(bp);
                compute_secondary_structure_helper(i, t - 1);
                compute_secondary_structure_helper(t + 1, j - 1);

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
        for (auto &i : secondaryStruct)
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

int main()
{

    std::string s = "GGGUGUAGAAAAGUAAGGGAAACUCAAACCCCUUUCUACACCC";
    int n = s.length();

    SecondaryStructurePredictor ss(s);
    ss.compute_OPT();
    // ss.print_OPT();
    ss.compute_secondary_structure();
    ss.print_secondary_structure();
}