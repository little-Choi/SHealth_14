# 대화 프롬프트 기록 — 006-20260520-test-plan

- **보고서 번호**: 006
- **저장일**: 2026-05-20

---

## Turn 1

**User**

@src/main/cpp/SHealth.h @src/main/cpp/SHealth.cpp @src/test/cpp/SHealthBMITest.cpp @build/_deps/googletest-src/googlemock/include/gmock/internal/custom/README.md @docs/requirements_analysis.md

[P] 시니어 QA 리드입니다.
[C] C++17, Google Test, CMake, lcov
[T] 테스트 계획서를 작성해줘.
    - TEST_F 기반 단위 테스트 범위/우선순위
    - 경계값 케이스 목록
    - 예외/특이 케이스 목록
    - 커버리지 목표 (예: 90%+)와 lcov 측정·개선 전략
[F] Markdown 문서. docs 폴더에 test_plan.md로 저장

**Assistant**

참조 파일(`SHealth.h/cpp`, `SHealthBMITest.cpp`, `SHealthConstants.h`, `requirements_analysis.md`, `CMakeLists.txt`)을 검토한 뒤 `docs/test_plan.md`를 작성·저장했습니다.

**주요 산출 내용**

| 섹션 | 내용 |
|------|------|
| §1 목적·범위 | Public API·파이프라인·픽스처 In/Out Scope |
| §2 인프라 | `SHealthFixture`, `writeTempCsv`, Given-When-Then·`ctest` 통과 기준 |
| §3 TEST_F 범위·우선순위 | P0~P3, 현행 10건(T-001~010), 추가 예정 25건+(T-011~T-035), Sprint 순서 |
| §4 경계값 | BMI 18.5/23/25, 연령 19~80, 결측 체중, 비율 합 100% |
| §5 예외·특이 | E1~E8(파일/CSV), S1~S5(API), X1~X3, 회귀 R1~R4 |
| §6 Activity 4 | N1~N5 TDD 예약 |
| §7 lcov | ≥90% 목표, `SHEALTH_ENABLE_COVERAGE` 권장, 수집·필터·개선 매핑 |

**현행 Baseline 갭**

- P0 smoke·4분류·파일 오류: ✅ 10건
- BMI 직후/직전, 연령 경계, CSV 예외: 📋 예정

(도구: Read SHealth.h/cpp, SHealthBMITest.cpp, requirements_analysis.md, SHealthConstants.h, CMakeLists.txt; Grep lcov; Write `docs/test_plan.md`)

---

## Turn 2

**User**

보고서 작성 및 저장 Agent 실행해줘.

**Assistant**

`report-github-manager` 4단계 워크플로 실행: Report/Prompting/Prompting_user 006번 산출물 생성 및 Git push.

(도구: Read report-github-manager.md; next-report-id.ps1 → 006; git status; Write Report/Prompting/Prompting_user; git add/commit/push)
