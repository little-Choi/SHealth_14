# SHealth 코드 품질 분석 리포트

**대상:** `calculateBmi`, `getBmiRatio`, `split` (`SHealth.h` / `SHealth.cpp`)  
**관점:** SOLID, Code Smell, C++17 모던 스타일  
**작성일:** 2026-05-19

---

## 1. 함수별 책임 요약

| 함수 | 현재 책임 (실제로 수행하는 일) |
|------|-------------------------------|
| `calculateBmi` | 파일 I/O → CSV 파싱 → 결측 체중 보정 → BMI 산출 → 연령대별 4분류 집계 → 백분율 저장 → `count` 반환 |
| `getBmiRatio` | `(ageClass, type)` 정수 쌍을 24개 멤버 변수 중 하나에 매핑해 반환 |
| `split` | 구분자 기준 문자열 토큰 분리 (유틸리티) |

`calculateBmi` 한 메서드가 **데이터 로딩·전처리·도메인 계산·통계 집계·결과 저장**까지 모두 담당한다. `getBmiRatio`는 그 결과를 **거대한 분기문**으로 노출한다.

---

## 2. 문제점 분석 표

| 문제점 | 위반 원칙/스멜 | 영향 | 개선 방향 | 우선순위 |
|--------|----------------|------|-----------|----------|
| `calculateBmi`가 파일 I/O, 파싱, 결측 보정, BMI 계산, 분류, 집계, 멤버 저장을 한 함수에서 처리 | **SRP** 위반, **Long Method** | 단위 테스트 불가, 변경 시 회귀 범위 확대, 가독성 저하 | `loadRecords`, `imputeMissingWeights`, `computeBmis`, `aggregateByAgeBand` 등 단계별 private 메서드 또는 별도 클래스로 분리 | **1** |
| 연령대(20~70)·BMI 카테고리마다 `underweight20`…`obesity70` 등 **24개 동형 멤버 변수** | **SRP** / **Data Clump**, **Duplicated Code** | 필드·분기·접근 코드가 6배로 증가, 연령대 추가 시 클래스 전면 수정 | `std::array<std::array<double, 4>, 6>` 또는 `std::map<AgeBand, BmiDistribution>` 등 **2차원 테이블**로 통합; `getBmiRatio`는 인덱스 조회 한 줄로 대체 | **1** |
| `getBmiRatio` 24분기 `if-else` 체인 (`ageClass`×`type` 100/200/300/400) | **OCP** 위반, **Switch Statements** 스멜 | 연령대·카테고리 추가 시 분기 4~6개씩 증가, 실수·누락 위험 | `enum class AgeBand`, `enum class BmiCategory` + 테이블 `ratio[ageIndex][categoryIndex]`; 잘못된 인자는 `std::optional` 또는 예외 | **2** |
| BMI 경계값 `18.5`, `23`, `25` 및 `100`(cm 변환), `100`(%), 연령 `20`~`70` step `10`, `type` 100~400이 리터럴로 산재 | **Magic Number** | 기준 변경 시 여러 위치 수정, README와 코드 불일치 가능 | `namespace BmiThreshold { constexpr double kUnderweightMax = 18.5; ... }`, `kPercentScale`, `kCmPerMeter` 등 **named constant** / `constexpr` 테이블 | **2** |
| `calculateBmi` 내 연령대 루프(`a=20..70`) 및 `a==20/30/...` 할당 블록 **6회 중복** | **Duplicated Code** | 동일 로직 6벌 유지, 버그 수정 시 6곳 동시 수정 | 루프 인덱스 `bandIdx = (a - 20) / 10`로 `ratios[bandIdx][category] = ...` 일괄 저장 | **1** |
| BMI 분류 조건: `>25` 비만 vs README "25**이상**"; `>18.5 && <23` 등 경계 중복·누락 | **조건문 복잡도**, 잠재 **버그** | BMI=25.0 등 경계값 미분류 또는 잘못된 분류 | `classifyBmi(double)` 단일 함수 + **반열린 구간 테이블** (`<=18.5`, `(18.5,23)`, `[23,25)`, `>=25`) | **2** |
| `ages[10000]`, `heights[10000]` 등 고정 C 배열 + `count` 수동 관리 | **Magic Number** (10000), 안전성 | 버퍼 오버플로, RAII 부재 | `std::vector<HealthRecord>` 또는 `std::array` + `reserve`; `HealthRecord { age, height, weight, bmi }` | **3** |
| 결측 체중 보정 시 `ageCount==0` 또는 `sum==0` 미검사 | **방어적 코딩 부재** | 0으로 나누기, NaN 비율 | 보정 전 유효 표본 수 검사; 불가 시 `std::optional` / 스킵 로그 | **3** |
| `split`이 `SHealth` private 멤버이나 도메인과 무관 | **SRP** (클래스 비대화) | 재사용·테스트 시 불필요한 `SHealth` 의존 | `namespace csv` 자유 함수 또는 C++17 `std::string_view` 기반 유틸; 표준 라이브러리 패턴과 동일 | **5** |
| `calculateBmi` 실패 시 `return 0` (파일 오픈 실패와 “레코드 0건” 구분 불가) | **의미 없는 반환값** | 호출부 오류 판별 불가 | `std::expected<int, Error>` (C++23) 또는 `bool`+out 파라미터 / 예외 정책 통일 | **4** |
| 새 연령대(80대)·BMI 기준·출력 타입 추가 시 `calculateBmi`+`getBmiRatio`+헤더 24필드 동시 수정 | **OCP** 위반 | 확장 비용 선형 이상 증가 | 데이터 주도: 연령대·카테고리 **설정 테이블** + 루프 집계; 분기 제거 | **1** |
| `tokens[1]`,`[2]`,`[3]` 컬럼 인덱스, `','` 구분자 하드코딩 | **Magic Number** | CSV 스키마 변경 시 파싱 깨짐 | `enum Column { Id, Age, Weight, Height }` 또는 헤더 매핑 | **4** |

