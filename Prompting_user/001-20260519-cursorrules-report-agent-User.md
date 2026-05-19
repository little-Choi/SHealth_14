# User 프롬프트만 — 001-20260519-cursorrules-report-agent

- **보고서 번호**: 001
- **저장일**: 2026-05-19

---

## User Prompt 1

[P] 당신은 레거시 코드 QA/리팩토링을 돕는 시니어 C++ 엔지니어입니다.
[C] TDD TV C++17 프로젝트에서 Cursor AI가 항상 따라야 하는 규칙을
    프로젝트 루트의 .cursorrules로 작성하려고 합니다.
[T] 아래 요구를 만족하는 .cursorrules 내용을 작성해줘. README.md 파일을 읽고 규칙을 추가해도 돼.
    - 기술 스택: C++17 + CMake + Google Test lcov
    - 테스트 규칙: Given-When-Then, TEST_F, 경계값 포함
    - 리팩토링 규칙: 매직 넘버 상수화 권장
    - git 규칙: git merge 진행하지 않기
[F] .cursorrules 파일을 만들기

## User Prompt 2

보고서 작성 및 프롬프트 저장, 깃 허브를 관리하는 Agent를 만들어줘.
.cursor/agent 에 생성
"보고서 작성 및 저장" 명령을 받으면
[1] 작업한 내용에 대한 보고서 작성 : Report 폴더에 파일번호가 들어간 .md 파일로 작성
[2] 작업한 모든 대화내용 저장 : 현재까지의 프롬프트를 대화형 프롬프트로 Prompting 폴더에 보고서 파일명에 -Prompt를 붙여서 저장
[3] 대화내용 중 User 프롬프트 저장 : Prompting_user 폴더에 User 프롬프트만 추출하여 보고서 파일명에 -User를 붙여서 저장
[4] 깃허브에 모든 작업내용 업로드하기.
이상 4단계의 명령을 루틴하게 수행하는 Agent야.

## User Prompt 3

응
