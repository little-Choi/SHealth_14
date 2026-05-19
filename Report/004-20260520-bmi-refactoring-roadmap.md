# 작업 보고서 — `calculateBmi` / `getBmiRatio` 모던 C++ 리팩토링 로드맵

- **보고서 번호**: 004
- **작성일**: 2026-05-20
- **브랜치**: refactoring
- **커밋(작성 시점)**: 2db8f09 (보고서 커밋 전 기준)

## 1. 작업 개요

`SHealth.h` / `SHealth.cpp`의 `calculateBmi`, `getBmiRatio`에 대해 `docs/code_quality_report.md`·`docs/requirements_analysis.md`를 근거로 **모던 C++ 리팩토링 단계별 계획**을 수립했습니다. 제약(Item/레코드 구조체 미도입, 커밋당 quality 0~50, 테스트 Green 선행)을 반영해 Phase 0~7로 커밋 단위를 쪼갰고, 각 단계별 체크리스트·`cmake --build` / `ctest` 검증 방법을 **`README.md`**에 저장했습니다. 프로덕션 소스(`SHealth.cpp` 등)는 변경하지 않았습니다.

## 2. 변경·산출물 목록

| 구분 | 경로/항목 | 설명 |
|------|-----------|------|
| 로드맵 | `README.md` | Phase 0~8+ 리팩토링 로드맵, 제약·검증 명령, 커밋 순서, 체크 표 (+252줄) |
| 참고 문서 | `docs/code_quality_report.md` | 분석 근거(기존) |
| 참고 문서 | `docs/requirements_analysis.md` | 도메인·경계값 근거(기존) |
| 보고서 | `Report/004-20260520-bmi-refactoring-roadmap.md` | 본 문서 |
| 프롬프트 | `Prompting/004-20260520-bmi-refactoring-roadmap-Prompt.md` | 세션 전체 대화 |
| User | `Prompting_user/004-20260520-bmi-refactoring-roadmap-User.md` | User 메시지만 |
| 대상 소스 | `src/main/cpp/SHealth.h`, `SHealth.cpp` | 계획 대상(코드 변경 없음) |

## 3. 주요 결정·이슈

### 결정 사항

- **Item 구조체 수정 금지**: 코드베이스에 `Item` 없음 → Phase 0~7 동안 `ages[]`/`heights[]`/`weights[]`/`bmis[]` + `count` 레이아웃 유지, `HealthRecord` 도입은 Phase 8+.
- **quality 0~50**: 커밋당 프로덕션 diff 약 50줄 이내, 스멜 1개씩 제거.
- **Phase 0 필수**: `SHealthBMITest.cpp`의 `FAIL()` placeholder 제거 후 Green 확보 — 현재 Red 상태에서 리팩토링 금지.
- **우선순위**: 상수화(1) → `classifyBmi`(2) → 인덱스 헬퍼(3) → 24필드 2D 테이블(4) → `calculateBmi` 분해(5) → `enum class`(6) → C++17 polish(7).

### 식별 이슈 (미해결)

| 이슈 | 설명 |
|------|------|
| 테스트 Red | `TEST(SHealthBMITest, FailedTest)` — `FAIL()` |
| BMI=25.0 | 코드 `>25` vs README `≥25` — Phase 2에서 TC 선행 후 수정 |
| 24-way 분기 | `getBmiRatio` — Phase 4에서 테이블 조회로 통합 예정 |

## 4. 테스트·검증

| 명령 | 결과 |
|------|------|
| `next-report-id.ps1` | `004` 출력 |
| `git status` | `README.md` 수정, `build/` 미추적(커밋 제외) |
| `cmake --build` / `ctest` | 본 세션 문서·계획 작업만 수행, 미실행 |

## 5. 다음 단계

1. **Phase 0**: `FAIL()` 제거, `TEST_F` smoke·경계 스냅샷 TC 추가 → `ctest` Green.
2. **Phase 1**: BMI·연령대·`type` 매직 넘버 `constexpr` 상수화(동작 동일).
3. **Phase 4**: 24 멤버 → `std::array` 2D + `getBmiRatio` 1줄 조회(가장 큰 중복 제거).
