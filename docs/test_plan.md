# SHealth BMI — 단위 테스트 계획서

> **역할**: 시니어 QA 리드  
> **대상**: `SHealth` (`src/main/cpp/SHealth.h`, `SHealth.cpp`)  
> **기술 스택**: C++17, Google Test v1.16.0, CMake 3.10+, lcov (GCC/Clang `--coverage`)  
> **참조**: `docs/requirements_analysis.md`, `src/test/cpp/SHealthBMITest.cpp`, `.cursorrules`  
> **문서 버전**: 2026-05-20

---

## 1. 목적·범위

### 1.1 목적

- BMI 산출·4분류·연령대별 비율·결측 체중 보정의 **도메인 규칙**을 `TEST_F` 단위 테스트로 고정한다.
- 경계값·예외 입력에 대한 **회귀 방지** 기준선을 확립한다.
- **lcov** 기반 라인 커버리지 **≥ 90%** (`SHealth.cpp` 기준)를 달성·유지한다.

### 1.2 범위 (In Scope)

| 구분 | 대상 |
|------|------|
| Public API | `calculateBmi`, `getBmiRatio` |
| 파이프라인 (간접 검증) | `loadFromCsv` → `imputeMissingWeights` → `computeAllBmis` → `aggregateRatiosByAgeBand` |
| 헬퍼 (간접·직접) | `classifyBmi`, `ageBandIndex`, `categoryIndex`, `inAgeBand`, `lookupRatio`, `split` |
| 픽스처 | `shealth.dat`, `writeTempCsv()` 임시 CSV |

### 1.3 범위 외 (Out of Scope — 별도 계획)

- `SHealthBMI.cpp` `main()` CLI 출력 형식 검증
- Google Mock / 외부 I/O 모킹 (현재는 실제 파일·임시 CSV 사용)
- README Activity 4 신규 API (`listNormalBmiUsers`, `getGlobalBmiRatio`, `height==0` 보정) — **구현 착수 시 본 문서 §6에 TC 추가**

---

## 2. 테스트 인프라

### 2.1 픽스처·헬퍼

```cpp
class SHealthFixture : public ::testing::Test {
protected:
    SHealth health;
};
```

| 헬퍼 | 용도 |
|------|------|
| `writeTempCsv(body)` | 테스트별 고유 임시 CSV (`test_<suite>_<name>.csv`) |
| `removeTempFile(path)` | TearDown 대용 정리 |
| `shealth.dat` | 통합 smoke·대용량 회귀 (CMake `WORKING_DIRECTORY` = 프로젝트 루트) |

### 2.2 작성 규칙

- 모든 BMI 도메인 TC는 **`TEST_F(SHealthFixture, …)`** 사용.
- **Given-When-Then** 주석 필수.
- 부동소수 비교: 비율 합계·BMI는 `EXPECT_NEAR` + 허용 오차 명시 (`1e-6` ~ `0.01` — 지표별 표준화).
- 네이밍: `Given<상태>_When<동작>_Then<기대>` (영문 snake_case).

### 2.3 실행·통과 기준

```bash
mkdir -p build && cd build
cmake ..
cmake --build .
ctest --output-on-failure
```

| 기준 | 값 |
|------|-----|
| `ctest` exit code | 0 |
| 실패 테스트 | 0건 |
| Phase 0 게이트 | Green 없이 리팩토링 Phase 진행 금지 (`README.md`) |

---

## 3. TEST_F 단위 테스트 — 범위·우선순위

### 3.1 우선순위 정의

| 등급 | 의미 | 릴리스 게이트 |
|------|------|---------------|
| **P0** | 핵심 API·경계·데이터 무결성 — 미구현 시 배포 불가 | 필수 Green |
| **P1** | 도메인 불변식·보정·통계 — 회귀 위험 높음 | 필수 Green |
| **P2** | CSV 엣지·다중 레코드·스냅샷 | 스프린트 내 Green |
| **P3** | Activity 4·리팩토링 후 단계별 TC | 기능별 TDD |

### 3.2 Public API 매트릭스

| API | 검증 포인트 | 우선순위 |
|-----|-------------|----------|
| `calculateBmi(filename)` | 반환 `count`, 파이프라인 전체, 파일 실패 시 0 | P0 |
| `getBmiRatio(ageClass, type)` | 유효 (20~70, 100~400), 무효 인자 0.0, 밴드 합 ≈ 100% | P0 / P1 |

