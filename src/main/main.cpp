#include <bits/stdc++.h>

#include <rna/base.h>
#include <rna/basepair.h>
#include <rna/predictor.h>


int main()
{

    std::string s = "GGGUGUAGAAAAGUAAGGGAAACUCAAACCCCUUUCUACACCC";
    int n = s.length();

    SecondaryStructurePredictor ss(s);
    ss.compute_OPT();
    ss.compute_secondary_structure();
    ss.print_secondary_structure();
}