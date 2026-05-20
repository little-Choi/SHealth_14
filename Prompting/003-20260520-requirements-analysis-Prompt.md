# 대화 프롬프트 기록 — 003-20260520-requirements-analysis

- **보고서 번호**: 003
- **저장일**: 2026-05-20

---

## Turn 1

**User**

@README.md

[P] 시니어 C++ QA 엔지니어 관점에서
[C] Gilded Rose C++17 프로젝트 (CMake, Google Test)
[T] 요구사항을 C++ 구현 관점에서 재정리해줘.
    1) 체중, 나이대 별별 비즈니스 규칙 표
       - 저체중/정상체중/과체중/비만, 20대/30대/40대 등
    2) 문자열 비교/분기 시 주의점 (std::string find/== 등)
    3) 예외/경계값 조건
    4) Conjured 신규 요구사항
    5) Google Test 기준 테스트 시나리오 목록 (번호)
[F] Markdown (표 + 번호 목록). docs 폴더에 requirements_analysis.md로 저장

**Assistant**

README·소스·테스트를 확인한 결과, 저장소는 Gilded Rose가 아닌 **SHealth BMI** 프로젝트임을 확인했습니다. `docs/requirements_analysis.md`를 작성·저장했습니다.

**주요 산출 내용**

| 섹션 | 내용 |
|------|------|
| §1 비즈니스 규칙 표 | BMI 공식, 4분류(경계 18.5/23/25), 20~70대 연령 구간, `getBmiRatio(ageClass, type)` 코드(100~400), 체중 0 보정 |
| §2 문자열/분기 | CSV·`split`·`stoi`/`stod`, `find`/`==` 함정, 실제 핵심은 정수 연령·`weight==0.0` 분기 |
| §3 예외·경계 | 파일 실패, `sum==0`·`ageCount==0` 0나눗셈, BMI=25 미분류 버그, height=0, 버퍼 10000 등 |
| §4 신규 요구 | README Activity 4 — height 0 보정, 정상 BMI 목록, 전체 비율, SRP 분리 (N1~N5) |
| §5 GTest 시나리오 | 번호 1~40 |

**구현 시 우선 확인할 불일치**

- README: 비만 **25 이상** → 코드: `bmis[i] > 25` → BMI=25.0 미분류 가능
- 연령대 집계 `sum==0` 또는 보정 시 `ageCount==0` → 0 나눗셈 위험
- 빈 CSV 줄: 현행 `break`로 이후 레코드 무시

(도구: Read README.md, SHealth.h/cpp, SHealthBMITest.cpp, .cursorrules; Grep; Write `docs/requirements_analysis.md`)

---

## Turn 2

**User**

보고서 작성 및 저장 Agent 실행해줘.

**Assistant**

`report-github-manager` 4단계 워크플로 실행: Report/Prompting/Prompting_user 003번 산출물 생성 및 Git push.

(도구: Read report-github-manager.md; next-report-id.ps1 → 003; git status; Write Report/Prompting/Prompting_user; git add/commit/push)