### 3.3 테스트 케이스 목록

#### 3.3.1 현행 구현 TC (10건 — Baseline)

| ID | TEST_F 이름 | 등급 | 상태 | 검증 요약 |
|----|-------------|------|------|-----------|
| T-001 | `SmokeShealthDat` | P0 | ✅ | `shealth.dat` 로드 `count > 0`, 비율 유한·[0,100) |
| T-002 | `GetBmiRatioInvalidArgumentsReturnZero` | P0 | ✅ | `ageClass=25`, `type=500` → `0.0` |
| T-003 | `AgeBandRatiosSumNearOneHundred` | P1 | ✅ | 6연령대×4분류 합 ≈ 100% (`EXPECT_NEAR`, 0.01) |
| T-004 | `ClassifyObesityAtBmi25` | P0 | ✅ | BMI=25.0 → 비만 100% |
| T-005 | `ClassifyUnderweightAtBmi18_5` | P0 | ✅ | BMI=18.5 → 저체중 100% |
| T-006 | `ClassifyNormalBetweenBoundaries` | P0 | ✅ | 정상 구간 단일 표본 100% |
| T-007 | `ClassifyOverweightAtBmi23` | P0 | ✅ | BMI=23.0 → 과체중 100% |
| T-008 | `ImputeMissingWeightByAgeBand` | P1 | ✅ | `weight==0` → 동일 20대 평균 보정 |
| T-009 | `CalculateBmiMissingFileReturnsZero` | P0 | ✅ | 파일 없음 → `count==0` |
| T-010 | `HeaderOnlyFileReturnsZeroCount` | P0 | ✅ | 헤더만 → `count==0` |

#### 3.3.2 추가 예정 TC — P0 (필수)

| ID | 제안 TEST_F 이름 | Given | When | Then |
|----|------------------|-------|------|------|
| T-011 | `GivenBmi18_51_WhenClassify_ThenNormal100Pct` | 단일 20대, BMI≈18.51 | `calculateBmi` | `getBmiRatio(20, 200)==100` |
| T-012 | `GivenBmi22_99_WhenClassify_ThenNormal100Pct` | BMI≈22.99 | 동일 | 정상 100% |
| T-013 | `GivenBmi24_99_WhenClassify_ThenOverweight100Pct` | BMI≈24.99 | 동일 | 과체중 100% |
| T-014 | `GivenAge20_WhenAggregate_ThenIncludedIn20Band` | age=20 | 소규모 CSV | 20대 집계에 포함 |
| T-015 | `GivenAge29_WhenAggregate_ThenIncludedIn20Band` | age=29 | 동일 | 20대 포함 |
| T-016 | `GivenAge30_WhenAggregate_ThenIncludedIn30Band` | age=30 | 동일 | 30대 100%, 20대 0% |
| T-017 | `GivenKnownTwoUsers_WhenCalculate_ThenRatioMatchesManual` | 2명, 서로 다른 분류 | `calculateBmi` | 수동 계산 %와 `EXPECT_NEAR` |
| T-018 | `GivenWeight70Height175_WhenCalculate_ThenBmiNear22_86` | 표준 입력 | 파이프라인 | BMI≈22.86 (간접: 정상 100%) |

#### 3.3.3 추가 예정 TC — P1

| ID | 제안 TEST_F 이름 | 검증 요약 |
|----|------------------|-----------|
| T-021 | `GivenThreeWeights50_70_0_WhenImpute_ThenMissingBecomes60` | 20대 3명, 평균 60.0 보정 |
| T-022 | `Given30sMissingWeight_WhenImpute_ThenUses30BandOnly` | 30대 결측이 20대 평균에 영향 없음 |
| T-023 | `GivenAge19_WhenAggregate_ThenExcludedFromAllBands` | 19세 → 6밴드 합 0% (현행 설계) |
| T-024 | `GivenAge80_WhenAggregate_ThenExcludedFrom70Band` | 80세 → 70대 제외 |
| T-025 | `GivenEmptyAgeBand_WhenGetRatio_ThenZeroNoDivide` | 해당 연령대 인원 0 → 조회 0, 크래시 없음 |
| T-026 | `GivenAllWeightsZeroInBand_WhenImpute_ThenNoDivideByZero` | 전원 `weight==0` → 보정 스킵, 안전 종료 |
| T-027 | `GivenSixAgeBands_WhenSnapshot_ThenMatchGoldenFile` | `shealth.dat` 6×4 비율 스냅샷 (리팩토링 회귀) |

