# SHealth BMI (C++)

# Overview
- 삼성 헬스에서는 수집된 데이터를 활용하여 사용자들의 나이대(ex. 20대, 30대, 40대 등)별 저체중/정상체중/과체중/비만의 통계를 계산하고자 합니다.
- 수집 중 누락된 체중값이 있으며 같은 나이대(ex. 20대, 30대, 40대 등)의 평균을 적용합니다. 체중 0이 누락된 경우 입니다.
- 수집 데이터는 ID, 나이, 몸무게(kg), 키(cm)이며, BMI는 체중(kg) / 키(m)제곱 으로 계산합니다.
- BMI 기준으로 18.5이하 저체중, 18.5초과 23미만 정상체중, 23이상 25미만 과체중, 25이상 비만으로 판단합니다.
![BMI](./bmi.png)
- 제공된 코드에는 다양한 코드 품질 문제가 있습니다. 


## data sample
- 입력 데이터 (shealth.dat)
```
id,age,weight,height
93705,66,79.5,158.3
93708,66,53.5,150.2
93709,75,88.8,151.1
... (이하 생략)
```
- 각 라인별 ID, 나이, 체중(kg), 키(cm) 순서 입니다.


## 빌드 및 실행

### 요구사항
- CMake 3.10 이상
- C++17 지원 컴파일러
- Google Test (CMake에서 자동 다운로드)

### 빌드
```bash
mkdir build && cd build
cmake ..
cmake --build .
```

### 실행
```bash
./SHealthBMI
```

### 테스트 실행
```bash
cd build
ctest
```


## 프로젝트 구조
```
CMakeLists.txt
shealth.dat
src/
  main/cpp/
    SHealth.h          - SHealth 클래스 헤더
    SHealth.cpp        - BMI 계산 및 통계 로직 구현
    SHealthBMI.cpp     - main 함수 (프로그램 진입점)
  test/cpp/
    SHealthBMITest.cpp - Google Test 기반 단위 테스트
```


# 생성형AI를 활용한 Activities (6 시간)

- [x] **1. 문제 코드 분석 및 코드 스멜 찾기** (1시간)
  - [x] 기본 코드구조, BMI 로직 이해
  - [x] 코드 스멜 찾기
- [x] **2. 1차 리펙토링** (클린코드 관점, 아래 내용을 순차적으로 수행) (1시간)
  - [x] 네이밍 개선
  - [x] 하드코드 및 전역변수 제거
  - [x] 함수 추출
  - [x] 반복/중복 제거
- [x] **3. UnitTest 작성** (1시간)
  - [x] BMI 계산 로직 TC
  - [x] Age 평균치 보정 로직 TC
  - [x] 정상/저체중/과체중/비만 분류 TC
  - [x] 예외상황 TC
- [x] **4. 기능 개선** (2시간)
  - [x] SRP에 따른 책임 분리등 리팩토링
  - [x] 특정 연령대의 BMI 분포 비율 계산 기능 추가
  - [x] Height가 0인 경우에 대한 평균치 보정 로직 추가
  - [x] BMI 정상 범위 사용자 목록 조회 기능 추가
  - [x] 전체 사용자 대비 각 BMI 범주 비율 계산 기능 추가
- [ ] **5. 회고 및 발표** (1시간)
  - [ ] 실습 목표와 달성도
  - [ ] 코드 품질 Before & After
  - [ ] AI를 어떻게 활용했나? 도움이 된 순간과 한계는?
  - [ ] TC를 추가해보면서 개선에 미친 영향, TC 작성 팁
  - [ ] 클린코드와 리팩토링에서 느낀 장점과 어려운점


# 주의 사항
- 코드 품질을 높이기 위해 C++의 경우 STL을 필요한 경우 사용하셔도 됩니다.


---

# 모던 C++ 리팩토링 로드맵

> **근거 문서**: `docs/code_quality_report.md`, `docs/requirements_analysis.md`  
> **기준일**: 2026-05-20 · **테스트 기준선**: `ctest` **24/24 Passed** (Activity 4 TC 포함)

## 제약 (모든 단계 공통)

| 제약 | 의미 |
|------|------|
| **배열 레이아웃 유지** | `ids[]` / `ages[]` / `heights[]` / `weights[]` / `bmis[]` + `count` 구조 유지. `HealthRecord`·`std::vector` 레코드 도입은 **Epic B**에서 별도 결정. |
| **커밋당 ~50줄** | 한 커밋 = 한 스멜 제거. 거대 리팩터링 금지. |
| **테스트 Green에서만 진행** | 각 커밋 후 빌드·테스트 **전부 통과**할 때만 다음 단계. 실패 시 revert 후 범위 축소. |