---

## 3. SOLID 상세 — SRP / OCP

### 3.1 SRP (Single Responsibility Principle)

| 구성 요소 | 책임 개수 (추정) | 근거 |
|-----------|------------------|------|
| `SHealth` 클래스 전체 | ≥5 | 저장소(배열+24비율) + ETL + 통계 + 조회 API |
| `calculateBmi` | ≥5 | I/O, 파싱, imputation, BMI, 집계·저장 |
| `getBmiRatio` | 2 | (1) 키 해석 (2) 저장소 접근 — 해석 로직이 비대 |
| `split` | 1 | 단일 책임이나 **잘못된 소유자**(SHealth)에 위치 |

**SRP 위반 핵심:** “BMI 파이프라인”과 “결과 저장·조회”가 한 클래스에 결합되어, README 4단계 기능(특정 연령대 분포, Height 0 보정, 정상 사용자 목록 등) 추가 시 `calculateBmi`만 계속 비대해진다.

### 3.2 OCP (Open-Closed Principle)

| 변경 시나리오 | 현재 수정 지점 | OCP 관점 |
|---------------|----------------|----------|
| 80대 추가 | 루프 상한, 멤버 4×4개, `calculateBmi` 7분기, `getBmiRatio` 4분기 | **열림에 닫혀 있지 않음** — 수정으로만 확장 |
| BMI 기준(예: WHO 아시아) 변경 | `calculateBmi` 65~72행 조건문 | 분류 전략 교체 불가 |
| 카테고리 5개(고도비만) 추가 | 조건·필드·`type` 코드 전부 | 테이블 스키마 없어 비용 큼 |

**OCP 개선 핵심:** “연령대 × 카테고리”를 **데이터 구조**로 두고, 동작은 **루프 + 함수 포인터/람다** 한 벌만 유지한다.

---

## 4. Magic Number 상수화 필요성

| 리터럴 | 위치/용도 | 상수화 제안 |
|--------|-----------|-------------|
| `10000` | 배열 크기 | `kMaxRecords` 또는 vector 동적 |
| `20`, `70`, `10` | 연령대 밴드 | `kMinAgeBand`, `kMaxAgeBand`, `kAgeBandWidth` |
| `1`, `2`, `3` | CSV 컬럼 | `Column::Age` 등 |
| `0`, `0.0` | 결측 체중, 실패 반환 | `kMissingWeight`, `ErrorCode` |
| `100.0` | cm→m | `kCentimetersPerMeter` |
| `18.5`, `23`, `25` | BMI 분류 | `BmiThreshold` 네임스페이스 |
| `100` | 백분율 | `kPercentMultiplier` |
| `100`, `200`, `300`, `400` | `type` 코드 | `enum class BmiCategory : int` |

`type`과 `ageClass`의 **매직 쌍**은 `getBmiRatio` 24분기의 직접 원인이므로, enum + 테이블이 가장 효과가 크다.

---

## 5. Code Smell 체크리스트

| 스멜 | 해당 여부 | 위치 |
|------|-----------|------|
| **Long Method** | ✅ | `calculateBmi` (~100줄, 4단계) |
| **Duplicated Code** | ✅ | 연령대별 집계·할당 6회; `getBmiRatio` 24회 유사 분기 |
| **조건문 복잡도** | ✅ | BMI 분류 nested if; `getBmiRatio` 평탄하지만 길이 극대 |
| **God Class** | ✅ | `SHealth` — 데이터+로직+24 결과 필드 |
| **Primitive Obsession** | ✅ | `ageClass`, `type` int |
| **Feature Envy** | △ | 집계 결과가 멤버 필드에 직접 쓰기 — 절차적 스타일 |

`split` 자체는 짧고 단순하여 스멜이 적으나, **부적절한 응집**으로 클래스를 비대하게 만든다.

---

## 6. C++17 개선 방향

### 6.1 테이블 기반 (권장 1순위)

```cpp
enum class AgeBand : int { Twenties = 20, Thirties = 30, /* ... */ };
enum class BmiCategory { Underweight, Normal, Overweight, Obesity };

struct BmiDistribution {
    std::array<double, 4> percentByCategory{}; // index = BmiCategory
};

// ageBandIndex: (ageClass - 20) / 10
std::array<BmiDistribution, 6> distributions_{};
```

