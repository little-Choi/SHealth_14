# 대화 프롬프트 기록 — 004-20260520-bmi-refactoring-roadmap

- **보고서 번호**: 004
- **저장일**: 2026-05-20

---

## Turn 1

**User**

@src/main/cpp/SHealth.h @src/main/cpp/SHealth.cpp @src/main/cpp/SHealthBMI.cpp @docs/code_quality_report.md @docs/requirements_analysis.md

[P] 모던 C++ 리팩토링 코치입니다.
[C] 제약: Item 구조체 수정 금지, quality 0~50, 테스트 Green에서만 진행
[T] calculateBmi, getBmiRatio 리팩토링 계획을 단계별로 제안해줘.
    - 조건 분기 축소/중복 제거 (커밋 단위로 쪼개기)
    - 타입/정책 분리 (전략 패턴/테이블 기반/함수 분해)
    - 매직 넘버 상수화와 C++17 스타일 개선
[F] 단계별 체크리스트 + 각 단계 검증 방법 (cmake --build && ctest)을 README.md에 저장

**Assistant**

관련 소스·문서·테스트를 확인한 뒤, `README.md`에 **Phase 0~7** 리팩토링 로드맵을 추가했습니다.

**주요 산출 내용**

| 항목 | 내용 |
|------|------|
| 제약 | Item 없음 → 배열 레이아웃 유지(Phase 0~7), 커밋당 ~50줄, Green 선행 |
| Phase 0 | `FAIL()` 제거 + smoke/경계 TC (필수 선행) |
| Phase 1~2 | 상수화, `classifyBmi` + 분류 테이블 |
| Phase 3~4 | 인덱스 헬퍼, 24필드 → `std::array` 2D, `getBmiRatio` 조회 단순화 |
| Phase 5~7 | `calculateBmi` 분해, `enum class`, C++17 polish |
| 검증 | 모든 단계 `cd build && cmake --build . && ctest --output-on-failure` |

**현재 상태**

- `SHealthBMITest.cpp`: `FailedTest` — `FAIL()` → **Red**
- `SHealth.cpp`: 미변경(계획만)

(도구: Read SHealth.h/cpp, SHealthBMI.cpp, code_quality_report.md, requirements_analysis.md, README.md, SHealthBMITest.cpp; StrReplace README.md +252줄)

---

## Turn 2

**User**

보고서 작성 및 저장 Agent 실행해줘.

**Assistant**

`report-github-manager` 4단계 워크플로 실행: Report/Prompting/Prompting_user 004번 산출물 생성 및 Git push.

(도구: next-report-id.ps1 → 004; git status; Write Report/Prompting/Prompting_user; git add/commit/push)