#### 3.3.4 추가 예정 TC — P2 (CSV·파싱)

| ID | 제안 TEST_F 이름 | 검증 요약 |
|----|------------------|-----------|
| T-031 | `GivenRowWith3Fields_WhenLoad_ThenThrowOrSkip` | 정책 확정 후 `EXPECT_THROW` 또는 스킵 |
| T-032 | `GivenNonNumericAge_WhenLoad_ThenThrowOrSkip` | `stoi` 예외 정책 |
| T-033 | `GivenBlankLineInMiddle_WhenLoad_ThenDocumentedBehavior` | 현행: `break`로 이후 행 무시 — TC로 고정 |
| T-034 | `Given10001Records_WhenLoad_ThenNoOverflow` | `kMaxRecords` 경계 (정책: 거부 vs vector 전환) |
| T-035 | `GivenHeightZero_WhenCalculate_ThenNaNOrHandled` | B1 — Activity 4 전까지 **문서화된 현행** 기록 |

### 3.4 Private 로직 — 테스트 접근 전략

| 함수 | 접근 방법 | 우선순위 |
|------|-----------|----------|
| `classifyBmi` | `getBmiRatio` + 단일 표본 CSV로 4분류 유도 | P0 |
| `ageBandIndex` / `categoryIndex` | 무효 인자 TC (T-002) + 유효 밴드 순회 (T-003) | P0 |
| `inAgeBand` | 연령 경계 TC (T-014~016, T-023~024) | P1 |
| `split` | CSV 파싱 TC 간접; 필요 시 `friend` 또는 `csv::split` free 함수 분리 후 직접 TC | P2 |
| `loadFromCsv` | 파일·헤더·결측 TC | P0 |
| `imputeMissingWeights` | T-008, T-021~022, T-026 | P1 |
| `aggregateRatiosByAgeBand` | T-003, T-017, T-027 | P1 |

> **권장**: 테스트 전용 `#ifdef SHealth_TEST_FRIENDS` 또는 `detail` 네임스페이스 노출은 **최후 수단**. 현재는 public API 관찰 가능성으로 충분.

### 3.5 우선순위별 실행 순서 (스프린트)

```
Sprint 0 (완료) : T-001 ~ T-010 Green
Sprint 1        : T-011 ~ T-018 (경계·연령 P0)
Sprint 2        : T-021 ~ T-027 (보정·불변식 P1)
Sprint 3        : T-031 ~ T-035 (CSV·엣지 P2) + lcov 90% 달성
Sprint 4+       : §6 Activity 4 (TDD)
```

---

## 4. 경계값 케이스 목록

### 4.1 BMI 분류 경계 (`BmiThreshold`)

구현 기준 (`SHealth.cpp` `classifyBmi`):

| 구간 | 조건 (현행) | `type` |
|------|-------------|--------|
| 저체중 | `bmi ≤ 18.5` | 100 |
| 정상 | `18.5 < bmi < 23` | 200 |
| 과체중 | `23 ≤ bmi < 25` | 300 |
| 비만 | `bmi ≥ 25` | 400 |

| Case ID | BMI | 기대 분류 | TC ID | 비고 |
|---------|-----|-----------|-------|------|
| B-B01 | 18.5 | 저체중 | T-005 | 경계 포함 (≤) |
| B-B02 | 18.5000001 | 정상 | T-011 | 직후 |
| B-B03 | 22.999… | 정상 | T-012 | 상한 직전 |
| B-B04 | 23.0 | 과체중 | T-007 | 하한 포함 |
| B-B05 | 24.999… | 과체중 | T-013 | 상한 직전 |
| B-B06 | 25.0 | 비만 | T-004 | README 정합 (≥25) |
| B-B07 | 30.0 | 비만 | (신규) | 원거리 sanity |

**체중·키 조합 예시** (height cm, weight kg → BMI):

| Case ID | weight | height | BMI (근사) | 기대 |
|---------|--------|--------|------------|------|
| B-B10 | 66.785 | 190 | 18.5 | 저체중 |
| B-B11 | 70.0 | 175 | ~22.86 | 정상 |
| B-B12 | 79.571 | 186 | 23.0 | 과체중 |
| B-B13 | 100.0 | 200 | 25.0 | 비만 |

