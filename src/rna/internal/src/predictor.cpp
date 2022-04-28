#include <iostream>

#include <internal/include/basepair.h>
#include <internal/include/predictor.h>

SecondaryStructurePredictor::SecondaryStructurePredictor(std::string s)
{
    sequence = s;
    n = sequence.length();
    OPT.resize(n + 1, std::vector<int>(n + 1, 0));
}

bool SecondaryStructurePredictor::correct_pair(std::string &s, int i, int j)
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

void SecondaryStructurePredictor::compute_OPT()
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

void SecondaryStructurePredictor::compute_secondary_structure_helper(int i, int j)
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
        BasePair bp(Base(sequence[i - 1]), Base(sequence[j - 1]), i, j);

        secondaryStruct.push_back(bp);
        compute_secondary_structure_helper(i + 1, j - 1);
        return;
    }

    for (int t = i + 1; t <= j - 5; t++)
    {
        if (OPT[i][j] == 1 + OPT[i][t - 1] + OPT[t + 1][j - 1] && correct_pair(sequence, t - 1, j - 1))
        {

            BasePair bp(Base(sequence[t - 1]), Base(sequence[j - 1]), t, j);

            secondaryStruct.push_back(bp);
            compute_secondary_structure_helper(i, t - 1);
            compute_secondary_structure_helper(t + 1, j - 1);

            return;
        }
    }
}

void SecondaryStructurePredictor::compute_secondary_structure()
{
    compute_secondary_structure_helper(1, n);
}

void SecondaryStructurePredictor::print_OPT()
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

void SecondaryStructurePredictor::print_secondary_structure()
{
    for (auto &i : secondaryStruct)
    {
        std::cout << (char)i.getFirst() << "-" << (char)i.getSecond();
        std::cout << " pair at (" << i.getFirstIndex() << "," << i.getSecondIndex() << ") ";
        std::cout << std::endl;
    }

    std::cout << "Number of base pairs: " << secondaryStruct.size() << std::endl;

    std::cout << std::endl;
}

bool SecondaryStructurePredictor::verify_secondary_structure()
{

    int max_index = 0;

    for (auto &bp: secondaryStruct)
    {
        max_index = std::max(max_index, bp.getFirstIndex());
        max_index = std::max(max_index, bp.getSecondIndex());
    }

    std::vector<bool> appeared(max_index + 1, false);

    for(auto &bp: secondaryStruct) {
        if(std::abs((int64_t) bp.getFirstIndex() - (int64_t) bp.getSecondIndex()) <= 4) {
            return false;
        }

        if(not correct_pair(sequence, bp.getFirstIndex() - 1, bp.getSecondIndex() - 1)) {
            return false;
        }

        if(appeared[bp.getFirstIndex()]) {
            return false;
        }

        appeared[bp.getFirstIndex()] = true;

        if(appeared[bp.getSecondIndex()]) {
            return false;
        }

        appeared[bp.getSecondIndex()] = true;

        for(auto& bp2: secondaryStruct) {
            if(bp.getFirstIndex() < bp2.getFirstIndex() && bp2.getFirstIndex() < bp.getSecondIndex() && bp.getSecondIndex() < bp2.getSecondIndex()) {
                return false;
            }
        }
    }

    return true;
}







