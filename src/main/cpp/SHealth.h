#pragma once

#include "SHealthConstants.h"

#include <array>
#include <string>
#include <vector>

enum class BmiCategory { Underweight, Normal, Overweight, Obesity };

enum class AgeBand : int {
    Twenties = 20,
    Thirties = 30,
    Forties = 40,
    Fifties = 50,
    Sixties = 60,
    Seventies = 70
};

enum class BmiCategoryType : int {
    Underweight = 100,
    Normal = 200,
    Overweight = 300,
    Obesity = 400
};

class SHealth {
public:
    int calculateBmi(const std::string& filename);
    double getBmiRatio(int ageClass, int type);
    double getGlobalBmiRatio(int type) const;
    std::vector<int> getNormalBmiUserIds() const;

private:
    using CategoryCounts = std::array<int, BmiCategoryConfig::kCount>;
    using CategoryRatios = std::array<double, BmiCategoryConfig::kCount>;

    static int ageBandIndex(int ageClass);
    static int categoryIndex(int type);
    static bool inAgeBand(int age, int bandStart);
    static BmiCategory classifyBmi(double bmi) noexcept;
    static bool isNormalBmi(double bmi) noexcept;
    static void fillPercentages(const CategoryCounts& counts, int total,
                                CategoryRatios& out);
    static double ratioAt(int catIdx, const CategoryRatios& table) noexcept;

    int loadFromCsv(const std::string& filename);
    void imputeMissingField(double* values, double missingSentinel);
    void imputeMissingWeights();
    void imputeMissingHeights();
    void computeAllBmis();
    CategoryCounts countCategoriesInAgeBand(int bandStart) const;
    CategoryCounts countCategoriesAll() const;
    void aggregateRatiosByAgeBand();
    void aggregateGlobalRatios();

    double lookupRatio(int ageClass, int type) const;

    int count = 0;
    int ids[kMaxRecords];
    int ages[kMaxRecords];
    double heights[kMaxRecords];
    double weights[kMaxRecords];
    double bmis[kMaxRecords];

    std::array<CategoryRatios, AgeBandConfig::kNumBands> ratiosByBand_{};
    CategoryRatios globalRatios_{};

    std::vector<std::string> split(const std::string& line, char delimiter);
};