### 4.2 연령대 경계 (`AgeBandConfig`)

| Case ID | age | bandStart | `inAgeBand` | 집계 포함 밴드 |
|---------|-----|-----------|-------------|----------------|
| B-A01 | 19 | 20 | false | 없음 |
| B-A02 | 20 | 20 | true | 20대 |
| B-A03 | 29 | 20 | true | 20대 |
| B-A04 | 30 | 20 | false | 30대 (30 밴드) |
| B-A05 | 30 | 30 | true | 30대 |
| B-A06 | 69 | 60 | true | 60대 |
| B-A07 | 70 | 70 | true | 70대 |
| B-A08 | 79 | 70 | true | 70대 |
| B-A09 | 80 | 70 | false | 없음 |

**API `ageClass` 경계**:

| Case ID | ageClass | 기대 |
|---------|----------|------|
| B-AC01 | 20, 30, …, 70 | 유효, `ageBandIndex` ≥ 0 |
| B-AC02 | 19, 15 | 무효 → `getBmiRatio` = 0 |
| B-AC03 | 25, 35, 80 | 무효 (10 단위 아님) → 0 |
| B-AC04 | 71 | 무효 → 0 |

### 4.3 결측·신체 계측

| Case ID | weight | height | 기대 (현행/목표) |
|---------|--------|--------|------------------|
| B-M01 | 0.0 | >0 | 동일 밴드 평균 보정 (T-008) |
| B-M02 | 0.0 × N (밴드 전원) | >0 | 보정 스킵, 0 나눗셈 없음 (T-026) |
| B-M03 | -0.0 | >0 | 결측 **아님** (부호만 음수 0) |
| B-M04 | 1e-300 | >0 | 결측 아님, BMI 계산 |
| B-M05 | >0 | 0.0 | BMI ∞/NaN — Activity 4 (N2) |
| B-M06 | >0 | <0 | 비정의 — 스펙 확정 후 TC |

### 4.4 통계·비율 불변식

| Case ID | 조건 | 기대 |
|---------|------|------|
| B-S01 | 밴드 인원 ≥ 1 | 4분류 합 ≈ 100% (±0.01) |
| B-S02 | 밴드 인원 0 | 전 type 0%, 나눗셈 없음 |
| B-S03 | 미분류 인원 0 (현행 classify 정합 시) | 합 = 100% 정확 |
| B-S04 | 단일 표본 | 해당 분류 100%, 나머지 0 |

---

## 5. 예외·특이 케이스 목록

### 5.1 파일·CSV (E계열)

| ID | 조건 | 현행 동작 | 기대 TC | 우선순위 |
|----|------|-----------|---------|----------|
| E1 | 파일 없음 | `cerr`, `return 0` | T-009 | P0 |
| E2 | 헤더만 | `count==0` | T-010 | P0 |
| E3 | 레코드 > 10000 | 버퍼 오버플로 위험 | T-034 | P2 |
| E4 | 필드 수 ≠ 4 | `stoi`/`stod` 예외 | T-031 | P2 |
| E5 | 비숫자 토큰 | 예외 전파 | T-032 | P2 |
| E6 | 데이터 중간 빈 줄 | `tokens.empty()` → **`break`** (이후 무시) | T-033 | P2 |
| E7 | 공백 포함 토큰 `" 66"` / `"66 "` | trim 없음 — 파싱 실패 가능 | (문서화) | P3 |
| E8 | 연속 호출 `calculateBmi` | `ratiosByBand_.fill({})`로 이전 결과 초기화 | (신규) | P1 |

### 5.2 API·논리 (S계열)

| ID | 조건 | 현행 동작 | TC | 우선순위 |
|----|------|-----------|-----|----------|
| S1 | `getBmiRatio` 전 `calculateBmi` 미호출 | ratios 0 | (신규) | P1 |
| S2 | 잘못된 `ageClass` | `0.0` (오류 은닉) | T-002 | P0 |
| S3 | 잘못된 `type` | `0.0` | T-002 | P0 |
| S4 | 실패(0%) vs 무효 인자(0%) | 구분 불가 | 문서화; 향후 `optional` | P3 |
| S5 | `calculateBmi` 실패(0) vs 빈 데이터(0) | 구분 불가 | 문서화 | P3 |

### 5.3 C++ 예외·프로세스

