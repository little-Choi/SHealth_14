# 작업 보고서 — Activities 1~3 점검·완료 및 UnitTest 보강

- **보고서 번호**: 009
- **작성일**: 2026-05-20
- **브랜치**: tc
- **커밋(작성 시점)**: 602a98b (보고서 커밋 전 기준)

## 1. 작업 개요

README.md의 **「생성형AI를 활용한 Activities」1~3항**을 코드·문서·테스트 기준으로 점검했습니다. 항목 1(코드 분석·스멜)과 항목 2(1차 리팩토링)는 기존 산출물(`docs/code_quality_report.md`, `docs/requirements_analysis.md`, `SHealthConstants.h`, 함수 분해·`ratiosByBand_` 등)로 이미 충족되어 체크만 반영했습니다. 항목 3(UnitTest)은 대부분 구현되어 있었으나 **70대 분류 TC**와 **BMI 공식 간접 검증 TC**가 없어 2건을 추가한 뒤 `ctest` 13/13 Green을 확인하고 README 하위 체크박스를 모두 `[x]`로 갱신했습니다.

## 2. 변경·산출물 목록

| 구분 | 경로/항목 | 설명 |
|------|-----------|------|
| 문서 | `README.md` | Activities 1~3 및 하위 11개 체크박스 `[x]` 처리 |
| 단위 테스트 | `src/test/cpp/SHealthBMITest.cpp` | +25줄; `SeventiesBand_*`, `GivenWeight70Height175_*` 추가 |
| 보고서 | `Report/009-20260520-activities-1-3-completion.md` | 본 문서 |
| 프롬프트 | `Prompting/009-20260520-activities-1-3-completion-Prompt.md` | 세션 전체 대화 |
| User | `Prompting_user/009-20260520-activities-1-3-completion-User.md` | User 메시지만 |

### Activities 1~3 점검 요약

| 항목 | 상태 | 근거 |
|------|------|------|
| 1. 코드 분석·스멜 | 완료(기존) | `docs/code_quality_report.md`, `docs/requirements_analysis.md` |
| 2. 1차 리팩토링 | 완료(기존) | 상수화, `classifyBmi`, private 파이프라인, `ratiosByBand_` |
| 3. UnitTest | 완료(보강) | 11→13건; 70대·BMI 공식 TC 추가 |

## 3. 주요 결정·이슈

### 결정 사항

- **기존 리팩토링 재작업 없음**: Phase 0~7 수준의 개선이 이미 반영되어 Activity 2는 검증·체크만 수행.
- **BMI 계산 TC**: `classifyBmi`는 private이므로 `weight=70, height=175` → 정상 100%로 공식·분류를 간접 검증 (`GivenWeight70Height175_WhenCalculate_ThenNormal100Percent`).
- **70대 TC**: `fourBmiBoundaryRows(70,73,76,79)` + `expectAllCategoriesQuarterPercent(health, 70)` — 6연령대 분류 커버 완성.

### 미해결·후속 이슈

| 이슈 | 설명 |
|------|------|
| Activity 4 | SRP 분리·height==0 보정·정상 BMI 목록·전체 비율 API — 미착수 |
| Activity 5 | 회고·발표 — 미착수 |
| test_plan P0 추가 | T-011~T-018 등 세부 경계 TC — 선택 보강 (`docs/test_plan.md`) |

## 4. 테스트·검증

| 명령 | 결과 |
|------|------|
| `cmake --build build` | `SHealthBMITest.cpp` 컴파일·링크 성공 |
| `ctest --output-on-failure` | **13/13 Passed** (기존 11 → +2) |
| `next-report-id.ps1` | `009` |

### TEST_F 목록 (13건)

| # | 테스트명 | Activity 3 매핑 |
|---|----------|-----------------|
| 1 | `SmokeShealthDat` | 통합 smoke |
| 2 | `GetBmiRatioInvalidArgumentsReturnZero` | 예외 |
| 3 | `AgeBandRatiosSumNearOneHundred` | 분류·집계 |
| 4–9 | `TwentiesBand_*` … `SeventiesBand_*` | 4분류 경계 (6연령대) |
| 10 | `GivenWeight70Height175_WhenCalculate_ThenNormal100Percent` | **BMI 계산** |
| 11 | `ImputeMissingWeightByAgeBand` | **Age 평균 보정** |
| 12–13 | 파일 없음 / 헤더만 | 예외 |

## 5. 다음 단계

1. **Activity 4**: README 4항 기능(연령대 분포·height==0 보정·정상 BMI 목록·전체 비율) TDD 착수.
2. **Activity 5**: Before/After·AI 활용 회고 보고서 초안 작성.
3. **lcov**: `SHEALTH_ENABLE_COVERAGE=ON` 후 `SHealth.cpp` 90% 게이트 측정 (`docs/test_plan.md`).
