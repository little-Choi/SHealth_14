#include "SHealth.h"

#include <cstdio>

int main() {
    SHealth shealth;
    shealth.calculateBmi("shealth.dat");

    for (int ageClass = AgeBandConfig::kMinAgeBand; ageClass <= AgeBandConfig::kMaxAgeBand;
         ageClass += AgeBandConfig::kAgeBandWidth) {
        printf("%d - underweight = %f, normal = %f, overweight = %f, obesity = %f\n", ageClass,
               shealth.getBmiRatio(ageClass, BmiTypeCode::kUnderweight),
               shealth.getBmiRatio(ageClass, BmiTypeCode::kNormal),
               shealth.getBmiRatio(ageClass, BmiTypeCode::kOverweight),
               shealth.getBmiRatio(ageClass, BmiTypeCode::kObesity));
    }

    return 0;
}
