#include <cstdio>
#include <fstream>
#include <gtest/gtest.h>

#include "SHealth.h"

namespace {

class SHealthFixture : public ::testing::Test {
protected:
    SHealth health;
};

std::string writeTempCsv(const std::string& body) {
    const auto* info = ::testing::UnitTest::GetInstance()->current_test_info();
    const std::string path =
        std::string("test_") + info->test_suite_name() + "_" + info->name() + ".csv";
    std::ofstream out(path);
    out << "id,age,weight,height\n";
    out << body;
    return path;
}

void removeTempFile(const std::string& path) {
    std::remove(path.c_str());
}

}  // namespace

TEST_F(SHealthFixture, SmokeShealthDat) {
    const int count = health.calculateBmi("shealth.dat");
    ASSERT_GT(count, 0);

    const double underweight = health.getBmiRatio(20, BmiTypeCode::kUnderweight);
    const double normal = health.getBmiRatio(20, BmiTypeCode::kNormal);
    EXPECT_GE(underweight, 0.0);
    EXPECT_GE(normal, 0.0);
    EXPECT_LT(underweight, 100.0);
    EXPECT_LT(normal, 100.0);
}

TEST_F(SHealthFixture, GetBmiRatioInvalidArgumentsReturnZero) {
    health.calculateBmi("shealth.dat");
    EXPECT_DOUBLE_EQ(health.getBmiRatio(25, BmiTypeCode::kUnderweight), 0.0);
    EXPECT_DOUBLE_EQ(health.getBmiRatio(20, 500), 0.0);
}

TEST_F(SHealthFixture, AgeBandRatiosSumNearOneHundred) {
    health.calculateBmi("shealth.dat");
    for (int ageClass = AgeBandConfig::kMinAgeBand; ageClass <= AgeBandConfig::kMaxAgeBand;
         ageClass += AgeBandConfig::kAgeBandWidth) {
        const double sum =
            health.getBmiRatio(ageClass, BmiTypeCode::kUnderweight) +
            health.getBmiRatio(ageClass, BmiTypeCode::kNormal) +
            health.getBmiRatio(ageClass, BmiTypeCode::kOverweight) +
            health.getBmiRatio(ageClass, BmiTypeCode::kObesity);
        if (sum > 0.0) {
            EXPECT_NEAR(sum, 100.0, 0.01);
        }
    }
}

TEST_F(SHealthFixture, ClassifyObesityAtBmi25) {
    const std::string path = writeTempCsv("1,25,100.0,200.0\n");
    health.calculateBmi(path);
    EXPECT_DOUBLE_EQ(health.getBmiRatio(20, BmiTypeCode::kObesity), 100.0);
    removeTempFile(path);
}

TEST_F(SHealthFixture, ClassifyUnderweightAtBmi18_5) {
    const std::string path = writeTempCsv("1,25,66.785,190.0\n");
    health.calculateBmi(path);
    EXPECT_DOUBLE_EQ(health.getBmiRatio(20, BmiTypeCode::kUnderweight), 100.0);
    removeTempFile(path);
}

TEST_F(SHealthFixture, ClassifyNormalBetweenBoundaries) {
    const std::string path = writeTempCsv("1,25,70.0,175.0\n");
    health.calculateBmi(path);
    EXPECT_DOUBLE_EQ(health.getBmiRatio(20, BmiTypeCode::kNormal), 100.0);
    removeTempFile(path);
}

TEST_F(SHealthFixture, ClassifyOverweightAtBmi23) {
    const std::string path = writeTempCsv("1,25,79.571,186.0\n");
    health.calculateBmi(path);
    EXPECT_DOUBLE_EQ(health.getBmiRatio(20, BmiTypeCode::kOverweight), 100.0);
    removeTempFile(path);
}

TEST_F(SHealthFixture, ImputeMissingWeightByAgeBand) {
    const std::string path = writeTempCsv("1,25,60.0,170.0\n2,27,0.0,170.0\n");
    health.calculateBmi(path);
    EXPECT_DOUBLE_EQ(health.getBmiRatio(20, BmiTypeCode::kNormal), 100.0);
    removeTempFile(path);
}

TEST_F(SHealthFixture, CalculateBmiMissingFileReturnsZero) {
    EXPECT_EQ(health.calculateBmi("missing_file.dat"), 0);
}

TEST_F(SHealthFixture, HeaderOnlyFileReturnsZeroCount) {
    const std::string path = writeTempCsv("");
    EXPECT_EQ(health.calculateBmi(path), 0);
    removeTempFile(path);
}