## 검증 명령 (모든 단계 공통)

```bash
cd build
cmake --build .
ctest --output-on-failure
```

최초 1회 (또는 `CMakeLists.txt` 변경 시):

```bash
mkdir build
cd build
cmake ..
cmake --build .
ctest --output-on-failure
```

**통과 기준**: `ctest` exit code 0, 실패 0건. Activity 4 회귀는 §A0 스모크 TC 11건 + 기존 13건을 함께 본다.

---

## Epic A — Activity 4 기능 리팩토링 (최근 구현, **현재 착수 대상**)

**대상 (가장 최근 구현)**  
Activity 4에서 추가·확장된 파이프라인 단계와 API:

| 구성 요소 | 파일 | 현재 스멜 |
|-----------|------|-----------|
| `imputeMissingHeights()` | `SHealth.cpp` | `imputeMissingWeights()`와 **거의 동일한 이중 루프** (Duplicated Code) |
| `aggregateGlobalRatios()` | `SHealth.cpp` | `aggregateRatiosByAgeBand()`와 **카테고리 카운트·% 변환 중복** |
| `getGlobalBmiRatio()` | `SHealth.cpp` | `lookupRatio()`와 **인덱스 검증·테이블 조회 패턴 중복** |
| `getNormalBmiUserIds()` | `SHealth.cpp` | `classifyBmi` 재사용은 양호; `reserve`·필터 정책 분리 여지 |
| `calculateBmi` 오케스트레이션 | `SHealth.cpp` | 6단계 파이프라인 — 단계 순서(체중→키→BMI) **정책 문서화** 필요 |

**핫스팟 (중복 블록)**

```95:149:src/main/cpp/SHealth.cpp
void SHealth::imputeMissingWeights() { /* 연령대 루프 × 2-pass 평균 */ }
void SHealth::imputeMissingHeights() { /* 동일 구조, heights / kMissingHeight */ }
```

```158:204:src/main/cpp/SHealth.cpp
void SHealth::aggregateRatiosByAgeBand() { /* inAgeBand 필터 + categoryCounts */ }
void SHealth::aggregateGlobalRatios()      { /* 전체 categoryCounts */ }
```

### A0 — 기준선 고정 (Green 확인) 【필수 선행】

| # | 작업 | 커밋 메시지 예 |
|---|------|----------------|
| A0.1 | `ctest` 24/24 Green 확인 (이미 Green이면 스킵) | — |
| A0.2 | Activity 4 TC 11건 목록을 팀 공유용으로 고정 | — |

**체크리스트**

- [x] `SHealthFixture` + `shealth.dat` smoke
- [x] `ImputeMissingHeight*` 4건, `GetNormalBmiUserIds*` 2건, `GetGlobalBmiRatio*` 5건
- [x] `cmake --build . && ctest` exit 0

**검증**: 위 공통 명령. **Green 아니면 A1 이후 진행 금지.**

---

### A1 — 도메인 상수 정리 (동작 동일)

**목표**: 매직 `4`, 반복 `category` 루프를 이름 있는 상수로 통일.

| # | 작업 | 파일 | diff ≤50 |
|---|------|------|----------|
| A1.1 | `namespace BmiCategoryConfig { constexpr int kCount = 4; }` | `SHealthConstants.h` | ✓ |
| A1.2 | `std::array<int, kCount>` / `for (int c = 0; c < kCount; ++c)` 치환 | `SHealth.cpp` 집계·루프 | ✓ |

**체크리스트**

- [x] `BmiTypeCode`·`BmiCategory` enum과 `kCount == 4` 정합
- [x] 연령대·전체 비율 수치 **변화 없음**

**검증**

```bash
cd build && cmake --build . && ctest --output-on-failure
```

회귀 포인트: `AgeBandRatiosSumNearOneHundred`, `GetGlobalBmiRatio_EachCategoryQuarterPercent`

---

### A2 — 결측 보정 정책 통합 (함수 분해 + 테이블 기반)

**목표**: 체중·키 보정의 **이중 3중 루프**를 하나의 정책으로 합친다. (전략 패턴 대신 **데이터 주도** — 필드 포인터 + sentinel)

