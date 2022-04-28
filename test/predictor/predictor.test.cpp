#include <gtest/gtest.h>

#include <rna/predictor.h>
#include <string>
#include <vector>

TEST(SecondaryStructurePredictor,Prediction1) {
    SecondaryStructurePredictor ss("GGCGAAGAACCGGGGAGCC");
    ss.compute_OPT();
    ss.compute_secondary_structure();

    ASSERT_EQ(ss.verify_secondary_structure(), true);

}

TEST(SecondaryStructurePredictor,Prediction2) {
    SecondaryStructurePredictor ss("CUGUGAAUG");
    ss.compute_OPT();
    ss.compute_secondary_structure();

    ASSERT_EQ(ss.verify_secondary_structure(), true);
}

TEST(SecondaryStructurePredictor,Prediction3) {
    SecondaryStructurePredictor ss("GGGUGUAGAAAAGUAAGGGAAACUCAAACCCCUUUCUACACCC");
    ss.compute_OPT();
    ss.compute_secondary_structure();

    ASSERT_EQ(ss.verify_secondary_structure(), true);
}

TEST(SecondaryStructurePredictor,Prediction4) {
    SecondaryStructurePredictor ss("AAUAAA");
    ss.compute_OPT();
    ss.compute_secondary_structure();

    ASSERT_EQ(ss.verify_secondary_structure(), true);
}

TEST(SecondaryStructurePredictor,Prediction5) {
    SecondaryStructurePredictor ss("UUCUGC");
    ss.compute_OPT();
    ss.compute_secondary_structure();

    ASSERT_EQ(ss.verify_secondary_structure(), true);
}

TEST(SecondaryStructurePredictor,Prediction6) {
    SecondaryStructurePredictor ss("UCGUGCGA");
    ss.compute_OPT();
    ss.compute_secondary_structure();

    ASSERT_EQ(ss.verify_secondary_structure(), true);
}

TEST(SecondaryStructurePredictor,Prediction7) {
    SecondaryStructurePredictor ss("CUUGUCUGACACG");
    ss.compute_OPT();
    ss.compute_secondary_structure();

    ASSERT_EQ(ss.verify_secondary_structure(), true);
}

TEST(SecondaryStructurePredictor,Prediction8) {
    SecondaryStructurePredictor ss("GGACUCGACUCC");
    ss.compute_OPT();
    ss.compute_secondary_structure();

    ASSERT_EQ(ss.verify_secondary_structure(), true);
}

TEST(SecondaryStructurePredictor,Prediction9) {
    SecondaryStructurePredictor ss("UGCUCCUAGUACGAGAGGACCGGAGUG");
    ss.compute_OPT();
    ss.compute_secondary_structure();

    ASSERT_EQ(ss.verify_secondary_structure(), true);
}

TEST(SecondaryStructurePredictor,Prediction10) {
    SecondaryStructurePredictor ss("GGCGAAGAACCGGGGAGCC");
    ss.compute_OPT();
    ss.compute_secondary_structure();

    ASSERT_EQ(ss.verify_secondary_structure(), true);
}

TEST(SecondaryStructurePredictor,Prediction11) {
    SecondaryStructurePredictor ss("CAAAGAAAUCCAGGUGCA");
    ss.compute_OPT();
    ss.compute_secondary_structure();

    ASSERT_EQ(ss.verify_secondary_structure(), true);
}