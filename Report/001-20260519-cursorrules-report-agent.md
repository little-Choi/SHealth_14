# 작업 보고서 — Cursor 규칙·보고서 Agent·1차 저장

- **보고서 번호**: 001
- **작성일**: 2026-05-19
- **브랜치**: refactoring
- **커밋(작성 시점)**: 2cb5a99 (보고서 커밋 전 기준)

## 1. 작업 개요

SHealth BMI C++17 TDD 프로젝트에서 Cursor AI가 따를 프로젝트 규칙 파일(`.cursorrules`)을 작성했습니다. 이어서 "보고서 작성 및 저장" 시 4단계(보고서·전체 대화·User 프롬프트·GitHub push)를 수행하는 `report-github-manager` 서브에이전트를 `.cursor/agent`에 정의하고, 보고서·프롬프트 저장용 폴더와 번호 자동 부여 스크립트를 추가했습니다. 사용자 요청에 따라 본 보고서(001번)와 프롬프트 아카이브를 생성하고 원격 저장소에 반영했습니다.

## 2. 변경·산출물 목록

| 구분 | 경로/항목 | 설명 |
|------|-----------|------|
| 규칙 | `.cursorrules` | C++17, CMake, GTest, lcov, TEST_F, Given-When-Then, 매직 넘버 상수화, git merge 금지 |
| 에이전트 | `.cursor/agent/report-github-manager.md` | 4단계 보고·저장·push 워크플로 정의 |
| 에이전트(인식) | `.cursor/agents/report-github-manager.md` | Cursor 서브에이전트 자동 인식용 동일 본문 |
| 스크립트 | `.cursor/agent/scripts/next-report-id.ps1` | Report/ 기준 다음 3자리 번호 산출 |
| 폴더 | `Report/`, `Prompting/`, `Prompting_user/` | 보고서·프롬프트 저장소 |
| 빌드 | `CMakeLists.txt` | Google Test FetchContent, coverage, gtest_discover_tests (기존 수정분) |
| 보고서 | `Report/001-20260519-cursorrules-report-agent.md` | 본 문서 |
| 프롬프트 | `Prompting/001-...-Prompt.md` | 세션 전체 대화 |
| User | `Prompting_user/001-...-User.md` | User 메시지만 |

## 3. 주요 결정·이슈

- `.cursorrules`에 README 도메인(BMI 임계값, 체중 0 보정, 연령대)과 Activity 체크리스트를 반영했습니다.
- Cursor 공식 경로는 `.cursor/agents/`이나, 사용자 요청 경로 `.cursor/agent/`에 본문을 두고 agents에도 동일 정의를 복제했습니다.
- `build/`, `_deps/` 등 빌드 산출물은 커밋 대상에서 제외했습니다.
- `git merge`는 수행하지 않았습니다.

## 4. 테스트·검증

| 명령 | 결과 |
|------|------|
| `next-report-id.ps1` | `001` 출력 확인 |
| `git status` / `git diff --stat` | 변경 파일 목록 확인 |
| `ctest` | 본 세션에서 미실행 (보고서·에이전트 작업 위주) |

## 5. 다음 단계

1. README Activity 1: 코드 스멜 분석 및 문서화
2. `SHealthBMITest.cpp` placeholder(`FAIL()`) 제거 후 TEST_F 기반 BMI·보정 TC 작성
3. 매직 넘버(18.5, 23, 25 등) `constexpr` 상수화 1차 리팩토링