| # | 작업 | 패턴 | 커밋 |
|---|------|------|------|
| A2.1 | `imputeMissingField(double* field, double sentinel)` private 추출 | **함수 분해** | `refactor: extract imputeMissingField` |
| A2.2 | `imputeMissingWeights` / `imputeMissingHeights` → 위 함수 1줄 위임 | **중복 제거** | `refactor: dedupe weight/height imputation` |
| A2.3 | (선택) `struct ImputePolicy { double* data; double sentinel; };` | **정책 구조체** | `refactor: impute policy struct` |

**스케치 (C++17)**

```cpp
void SHealth::imputeMissingField(double* values, double missingSentinel) noexcept;
// imputeMissingWeights()  → imputeMissingField(weights, kMissingWeight);
// imputeMissingHeights()  → imputeMissingField(heights, kMissingHeight);
```

**체크리스트**

- [x] 보정 순서 유지: **체중 → 키 → BMI** (`calculateBmi` 호출 순서 불변)
- [x] `imputeMissingWeights` / `imputeMissingHeights` → `imputeMissingField` thin wrapper
- [x] 연령대 격리·`ageCount==0` 스킵 동작 동일

**검증**: `ctest` — `ImputeMissingWeightByAgeBand`, `ImputeMissingHeight*`, `ImputeMissingHeightAndWeight_BothImputed` Green

---

### A3 — BMI 카테고리 집계 코어 추출 (중복 제거)

**목표**: 연령대별·전체 집계의 **count → percent** 로직을 한곳에 모은다.

| # | 작업 | 패턴 | 커밋 |
|---|------|------|------|
| A3.1 | `countCategories(Predicate include)` → `std::array<int, kCount>` | **함수 분해** | `refactor: extract countCategories` |
| A3.2 | `fillPercentages(counts, total, double* out)` 또는 `std::array<double,kCount>` 반환 | **테이블 기반** | `refactor: extract fillPercentages` |
| A3.3 | `aggregateRatiosByAgeBand` / `aggregateGlobalRatios`에서 위 함수만 호출 | **중복 제거** | `refactor: dedupe aggregation` |

**스케치**

```cpp
using RecordPredicate = bool (*)(int index, void* ctx);  // 또는 람다 + std::function (테스트 Green 후)
static std::array<int, BmiCategoryConfig::kCount> countCategories(
    const SHealth& self, RecordPredicate pred);
```

`aggregateRatiosByAgeBand`: `pred = inAgeBand(ages[i], bandStart)`  
`aggregateGlobalRatios`: `pred = always true`

**체크리스트**

- [x] `bandTotal == 0` / `count == 0` 시 0 나눗셈 **없음** (기존 `continue`/`return` 유지)
- [x] 6연령대×4분류 + 전체 4분류 **수치 불변**

**검증**: `TwentiesBand_*` ~ `SeventiesBand_*`, `GetGlobalBmiRatio_*`, `GetGlobalBmiRatio_DiffersFromSingleAgeBand`

---

### A4 — 비율 조회 API 통합 (조건 분기 축소)

**목표**: `lookupRatio` / `getGlobalBmiRatio`의 **인덱스 검증 + 배열 접근** 중복 제거.

| # | 작업 | 커밋 |
|---|------|------|
| A4.1 | `ratioAt(int catIdx, const double* table) const` private | `refactor: unify ratio lookup` |
| A4.2 | `getGlobalBmiRatio` → `ratioAt(catIdx, globalRatios_.data())` | 동일 커밋 또는 분리 |
| A4.3 | (선택) `getBmiRatio`도 동일 헬퍼 경유 | |

**체크리스트**

- [x] 잘못된 `type` → `0.0` (기존 TC 유지)
- [x] public 시그니처 `int type` **변경 없음**

**검증**: `GetBmiRatioInvalidArgumentsReturnZero`, `GetGlobalBmiRatio_InvalidTypeReturnsZero`

---

### A5 — 정상 BMI 목록 조회 정책 분리

**목표**: “정상 사용자 필터”를 **분류 정책(`classifyBmi`)과 조회 책임**으로 분리.

| # | 작업 | 패턴 | 커밋 |
|---|------|------|------|
| A5.1 | `isNormalBmi(double bmi)` → `classifyBmi(bmi) == BmiCategory::Normal` 위임 | **정책 함수** | `refactor: extract isNormalBmi` |
| A5.2 | `getNormalBmiUserIds`: `normalIds.reserve(count)` + `isNormalBmi` 사용 | **C++17** | `refactor: reserve normal user ids` |
| A5.3 | (선택) `static_assert`로 Normal 구간이 README와 일치 문서화 | | |

**체크리스트**

- [x] 경계 18.5·23 **제외** (`GetNormalBmiUserIds_ExcludesBoundaryValues`)
- [x] 빈 목록 동작 (`GetNormalBmiUserIds_EmptyWhenNone`)