- `getBmiRatio(age, cat)` → `return distributions_[idx][catIdx];`
- OCP: 연령대 수를 `constexpr size_t kNumBands = 6`로만 관리

### 6.2 전략 패턴 / 분류 테이블

```cpp
struct BmiBand {
    double upperExclusive; // 또는 lower/upper pair
    BmiCategory category;
};
constexpr std::array<BmiBand, 4> kClassificationTable = { ... };

BmiCategory classify(double bmi) {
    for (const auto& band : kClassificationTable)
        if (bmi <= band.upperExclusive) return band.category;
    return BmiCategory::Obesity;
}
```

- 기준 변경 시 **테이블만** 교체 (WHO/아시아 등)

### 6.3 `std::variant` / `std::optional` 활용

- 결측: `std::optional<double> weight`
- `getBmiRatio` 실패: `std::optional<double>` — `0.0`과 “없음” 구분
- 파이프라인 단계별 결과: `std::variant<LoadError, ImputeError, Success>` (C++17)

### 6.4 기타 C++17 관용구

- `std::string_view` — `split` 입력, 불필요한 복사 감소
- Structured binding — `for (auto& [band, dist] : enumerate(distributions_))`
- `if constexpr` — 컴파일 타임 밴드 수 검증
- 파일 I/O: RAII `ifstream` (이미 사용) + `std::filesystem::path`

### 6.5 책임 분리 (SRP) 클래스 스케치

```
CsvRecordReader  → vector<HealthRecord>
WeightImputer    → impute by age band
BmiCalculator    → fill bmi field
BmiAggregator    → BmiDistribution table
SHealth          → facade: orchestrate + expose getBmiRatio
```

---

## 7. 리팩토링 우선순위 (1~5)

| 순위 | 작업 | 이유 |
|------|------|------|
| **1** | 24개 멤버 → `std::array` 2D 테이블 + `calculateBmi`/`getBmiRatio` 중복 제거 | **가장 큰 OCP/SRP/중복 해소**; 이후 모든 리팩토링의 기반 |
| **2** | BMI 임계값·연령대·`type` enum/상수화 + `classifyBmi()` 단일화 | 도메인 정확성(경계 버그), README와 코드 일치, 테스트 용이 |
| **3** | `calculateBmi` 단계별 함수/클래스 분리 + `vector<HealthRecord>` | Long Method 해소, 3단계(UT) 요구사항 대응 |
| **4** | 오류 모델·CSV 컬럼 매핑 개선 | 운영 안정성; 기능 확장 전 신뢰성 |
| **5** | `split` 외부 유틸로 이동 | 영향 범위 작음; 1~4 완료 후 정리해도 무방 |

---

## 8. 개선 방향 요약

1. **데이터 모델부터 바꾼다.** 24개 필드와 24분기는 구조적 부채의 중심이므로, `연령대 × BMI카테고리` 2차원 테이블 하나로 `calculateBmi` 집계와 `getBmiRatio` 조회를 동시에 단순화한다.
2. **도메인 규칙을 한곳에 모은다.** BMI 분류·백분율·cm→m 변환은 `constexpr` 상수와 `classifyBmi()`(또는 분류 테이블)로 모아 README 변경 시 한 파일만 수정하게 한다.
3. **`calculateBmi`는 오케스트레이터로 축소한다.** 로드 → 보정 → 계산 → 집계를 각각 테스트 가능한 단위로 나누면 SRP를 만족하고 Google Test 작성(실습 3단계)이 수월해진다.
4. **확장은 데이터 추가로 한다.** 새 연령대·카테고리는 배열 크기와 설정 테이블만 늘리고, `if (a==20)`·`if (ageClass==20 && type==100)` 패턴은 제거한다.
5. **`split`은 후순위로 분리한다.** 로직은 양호하나 SHealth와 결합만 끊으면 된다.

이 순서대로 진행하면 **1·2차 리팩토링(네이밍, 하드코드, 중복 제거)**과 **4단계 기능 개선(SRP, Height 0 보정, 목록 조회)**이 같은 아키텍처 방향을 공유하게 된다.

---

## 9. 참고: 현재 코드 핫스팟

```6:108:src/main/cpp/SHealth.cpp
int SHealth::calculateBmi(const std::string& filename) {
    // ... 파일 로드, 보정, BMI, 6연령대 × 4카테고리 집계 및 개별 멤버 할당 ...
}
```

```111:137:src/main/cpp/SHealth.cpp
double SHealth::getBmiRatio(int ageClass, int type) {
    // ... 24-way if-else ...
}
```

```18:25:src/main/cpp/SHealth.h
    double underweight20 = 0, underweight30 = 0, /* ... */ obesity70 = 0;
```

---

*본 문서는 정적 분석 기준이며, 리팩토링 시 `SHealthBMITest.cpp` 및 `shealth.dat` 기반 회귀 테스트를 함께 수행할 것을 권장한다.*
