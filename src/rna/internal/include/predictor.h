#pragma once

#include <string>
#include <vector>

#include <internal/include/basepair.h>

/**
 * @brief A class to predict the secondary structure of a RNA sequence
 *
 */
class SecondaryStructurePredictor
{
public:
    /**
     * @brief Construct a new Secondary Structure Predictor object
     *
     * @param sequence The string of the sequence of the RNA
     */
    SecondaryStructurePredictor(std::string s);

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
    bool correct_pair(std::string &s, int i, int j);

    /**
     * @brief Computes the DP table
     *
     * @return void
     *
     */
    void compute_OPT();

    /**
     * @brief Recursive function which finds the base pairs in the optimal secondary structure
     *
     * @returns void
     *
     */
    void compute_secondary_structure_helper(int i, int j);

    void compute_secondary_structure();

    void print_OPT();

    void print_secondary_structure();

    bool verify_secondary_structure();

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
};