**검증**: 위 2 TC + 전체 `ctest`

---

### A6 — 타입 안전 조회 (선택, API 호환)

**목표**: Primitive `int type`은 유지하되, 내부는 `BmiCategoryType` + 인덱스 테이블.

| # | 작업 |
|---|------|
| A6.1 | `getGlobalBmiRatio(BmiCategoryType type)` private 오버로드 |
| A6.2 | `getNormalBmiUserIds` 반환 타입 `std::vector<int>` 유지 |

**전략 패턴 vs 테이블**: 고정 4분류 → **테이블 + `classifyBmi`** 유지. 기준 변경 시에만 `IBmiClassifier` 검토.

**검증**: `ctest` Green

---

### A7 — 데모·마무리 (낮은 우선순위)

| # | 작업 | 파일 |
|---|------|------|
| A7.1 | `SHealthBMI.cpp`: 6연령대 `printf` → `for (band = 20; band <= 70; band += 10)` + `BmiTypeCode` | 출력만 |
| A7.2 | `split` → `std::string_view` 입력 (동작 동일) | `SHealth.cpp` |
| A7.3 | `aggregate*` / `classifyBmi`에 `noexcept`·`[[nodiscard]]` 정리 | |

**체크리스트**

- [ ] 프로덕션 API·수치 변경 없음 (A7.1은 stdout 포맷만 동일하면 OK)

**검증**: `ctest` Green; `SHealthBMI` 실행 육안 확인

---

### Epic A — 커밋 순서 요약

```
A0 Green 확인
  → A1 kCount 상수화
  → A2 imputeMissingField (체중/키 중복 제거)
  → A3 countCategories + fillPercentages (집계 중복 제거)
  → A4 ratioAt 조회 통합
  → A5 isNormalBmi + reserve
  → A6 enum 내부 오버로드 (선택)
  → A7 데모/C++17 polish
```

### Epic A — 단계별 체크 (복사용)

| Phase | Green? | `cmake --build` | `ctest` | 핵심 회귀 TC |
|-------|--------|-----------------|---------|----------------|
| A0 | ☑ | ☑ | ☑ | 24/24 (기준선) |
| A1 | ☑ | ☑ | ☑ | `BmiCategoryConfig::kCount` |
| A2 | ☑ | ☑ | ☑ | `imputeMissingField` |
| A3 | ☑ | ☑ | ☑ | `countCategories*` + `fillPercentages` |
| A4 | ☑ | ☑ | ☑ | `ratioAt` |
| A5 | ☑ | ☑ | ☑ | `isNormalBmi` + `reserve` |
| A6 | — | — | — | public API `int` 유지 (스킵) |
| A7 | ☑ | ☑ | ☑ | `SHealthBMI` 연령대 루프 |

---

## Epic B — 구조 개선 (Epic A 완료·Green 유지 후)

`docs/code_quality_report.md` 우선순위 3~4. Activity 4 API 동작은 유지한 채 인프라만 개선.

| 항목 | 내용 |
|------|------|
| B1 | `std::vector<HealthRecord>` — `kMaxRecords`·수동 `count` 제거 |
| B2 | `calculateBmi` 실패 구분: `std::optional` / 에러 코드 |
| B3 | CSV `split` → `namespace csv` 자유 함수 + `string_view` |
| B4 | `getBmiRatio` 실패 시 `0.0` vs “없음” — `std::optional<double>` |

---

## Epic C — `calculateBmi` / `getBmiRatio` 1차 리팩토링 【완료】

Activity 1~3 + 기반 리팩토링. **추가 작업 불필요** (회고·Before/After 참고용).

| Phase | 내용 | 상태 |
|-------|------|------|
| 0 | `FAIL()` 제거, smoke·경계 TC | ☑ |
| 1 | `SHealthConstants.h` 상수화 | ☑ |
| 2 | `classifyBmi`, README 경계 `≥25` | ☑ |
| 3 | `ageBandIndex`, `categoryIndex`, `inAgeBand` | ☑ |
| 4 | `ratiosByBand_` 2D 테이블, `lookupRatio` | ☑ |
| 5 | 파이프라인 private 분해 (SRP) | ☑ |
| 6 | `enum class AgeBand`, `BmiCategoryType` | ☑ |
| 7 | C++17 기본 적용 | ☑ (A7에서 추가 polish 가능) |

상세 분석: `docs/code_quality_report.md` · 요구사항: `docs/requirements_analysis.md`
