#include <gtest/gtest.h>

#include "SHealth.h"
#include "golden/GoldenMaster.h"
#include "golden/SHealthSnapshot.h"

namespace {

class SHealthGoldenFixture : public ::testing::Test {
protected:
    SHealth health;
    static constexpr const char* kDefaultDataFile = "shealth.dat";
};

}  // namespace

// T-027: shealth.dat 6x4 age-band ratio snapshot (refactoring regression guard).
TEST_F(SHealthGoldenFixture, GivenSixAgeBands_WhenSnapshot_ThenMatchGoldenFile) {
    const int recordCount = health.calculateBmi(kDefaultDataFile);
    ASSERT_GT(recordCount, 0);

    const std::string actual =
        golden::formatFullSnapshot(health, recordCount, kDefaultDataFile);

    EXPECT_TRUE(golden::assertMatchesGolden(actual, "shealth_full_snapshot.approved.txt"));
}

// CLI stdout parity with SHealthBMI (TexttestFixture-style line output).
TEST_F(SHealthGoldenFixture, GivenShealthDat_WhenCliReport_ThenMatchGoldenFile) {
    ASSERT_GT(health.calculateBmi(kDefaultDataFile), 0);

    const std::string actual = golden::formatAgeBandRatioReport(health);

    EXPECT_TRUE(
        golden::assertMatchesGolden(actual, "shealth_cli_age_bands.approved.txt"));
}
