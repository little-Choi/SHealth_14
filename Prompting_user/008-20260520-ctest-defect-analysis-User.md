# User 프롬프트만 — 008-20260520-ctest-defect-analysis

- **보고서 번호**: 008
- **저장일**: 2026-05-20

---

## User Prompt 1

@src/test/cpp/SHealthBMITest.cpp @src/main/cpp/SHealth.cpp

[P] 디버깅과 결함 분석에 능한 C++ QA 엔지니어입니다.
[T] (여기에 ctest 실패 로그를 붙여넣을 것)
    1) EXPECT_EQ 실패의 기대/실제 차이 요약
    2) 버그 위치 특정 (파일명:줄번호)
    3) 결함 심각도 (Critical/Major/Minor/Info) 분류 및 근거
    4) 최소 변경 수정 방안 (C++17 스타일)
[F] 수정 diff 제안 + cmake --build build && ctest Green 확인 절차

## User Prompt 2

보고서 작성 및 저장 Agent 실행해줘
