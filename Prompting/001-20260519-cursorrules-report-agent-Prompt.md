# 대화 프롬프트 기록 — 001-20260519-cursorrules-report-agent

- **보고서 번호**: 001
- **저장일**: 2026-05-19

> agent-transcript(`e2f9b54d-...jsonl`) 기반. 도구 호출은 요약, User/Assistant 본문은 전문 또는 요약 병기.

---

## Turn 1

**User**

[P] 당신은 레거시 코드 QA/리팩토링을 돕는 시니어 C++ 엔지니어입니다.
[C] TDD TV C++17 프로젝트에서 Cursor AI가 항상 따라야 하는 규칙을 프로젝트 루트의 .cursorrules로 작성하려고 합니다.
[T] 아래 요구를 만족하는 .cursorrules 내용을 작성해줘. README.md 파일을 읽고 규칙을 추가해도 돼.
    - 기술 스택: C++17 + CMake + Google Test lcov
    - 테스트 규칙: Given-When-Then, TEST_F, 경계값 포함
    - 리팩토링 규칙: 매직 넘버 상수화 권장
    - git 규칙: git merge 진행하지 않기
[F] .cursorrules 파일을 만들기

**Assistant**

README·프로젝트 구조·기존 테스트를 확인한 뒤 `.cursorrules`를 작성했습니다.

- 프로젝트 개요: BMI 도메인, shealth.dat, 연령대 통계
- 기술 스택: C++17, CMake 3.10+, GTest v1.16.0, lcov/--coverage
- TDD: Red→Green→Refactor, TEST_F, Given-When-Then 주석, 경계값 TC
- 리팩토링: 매직 넘버 상수화, README Activity 순서
- Git: merge 금지, build/ 미커밋
- (도구: Read README, CMakeLists, SHealth.h/cpp, Grep TEST; Write `.cursorrules`)

---

## Turn 2

**User**

보고서 작성 및 프롬프트 저장, 깃 허브를 관리하는 Agent를 만들어줘.
.cursor/agent 에 생성
"보고서 작성 및 저장" 명령을 받으면
[1] 작업한 내용에 대한 보고서 작성 : Report 폴더에 파일번호가 들어간 .md 파일로 작성
[2] 작업한 모든 대화내용 저장 : Prompting 폴더에 -Prompt
[3] User 프롬프트만 : Prompting_user 폴더에 -User
[4] 깃허브에 모든 작업내용 업로드하기.

**Assistant**

`report-github-manager` 에이전트와 4단계 루틴을 구현했습니다.

- `.cursor/agent/report-github-manager.md` — 트리거, [1]~[4] 절차, 파일명 규칙
- `.cursor/agent/scripts/next-report-id.ps1` — Report/ 최대 번호+1
- `Report/`, `Prompting/`, `Prompting_user/` 폴더 생성
- `.cursor/agents/report-github-manager.md` — Cursor 자동 인식용 복제
- (도구: Cursor subagents 문서 조회, Write/StrReplace, 스크립트 검증 `001` 출력)

---

## Turn 3

**User**

응

**Assistant**

4단계 루틴 실행: 보고서 001 작성, Prompt/User 프롬프트 저장, git commit & push.

- (도구: next-report-id, git status/diff, transcript 읽기, Write Report/Prompting/Prompting_user, git add/commit/push)
