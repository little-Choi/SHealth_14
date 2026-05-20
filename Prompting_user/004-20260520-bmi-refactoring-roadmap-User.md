# User 프롬프트만 — 004-20260520-bmi-refactoring-roadmap

- **보고서 번호**: 004
- **저장일**: 2026-05-20

---

## User Prompt 1

@src/main/cpp/SHealth.h @src/main/cpp/SHealth.cpp @src/main/cpp/SHealthBMI.cpp @docs/code_quality_report.md @docs/requirements_analysis.md

[P] 모던 C++ 리팩토링 코치입니다.
[C] 제약: Item 구조체 수정 금지, quality 0~50, 테스트 Green에서만 진행
[T] calculateBmi, getBmiRatio 리팩토링 계획을 단계별로 제안해줘.
    - 조건 분기 축소/중복 제거 (커밋 단위로 쪼개기)
    - 타입/정책 분리 (전략 패턴/테이블 기반/함수 분해)
    - 매직 넘버 상수화와 C++17 스타일 개선
[F] 단계별 체크리스트 + 각 단계 검증 방법 (cmake --build && ctest)을 README.md에 저장

---

## User Prompt 2

보고서 작성 및 저장 Agent 실행해줘.
