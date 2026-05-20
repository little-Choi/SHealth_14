#include <cstdio>
#include <fstream>
#include <gtest/gtest.h>
#include <string>

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

// BMI 경계: 18.5(저체중), 18.5<·<23(정상), 23(과체중), 25(비만) — 동일 체중·키 조합
std::string fourBmiBoundaryRows(int age1, int age2, int age3, int age4) {
    return std::to_string(1) + "," + std::to_string(age1) + ",66.785,190.0\n" +
           std::to_string(2) + "," + std::to_string(age2) + ",70.0,175.0\n" +
           std::to_string(3) + "," + std::to_string(age3) + ",79.571,186.0\n" +
           std::to_string(4) + "," + std::to_string(age4) + ",100.0,200.0\n";
}

void expectAllCategoriesQuarterPercent(SHealth& shealth, int ageClass) {
    EXPECT_EQ(shealth.getBmiRatio(ageClass, BmiTypeCode::kUnderweight), 25.0);
    EXPECT_EQ(shealth.getBmiRatio(ageClass, BmiTypeCode::kNormal), 25.0);
    EXPECT_EQ(shealth.getBmiRatio(ageClass, BmiTypeCode::kOverweight), 25.0);
    EXPECT_EQ(shealth.getBmiRatio(ageClass, BmiTypeCode::kObesity), 25.0);
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
    EXPECT_EQ(health.getBmiRatio(25, BmiTypeCode::kUnderweight), 0.0);
    EXPECT_EQ(health.getBmiRatio(20, 500), 0.0);
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

TEST_F(SHealthFixture, TwentiesBand_AllBmiCategoriesAtBoundaries) {
    // Given: 20대(20~29세) 4명 — BMI 18.5 / 정상 / 23.0 / 25.0 경계값
    const std::string path = writeTempCsv(fourBmiBoundaryRows(20, 23, 26, 29));

    // When
    ASSERT_EQ(health.calculateBmi(path), 4);

    // Then: 저체중·정상·과체중·비만 각 25%
    expectAllCategoriesQuarterPercent(health, 20);
    removeTempFile(path);
}

TEST_F(SHealthFixture, ThirtiesBand_AllBmiCategoriesAtBoundaries) {
    // Given: 30대(30~39세) 4명 — BMI 18.5 / 정상 / 23.0 / 25.0 경계값
    const std::string path = writeTempCsv(fourBmiBoundaryRows(30, 33, 36, 39));

    // When
    ASSERT_EQ(health.calculateBmi(path), 4);

    // Then
    expectAllCategoriesQuarterPercent(health, 30);
    removeTempFile(path);
}

TEST_F(SHealthFixture, FortiesBand_AllBmiCategoriesAtBoundaries) {
    // Given: 40대(40~49세) 4명 — BMI 18.5 / 정상 / 23.0 / 25.0 경계값
    const std::string path = writeTempCsv(fourBmiBoundaryRows(40, 43, 46, 49));

    // When
    ASSERT_EQ(health.calculateBmi(path), 4);

    // Then
    expectAllCategoriesQuarterPercent(health, 40);
    removeTempFile(path);
}

TEST_F(SHealthFixture, FiftiesBand_AllBmiCategoriesAtBoundaries) {
    // Given: 50대(50~59세) 4명 — BMI 18.5 / 정상 / 23.0 / 25.0 경계값
    const std::string path = writeTempCsv(fourBmiBoundaryRows(50, 53, 56, 59));

    // When
    ASSERT_EQ(health.calculateBmi(path), 4);

    // Then
    expectAllCategoriesQuarterPercent(health, 50);
    removeTempFile(path);
}

TEST_F(SHealthFixture, SixtiesBand_AllBmiCategoriesAtBoundaries) {
    // Given: 60대(60~69세) 4명 — BMI 18.5 / 정상 / 23.0 / 25.0 경계값
    const std::string path = writeTempCsv(fourBmiBoundaryRows(60, 63, 66, 69));

    // When
    ASSERT_EQ(health.calculateBmi(path), 4);

    // Then
    expectAllCategoriesQuarterPercent(health, 60);
    removeTempFile(path);
}

TEST_F(SHealthFixture, ImputeMissingWeightByAgeBand) {
    const std::string path = writeTempCsv("1,25,60.0,170.0\n2,27,0.0,170.0\n");
    health.calculateBmi(path);
    EXPECT_EQ(health.getBmiRatio(20, BmiTypeCode::kNormal), 100.0);
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
