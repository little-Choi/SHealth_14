# 작업 보고서 — SHealth 코드 품질 분석 (SOLID/Code Smell)

- **보고서 번호**: 002
- **작성일**: 2026-05-19
- **브랜치**: refactoring
- **커밋(작성 시점)**: 0e9b7f8 (보고서 커밋 전 기준)

## 1. 작업 개요

`SHealth.h` / `SHealth.cpp`의 `calculateBmi`, `getBmiRatio`, `split` 세 함수를 시니어 C++ 아키텍트 관점(SOLID, Code Smell, C++17)으로 정적 분석했습니다. SRP/OCP 위반, Magic Number, Long Method·중복 분기, BMI 경계 잠재 버그(BMI=25.0) 등을 식별하고, 테이블 기반 리팩토링·우선순위 1~5를 정리한 **`docs/code_quality_report.md`**(약 204줄)를 작성·저장했습니다. 본 보고서(002)와 세션 프롬프트 아카이브를 생성하고 GitHub에 반영합니다.

## 2. 변경·산출물 목록

| 구분 | 경로/항목 | 설명 |
|------|-----------|------|
| 분석 문서 | `docs/code_quality_report.md` | 문제점 표(11건), SOLID/Magic Number/스멜/C++17 개선·우선순위 |
| 보고서 | `Report/002-20260519-code-quality-analysis.md` | 본 문서 |
| 프롬프트 | `Prompting/002-20260519-code-quality-analysis-Prompt.md` | 세션 전체 대화 |
| User | `Prompting_user/002-20260519-code-quality-analysis-User.md` | User 메시지만 |
| 대상 소스 | `src/main/cpp/SHealth.h`, `SHealth.cpp` | 분석 대상(코드 변경 없음) |

## 3. 주요 결정·이슈

### 결정 사항

- 분석 산출물은 요청 경로 `docs/code_quality_report.md`에 저장(기능 코드 미변경).
- 리팩토링 우선순위 1순위: 24개 멤버 필드 → `std::array` 2D 테이블 + `getBmiRatio` 분기 제거.
- `split`은 로직은 양호, SHealth와의 결합 분리는 우선순위 5.

### 식별 이슈 (미해결)

| 이슈 | 설명 |
|------|------|
| BMI=25.0 경계 | 코드 `>25` vs README "25이상" — 미분류 가능 |
| 0 나눗셈 | `ageCount==0`, `sum==0` 미검사 |
| God Class | `calculateBmi` 다중 책임, 24 필드 + 24-way `getBmiRatio` |

## 4. 테스트·검증

| 명령 | 결과 |
|------|------|
| `next-report-id.ps1` | `002` 출력 |
| `git status` | `docs/` 미추적, `build/` 제외 예정 |
| `cmake` / `ctest` | 본 세션 분석·문서 작업만 수행, 미실행 |

## 5. 다음 단계

1. **1차 리팩토링**: 24 멤버 → `std::array<BmiDistribution, 6>` 테이블 통합 + 중복 루프 제거.
2. **상수화 + `classifyBmi()`**: BMI 18.5/23/25 경계 버그 수정 및 TEST_F 경계값 TC 추가.
3. **`calculateBmi` 단계 분리**: 로드·보정·계산·집계 private 메서드/클래스로 SRP 충족.
