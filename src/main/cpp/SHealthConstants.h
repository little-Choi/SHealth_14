#pragma once

namespace BmiThreshold {
constexpr double kUnderweightMax = 18.5;
constexpr double kNormalMax = 23.0;
constexpr double kOverweightMax = 25.0;
}  // namespace BmiThreshold

namespace AgeBandConfig {
constexpr int kMinAgeBand = 20;
constexpr int kMaxAgeBand = 70;
constexpr int kAgeBandWidth = 10;
constexpr int kNumBands = 6;
}  // namespace AgeBandConfig

namespace BmiCategoryConfig {
constexpr int kCount = 4;
}  // namespace BmiCategoryConfig

namespace BmiTypeCode {
constexpr int kUnderweight = 100;
constexpr int kNormal = 200;
constexpr int kOverweight = 300;
constexpr int kObesity = 400;
}  // namespace BmiTypeCode

namespace CsvColumn {
constexpr int kId = 0;
constexpr int kAge = 1;
constexpr int kWeight = 2;
constexpr int kHeight = 3;
}  // namespace CsvColumn

constexpr int kMaxRecords = 10000;
constexpr double kCentimetersPerMeter = 100.0;
constexpr double kPercentMultiplier = 100.0;
constexpr double kMissingWeight = 0.0;
constexpr double kMissingHeight = 0.0;
