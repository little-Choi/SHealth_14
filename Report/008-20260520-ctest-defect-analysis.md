# 작업 보고서 — ctest 결함 분석 (BMI 경계 QA)

- **보고서 번호**: 008
- **작성일**: 2026-05-20
- **브랜치**: tc
- **커밋(작성 시점)**: a7f8545 (보고서 커밋 전 기준)

## 1. 작업 개요

`SHealthBMITest.cpp`·`SHealth.cpp`를 대상으로 C++ QA 관점의 **ctest 결함 분석**을 수행했습니다. 사용자가 ctest 실패 로그를 붙여넣지 않아, 워크스페이스에서 `ctest --output-on-failure`를 실행해 **11/11 Green**임을 확인한 뒤, 연령대별 경계 `TEST_F`가 검출하도록 설계된 **리팩터링 전 결함(BMI=25.0 비만 미분류)** 을 `git show fb91080` 및 `docs/code_quality_report.md`·`docs/test_plan.md`와 대조해 역분석했습니다. 분석 결과에는 EXPECT_EQ 기대/실제 차이, 버그 위치(`SHealth.cpp` `classifyBmi` / 구버전 `> 25`), 심각도(Major), 최소 수정안(`>= 25` 또는 `classifyBmi` 단일화), Green 확인 절차를 포함합니다. 현행 트리에는 수정이 이미 반영되어 있어 추가 소스 diff는 없습니다.

## 2. 변경·산출물 목록

| 구분 | 경로/항목 | 설명 |
|------|-----------|------|
| 분석 대상 | `src/test/cpp/SHealthBMITest.cpp` | 경계 5× `TEST_F`, `expectAllCategoriesQuarterPercent` |
| 분석 대상 | `src/main/cpp/SHealth.cpp` | `classifyBmi`, `aggregateRatiosByAgeBand` |
| 분석 대상 | `src/main/cpp/SHealthConstants.h` | `BmiThreshold` 상수 |
| 참조 | `docs/code_quality_report.md` | 구버전 `> 25` vs README `≥ 25` 불일치 |
| 참조 | `docs/test_plan.md` §4.1 | BMI 경계 매트릭스 B-B01~B-B13 |
| 구버전 | `git show fb91080:src/main/cpp/SHealth.cpp` | `else if (bmis[i] > 25)` 결함 확인 |
| 보고서 | `Report/008-20260520-ctest-defect-analysis.md` | 본 문서 |
| 프롬프트 | `Prompting/008-20260520-ctest-defect-analysis-Prompt.md` | 세션 전체 대화 |
| User | `Prompting_user/008-20260520-ctest-defect-analysis-User.md` | User 메시지만 |

## 3. 주요 결정·이슈

### 결함 요약 (리팩터링 전 예상 실패)

| 항목 | 내용 |
|------|------|
| 실패 테스트 | `SHealthFixture.*Band_AllBmiCategoriesAtBoundaries` (20~60대, 5건) |
| EXPECT_EQ | `getBmiRatio(ageClass, kObesity)` — 기대 `25.0`, 구버전 실제 `0.0` |
| 근본 원인 | BMI **25.0**이 `> 25` 조건에 걸리지 않아 4분류 누락 |
| 수정 위치(현행) | `SHealth.cpp:43-53` `classifyBmi()` — 비만 `bmi >= kOverweightMax`(25.0) |

### 심각도

| 등급 | 판정 | 근거 |
|------|------|------|
| **Major** | BMI=25.0 미분류 | 도메인 핵심 비율 오류, 집계 합 100% 왜곡 가능 |
| Minor (구버전) | `ageCount==0` 시 0 나눗셈 | 현행 `imputeMissingWeights`에서 스킵 처리 |
| Info | `EXPECT_EQ` vs 부동소수 | 경계 CSV는 정수 조합; 일반 데이터는 `EXPECT_NEAR` 검토 |

### 현행 상태

- **코드 수정**: Phase 2~7 리팩터링(보고서 005·007)으로 `classifyBmi` 단일화 완료.
- **ctest**: 2026-05-20 실행 기준 **11/11 Passed** — 추가 패치 불필요.

### 미해결·후속

| 이슈 | 설명 |
|------|------|
| 실패 로그 미첨부 | Red 로그 제공 시 1)~4) 항목을 로그 줄 단위로 재정리 가능 |
| P0 미세 경계 TC | `test_plan.md` T-011~ (18.51, 22.99, 24.99) 단독 TEST_F 미구현 |
| 70대 경계 TC | 007 보고서 후속 — 70대 1건 패턴 동일 추가 가능 |

## 4. 테스트·검증

| 명령 | 결과 |
|------|------|
| `ctest --output-on-failure` (build/) | **11/11 Passed** |
| `git show fb91080:src/main/cpp/SHealth.cpp` | 구버전 `> 25` 확인 |
| `next-report-id.ps1` | `008` (본 보고서) |

### Green 확인 절차 (재현용)

```powershell
cmake --build c:\DEV\SHealth_14\build
ctest --test-dir c:\DEV\SHealth_14\build --output-on-failure
```

단일 테스트:

```powershell
.\build\SHealthBMITest.exe --gtest_filter=SHealthFixture.TwentiesBand_AllBmiCategoriesAtBoundaries
```

## 5. 다음 단계

1. ctest **Red** 로그가 있으면 본 보고서 §3 표를 실제 `Expected`/`Actual`에 맞게 갱신.
2. `docs/test_plan.md` P0 경계 단독 TEST_F(T-011~) 추가.
3. 70대 `AllBmiCategoriesAtBoundaries` 1건 추가(007 후속).