| ID | 조건 | 테스트 기법 | 우선순위 |
|----|------|-------------|----------|
| X1 | `std::stoi` / `std::stod` 실패 | `EXPECT_THROW` (정책: 전파 시) | P2 |
| X2 | 손상된 CSV 1행 | 전체 중단 vs 행 스킵 — **팀 정책 1개로 고정** | P2 |
| X3 | 테스트 임시 파일 잔류 | `removeTempFile` — 실패 시 CI 아티팩트 정리 | P1 |

### 5.4 특이·회귀 시나리오

| ID | 설명 | 대응 TC |
|----|------|---------|
| R1 | 리팩토링 전후 `shealth.dat` 6×4 스냅샷 | T-027 |
| R2 | `classifyBmi` 추출 후 경계 6건+ Green | T-004~007, T-011~013 |
| R3 | 24분기 → 테이블 리팩토링 후 API 동일 | T-001, T-003, T-027 |
| R4 | Windows MinGW + `--coverage` | §7 lcov 워크플로 |

---

## 6. Activity 4 — 신규 기능 TC (TDD 예약)

구현 시 **테스트 선행**으로 아래를 `SHealthBMITest.cpp`에 추가한다.

| ID | 기능 | TEST_F (제안) | 핵심 검증 |
|----|------|---------------|-----------|
| N1 | 특정 연령대 분포 | `Given40BandOnly_When…` | 40대 4합 ≈ 100% |
| N2 | `height==0` 보정 | `GivenMissingHeight_WhenImpute_Then…` | 체중 보정과 동일 밴드 평균 패턴 |
| N3 | 정상 BMI 사용자 목록 | `GivenMixedBmis_WhenListNormal_ThenIds…` | 18.5·23 경계 제외 |
| N4 | 전체 비율 API | `GivenSameFile_WhenGlobalVsBand_Then…` | 전체 합 100%, 연령대와 값 상이 가능 |
| N5 | SRP 분리 | 단계별 `TEST_F` | mock CSV로 load/impute/aggregate 독립 검증 |

---

## 7. 커버리지 목표 및 lcov 전략

### 7.1 목표

| 메트릭 | 대상 파일 | 목표 | 게이트 |
|--------|-----------|------|--------|
| **라인 커버리지** | `src/main/cpp/SHealth.cpp` | **≥ 90%** | PR 머지 전 필수 |
| 함수 커버리지 | `SHealth.cpp` | ≥ 85% | 권장 |
| 분기 커버리지 | `classifyBmi`, `ageBandIndex`, `categoryIndex`, `loadFromCsv` | ≥ 80% | 권장 |

**측정 제외 (lcov `--remove`)**:

- `src/main/cpp/SHealthBMI.cpp` (CLI)
- `src/test/**`, `build/**`, `**/googletest/**`
- 시스템 헤더 (`/usr/*`, `*/mingw64/*` 등)

### 7.2 빌드 설정 (현행·개선)

**현행** (`CMakeLists.txt`):

- `SHealthBMITest`에만 `--coverage` 적용.
- `shealth_lib`(`SHealth.cpp`)는 커버리지 플래그 **미적용** → `.gcda`가 라이브러리에 생성되지 않을 수 있음.

**권장 개선** (커버리지 정확도):

```cmake
option(SHEALTH_ENABLE_COVERAGE "Enable gcov/lcov" OFF)
if(SHEALTH_ENABLE_COVERAGE AND CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
  target_compile_options(shealth_lib PRIVATE --coverage -O0 -g)
  target_link_options(shealth_lib PRIVATE --coverage)
  target_compile_options(SHealthBMITest PRIVATE --coverage -O0 -g)
  target_link_options(SHealthBMITest PRIVATE --coverage)
endif()
```

- 측정 시 `-O0`으로 분기 왜곡 최소화.
- `SHealthBMITest` 실행 전 **기존 `.gcda` 삭제**로 stale 방지.

### 7.3 lcov 측정 워크플로 (GCC/MinGW)

```bash
# 1. 커버리지 ON으로 구성
cd build
cmake .. -DSHEALTH_ENABLE_COVERAGE=ON
cmake --build .

# 2. 이전 프로파일 제거
find . -name '*.gcda' -delete

# 3. 테스트 실행 (프로젝트 루트 cwd)
ctest --output-on-failure

# 4. lcov 수집·필터
lcov --capture --directory . --output-file coverage.info
lcov --remove coverage.info \
  '*/test/*' '*/googletest/*' '*/usr/*' '*/mingw64/*' \
  --output-file coverage.filtered.info

# 5. 리포트
genhtml coverage.filtered.info --output-directory coverage_html
lcov --summary coverage.filtered.info
```

