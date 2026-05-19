# 작업 보고서 — SHealth BMI C++ 요구사항 분석 (QA 관점)

- **보고서 번호**: 003
- **작성일**: 2026-05-20
- **브랜치**: refactoring
- **커밋(작성 시점)**: e298756 (보고서 커밋 전 기준)

## 1. 작업 개요

`README.md` 및 `SHealth.h` / `SHealth.cpp`를 시니어 C++ QA 엔지니어 관점에서 검토하고, BMI·연령대·결측 보정·API·경계값·Activity 4 신규 기능을 **C++17 구현·Google Test** 관점으로 재정리한 **`docs/requirements_analysis.md`**를 작성·저장했습니다. Gilded Rose 템플릿 용어는 실제 도메인(SHealth BMI)에 맞게 매핑했으며, 테스트 시나리오 40건을 번호 목록으로 명시했습니다. 본 보고서(003)와 세션 프롬프트 아카이브를 생성하고 GitHub에 반영합니다.

## 2. 변경·산출물 목록

| 구분 | 경로/항목 | 설명 |
|------|-----------|------|
| 요구사항 문서 | `docs/requirements_analysis.md` | BMI/연령대 규칙 표, 문자열·분기 주의, 예외·경계, Activity 4 신규 요구, GTest 시나리오 1~40 |
| 보고서 | `Report/003-20260520-requirements-analysis.md` | 본 문서 |
| 프롬프트 | `Prompting/003-20260520-requirements-analysis-Prompt.md` | 세션 전체 대화 |
| User | `Prompting_user/003-20260520-requirements-analysis-User.md` | User 메시지만 |
| 참조(미변경) | `src/main/cpp/SHealth.*`, `README.md` | 분석 기준 소스 |

## 3. 주요 결정·이슈

### 결정 사항

- 산출물 경로: 요청대로 `docs/requirements_analysis.md`.
- §4 "신규 요구사항": README Activity 4(키 0 보정, 정상 BMI 목록, 전체 비율, SRP)로 정의 — Gilded Rose "Conjured" 아님.
- §5 테스트: `TEST_F`·Given-When-Then·경계 18.5/23/25·`getBmiRatio` type 100~400 반영.

### 식별 이슈 (문서화, 코드 미수정)

| 이슈 | 설명 |
|------|------|
| BMI=25.0 | 코드 `>25` vs README `≥25` — 미분류 가능 |
| 0 나눗셈 | `ageCount==0`, 연령대 `sum==0` 시 비율 계산 위험 |
| 빈 CSV 줄 | `tokens.empty()` 시 `break`로 이후 레코드 무시 |
| placeholder TC | `SHealthBMITest.cpp`의 `FAIL()` 테스트 잔존 |

## 4. 테스트·검증

| 명령 | 결과 |
|------|------|
| `next-report-id.ps1` | `003` 출력 |
| `git status` | `docs/requirements_analysis.md` 미추적, `build/` 제외 예정 |
| `cmake` / `ctest` | 본 세션 문서 작업만 수행, 미실행 |

## 5. 다음 단계

1. **§5 시나리오 1~11 우선**: BMI 계산·4분류 경계 TC (`TEST_F`) 추가, BMI=25.0 회귀 포함.
2. **체중 0 보정 TC**: 시나리오 18~22 (0 나눗셈 케이스 스펙 확정 후 assert).
3. **Activity 4**: height 0 보정·정상 사용자 목록 — 요구사항 문서 N1~N5 기준 TDD 구현.
