#pragma once

#include <cstdio>
#include <sstream>
#include <string>

#include "SHealth.h"

namespace golden {

// TexttestFixture-style stable text snapshot (printf %f semantics).
inline std::string formatAgeBandRatioReport(SHealth& health) {
    std::ostringstream out;
    for (int ageClass = AgeBandConfig::kMinAgeBand; ageClass <= AgeBandConfig::kMaxAgeBand;
         ageClass += AgeBandConfig::kAgeBandWidth) {
        char line[256];
        std::snprintf(
            line, sizeof(line),
            "%d - underweight = %f, normal = %f, overweight = %f, obesity = %f",
            ageClass, health.getBmiRatio(ageClass, BmiTypeCode::kUnderweight),
            health.getBmiRatio(ageClass, BmiTypeCode::kNormal),
            health.getBmiRatio(ageClass, BmiTypeCode::kOverweight),
            health.getBmiRatio(ageClass, BmiTypeCode::kObesity));
        out << line << '\n';
    }
    return out.str();
}

inline std::string formatFullSnapshot(SHealth& health, int recordCount,
                                    const std::string& sourceFile) {
    std::ostringstream out;
    out << "# fixture: " << sourceFile << '\n';
    out << "# schema: shealth_snapshot_v1\n";
    out << "# records: " << recordCount << '\n';
    out << "# section: age_band_ratios\n";
    out << formatAgeBandRatioReport(health);
    out << "# section: global_ratios\n";
    char globalLine[256];
    std::snprintf(globalLine, sizeof(globalLine),
                  "global - underweight = %f, normal = %f, overweight = %f, obesity = %f",
                  health.getGlobalBmiRatio(BmiTypeCode::kUnderweight),
                  health.getGlobalBmiRatio(BmiTypeCode::kNormal),
                  health.getGlobalBmiRatio(BmiTypeCode::kOverweight),
                  health.getGlobalBmiRatio(BmiTypeCode::kObesity));
    out << globalLine << '\n';
    return out.str();
}

}  // namespace golden