**Windows 참고**: lcov/genhtml은 MSYS2·WSL·CI Linux에서 실행하는 것을 권장. 로컬 MinGW만 있을 경우 CI에서 커버리지 게이트 적용.

### 7.4 커버리지 개선 전략

| 단계 | 미커버 예상 영역 | 대응 TC |
|------|------------------|---------|
| 1 | `loadFromCsv` 오류 분기 (`!file.is_open`) | T-009 ✅ |
| 2 | `ageBandIndex` / `categoryIndex` `default` | T-002 ✅, T-AC02~04 |
| 3 | `aggregateRatiosByAgeBand` `bandTotal==0` continue | T-025 |
| 4 | `imputeMissingWeights` `ageCount==0` continue | T-026 |
| 5 | `loadFromCsv` while 루프 `tokens.empty()` break | T-033 |
| 6 | `split` 루프 전체 | T-031 (빈 필드 CSV) |
| 7 | `calculateBmi` early `count==0` | T-010 ✅ |

**개선 프로세스 (PR 단위)**:

1. `lcov --summary`로 **미커버 라인 목록** 확인 (`genhtml`에서 빨간 줄).
2. 해당 분기를 타는 **최소 TC 1건** 추가 (Red → Green).
3. 커버리지 **≥ 90%** 재확인 후 머지.
4. 회귀: T-001, T-003, T-027 스냅샷 유지.

### 7.5 커버리지와 TC 우선순위 매핑

| 커버리지 구간 | 목표 % | 주요 TC |
|---------------|--------|---------|
| `classifyBmi` | 100% | T-004~007, T-011~013 |
| `getBmiRatio` / `lookupRatio` | 100% | T-002, T-003 |
| `calculateBmi` | 100% | T-001, T-009, T-010 |
| `loadFromCsv` | ≥ 85% | T-009~010, T-031~033 |
| `imputeMissingWeights` | ≥ 90% | T-008, T-021~022, T-026 |
| `aggregateRatiosByAgeBand` | ≥ 90% | T-003, T-014~017, T-025 |
| `split` | ≥ 80% | CSV TC |

---

## 8. 추적·보고

### 8.1 TC ↔ 요구사항 추적

| 요구사항 문서 | 테스트 계획 |
|---------------|-------------|
| `docs/requirements_analysis.md` §1.2 | §4.1 BMI 경계 |
| §1.3 연령대 | §4.2 |
| §1.5 결측 체중 | §4.3, T-008, T-021~022 |
| §3 예외·경계 | §5 |
| §5 GTest 시나리오 | §3.3 |

### 8.2 Definition of Done (단위 테스트)

- [ ] P0 TC 전부 Green (`ctest`)
- [ ] BMI 경계 6점 + 연령 경계 4점 이상 TC 존재
- [ ] `SHealth.cpp` lcov 라인 커버리지 **≥ 90%**
- [ ] Given-When-Then 주석 및 `TEST_F` 준수
- [ ] `docs/requirements_analysis.md`와 분류 경계 불일치 0건

### 8.3 산출물

| 산출물 | 경로 |
|--------|------|
| 테스트 코드 | `src/test/cpp/SHealthBMITest.cpp` |
| 커버리지 HTML | `build/coverage_html/` (로컬/CI) |
| 본 계획서 | `docs/test_plan.md` |

---

## 부록 A — `getBmiRatio` 유효 인자 조합 (샘플링)

전수 24조합 중 **대표 샘플** (회귀 스모크):

| ageClass | type | 검증 |
|----------|------|------|
| 20 | 100, 200, 300, 400 | T-001, T-003 |
| 70 | 400 | (신규) 원거리 밴드 |
| 25 | 100 | T-002 무효 |
| 20 | 500 | T-002 무효 |

## 부록 B — 참고 명령·문서

- 요구사항: `docs/requirements_analysis.md`
- 코드 품질: `docs/code_quality_report.md`
- TDD 규칙: `.cursorrules`
- 빌드·Phase: `README.md`

*본 문서는 `SHealth.cpp` (2026-05-20) 및 `SHealthBMITest.cpp` 10건 Baseline 기준으로 작성되었습니다.*
