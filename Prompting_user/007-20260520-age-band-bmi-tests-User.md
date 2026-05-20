# User 프롬프트만 — 007-20260520-age-band-bmi-tests

- **보고서 번호**: 007
- **저장일**: 2026-05-20

---

## User Prompt 1

@src/test/cpp/SHealthBMITest.cpp @src/main/cpp/SHealthBMI.cpp @src/main/cpp/SHealth.h @src/main/cpp/SHealth.cpp @docs/requirements_analysis.md

[P] 테스트 설계에 강한 시니어 C++ QA입니다.
[C] C++17, Google Test
[T] 나이대, 체중대별 5개 TEST_F를 작성해줘.
    - 20대/30대/40대 등
    - EXPECT_EQ/ASSERT_EQ로 저체중/정상체중/과체중/비만 검증
    - 경계값 포함
    - Given-When-Then 주석 구조 사용
[F] 완성된 테스트 코드. cmake --build build && ctest가 Green이 되게 작성

## User Prompt 2

보고서 작성 및 저장 Agent 실행해줘.
