#include "SHealth.h"

#include <fstream>
#include <iostream>
#include <sstream>

namespace {

int toCategoryIndex(BmiCategory category) {
    return static_cast<int>(category);
}

}  // namespace

int SHealth::ageBandIndex(int ageClass) {
    if (ageClass < AgeBandConfig::kMinAgeBand ||
        ageClass > AgeBandConfig::kMaxAgeBand ||
        (ageClass - AgeBandConfig::kMinAgeBand) % AgeBandConfig::kAgeBandWidth != 0) {
        return -1;
    }
    return (ageClass - AgeBandConfig::kMinAgeBand) / AgeBandConfig::kAgeBandWidth;
}

int SHealth::categoryIndex(int type) {
    switch (type) {
        case BmiTypeCode::kUnderweight:
            return 0;
        case BmiTypeCode::kNormal:
            return 1;
        case BmiTypeCode::kOverweight:
            return 2;
        case BmiTypeCode::kObesity:
            return 3;
        default:
            return -1;
    }
}

bool SHealth::inAgeBand(int age, int bandStart) {
    return age >= bandStart && age < bandStart + AgeBandConfig::kAgeBandWidth;
}

BmiCategory SHealth::classifyBmi(double bmi) noexcept {
    if (bmi <= BmiThreshold::kUnderweightMax) {
        return BmiCategory::Underweight;
    }
    if (bmi < BmiThreshold::kNormalMax) {
        return BmiCategory::Normal;
    }
    if (bmi < BmiThreshold::kOverweightMax) {
        return BmiCategory::Overweight;
    }
    return BmiCategory::Obesity;
}

int SHealth::calculateBmi(const std::string& filename) {
    ratiosByBand_.fill({});
    globalRatios_.fill(0.0);
    count = loadFromCsv(filename);
    if (count == 0) {
        return count;
    }
    imputeMissingWeights();
    imputeMissingHeights();
    computeAllBmis();
    aggregateRatiosByAgeBand();
    aggregateGlobalRatios();
    return count;
}

int SHealth::loadFromCsv(const std::string& filename) {
    count = 0;
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Failed to open file: " << filename << std::endl;
        return 0;
    }

    std::string line;
    std::getline(file, line);
    while (std::getline(file, line)) {
        std::vector<std::string> tokens = split(line, ',');
        if (tokens.empty()) {
            break;
        }
        ids[count] = std::stoi(tokens[CsvColumn::kId]);
        ages[count] = std::stoi(tokens[CsvColumn::kAge]);
        weights[count] = std::stod(tokens[CsvColumn::kWeight]);
        heights[count] = std::stod(tokens[CsvColumn::kHeight]);
        count++;
    }
    return count;
}

void SHealth::imputeMissingWeights() {
    for (int bandStart = AgeBandConfig::kMinAgeBand;
         bandStart <= AgeBandConfig::kMaxAgeBand;
         bandStart += AgeBandConfig::kAgeBandWidth) {
        double sum = 0.0;
        int ageCount = 0;
        for (int i = 0; i < count; i++) {
            if (!inAgeBand(ages[i], bandStart)) {
                continue;
            }
            if (weights[i] == kMissingWeight) {
                continue;
            }
            sum += weights[i];
            ageCount++;
        }
        if (ageCount == 0) {
            continue;
        }
        const double average = sum / static_cast<double>(ageCount);
        for (int i = 0; i < count; i++) {
            if (inAgeBand(ages[i], bandStart) && weights[i] == kMissingWeight) {
                weights[i] = average;
            }
        }
    }
}

void SHealth::imputeMissingHeights() {
    for (int bandStart = AgeBandConfig::kMinAgeBand;
         bandStart <= AgeBandConfig::kMaxAgeBand;
         bandStart += AgeBandConfig::kAgeBandWidth) {
        double sum = 0.0;
        int ageCount = 0;
        for (int i = 0; i < count; i++) {
            if (!inAgeBand(ages[i], bandStart)) {
                continue;
            }
            if (heights[i] == kMissingHeight) {
                continue;
            }
            sum += heights[i];
            ageCount++;
        }
        if (ageCount == 0) {
            continue;
        }
        const double average = sum / static_cast<double>(ageCount);
        for (int i = 0; i < count; i++) {
            if (inAgeBand(ages[i], bandStart) && heights[i] == kMissingHeight) {
                heights[i] = average;
            }
        }
    }
}

void SHealth::computeAllBmis() {
    for (int i = 0; i < count; i++) {
        const double heightM = heights[i] / kCentimetersPerMeter;
        bmis[i] = weights[i] / (heightM * heightM);
    }
}

void SHealth::aggregateRatiosByAgeBand() {
    for (int bandStart = AgeBandConfig::kMinAgeBand;
         bandStart <= AgeBandConfig::kMaxAgeBand;
         bandStart += AgeBandConfig::kAgeBandWidth) {
        std::array<int, 4> categoryCounts{};
        int bandTotal = 0;

        for (int i = 0; i < count; i++) {
            if (!inAgeBand(ages[i], bandStart)) {
                continue;
            }
            bandTotal++;
            const int idx = toCategoryIndex(classifyBmi(bmis[i]));
            categoryCounts[static_cast<size_t>(idx)]++;
        }

        const int bandIdx = ageBandIndex(bandStart);
        if (bandIdx < 0 || bandTotal == 0) {
            continue;
        }

        for (int category = 0; category < 4; category++) {
            ratiosByBand_[static_cast<size_t>(bandIdx)][static_cast<size_t>(category)] =
                static_cast<double>(categoryCounts[static_cast<size_t>(category)]) *
                kPercentMultiplier / static_cast<double>(bandTotal);
        }
    }
}

void SHealth::aggregateGlobalRatios() {
    globalRatios_.fill(0.0);
    if (count == 0) {
        return;
    }

    std::array<int, 4> categoryCounts{};
    for (int i = 0; i < count; i++) {
        const int idx = toCategoryIndex(classifyBmi(bmis[i]));
        categoryCounts[static_cast<size_t>(idx)]++;
    }

    for (int category = 0; category < 4; category++) {
        globalRatios_[static_cast<size_t>(category)] =
            static_cast<double>(categoryCounts[static_cast<size_t>(category)]) *
            kPercentMultiplier / static_cast<double>(count);
    }
}

double SHealth::lookupRatio(int ageClass, int type) const {
    const int bandIdx = ageBandIndex(ageClass);
    const int catIdx = categoryIndex(type);
    if (bandIdx < 0 || catIdx < 0) {
        return 0.0;
    }
    return ratiosByBand_[static_cast<size_t>(bandIdx)][static_cast<size_t>(catIdx)];
}

double SHealth::getBmiRatio(int ageClass, int type) {
    return lookupRatio(ageClass, type);
}

double SHealth::getGlobalBmiRatio(int type) const {
    const int catIdx = categoryIndex(type);
    if (catIdx < 0) {
        return 0.0;
    }
    return globalRatios_[static_cast<size_t>(catIdx)];
}

std::vector<int> SHealth::getNormalBmiUserIds() const {
    std::vector<int> normalIds;
    for (int i = 0; i < count; i++) {
        if (classifyBmi(bmis[i]) == BmiCategory::Normal) {
            normalIds.push_back(ids[i]);
        }
    }
    return normalIds;
}

std::vector<std::string> SHealth::split(const std::string& line, char delimiter) {
    std::vector<std::string> tokens;
    std::string token;
    std::istringstream tokenStream(line);
    while (std::getline(tokenStream, token, delimiter)) {
        tokens.push_back(token);
    }
    return tokens;
}
