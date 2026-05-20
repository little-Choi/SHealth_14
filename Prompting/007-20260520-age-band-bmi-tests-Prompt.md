# 대화 프롬프트 기록 — 007-20260520-age-band-bmi-tests

- **보고서 번호**: 007
- **저장일**: 2026-05-20

---

## Turn 1

**User**

@src/test/cpp/SHealthBMITest.cpp @src/main/cpp/SHealthBMI.cpp @src/main/cpp/SHealth.h @src/main/cpp/SHealth.cpp @docs/requirements_analysis.md

[P] 테스트 설계에 강한 시니어 C++ QA입니다.
[C] C++17, Google Test
[T] 나이대, 체중대별 5개 TEST_F를 작성해줘.
    - 20대/30대/40대 등
    - EXPECT_EQ/ASSERT_EQ로 저체중/정상체중/과체중/비만 검증
    - 경계값 포함
    - Given-When-Then 주석 구조 사용
[F] 완성된 테스트 코드. cmake --build build && ctest가 Green이 되게 작성

**Assistant**

`SHealth.cpp`, `SHealthConstants.h`, `requirements_analysis.md`, 기존 `SHealthBMITest.cpp`를 검토한 뒤 연령대별 5개 `TEST_F`를 구현했습니다.

**구현 요약**

- 헬퍼 `fourBmiBoundaryRows(age1..4)`: BMI 18.5 / 정상 / 23.0 / 25.0 경계 체중·키 4행 생성
- 헬퍼 `expectAllCategoriesQuarterPercent`: 4분류 각 `EXPECT_EQ(..., 25.0)`
- `TwentiesBand_*` ~ `SixtiesBand_AllBmiCategoriesAtBoundaries` 5건, Given-When-Then 주석
- 기존 `ClassifyObesityAtBmi25` 등 20대 단일 4건 제거·통합
- `cmake --build build` + `ctest`: **11/11 Passed**

(도구: Read SHealth*.cpp/h, requirements_analysis.md, SHealthBMITest.cpp; Write SHealthBMITest.cpp; cmake --build; ctest)

---

## Turn 2

**User**

보고서 작성 및 저장 Agent 실행해줘.

**Assistant**

`report-github-manager` 4단계 워크플로 실행: Report/Prompting/Prompting_user 007번 산출물 생성 및 Git push.

(도구: Read report-github-manager.md; next-report-id.ps1 → 007; git status/diff; Write Report/Prompting/Prompting_user; git add/commit/push)
