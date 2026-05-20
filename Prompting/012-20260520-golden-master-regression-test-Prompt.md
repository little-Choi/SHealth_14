# 대화 프롬프트 기록 — 012-20260520-golden-master-regression-test

- **보고서 번호**: 012
- **저장일**: 2026-05-20

---

## Turn 1

**User**

@src/main/cpp/SHealth.h @src/main/cpp/SHealth.cpp

[P] 회귀 테스트(Approval/Golden Master) 설계 전문가입니다.
[C] C++17, Google Test, CMake
[T] TexttestFixture 출력 기반 Golden Master를 설계/구현해줘.
    1) expected 출력 파일 생성/보관 전략
    2) Google Test에서 파일 비교 테스트 구현
    3) CMake/ctest에 통합하는 방법
    4) CI에서 자동 실행 구성
[F] 테스트 코드 + CMake 수정안 + 실행 방법

**Assistant**

Golden Master(Approval) 회귀 테스트를 설계·구현했습니다. **26/26** `ctest` 통과를 확인했습니다.

**1) Expected 출력 파일 전략**

- `src/test/golden/*.approved.txt` — Git 커밋
- `build/test_output/golden/*.actual.txt` — 실패 시 로컬 diff (`build/` gitignore)
- `SHEALTH_UPDATE_GOLDEN=1`로 의도된 변경 후만 갱신

**2) Google Test**

- `golden/GoldenMaster.h` — 읽기/쓰기, CRLF→LF, 줄 diff
- `golden/SHealthSnapshot.h` — 스냅샷 포맷터
- `SHealthGoldenTest.cpp` — T-027 `GivenSixAgeBands_WhenSnapshot_ThenMatchGoldenFile`, CLI parity 테스트

**3) CMake/ctest**

- `SHealthGoldenTest.cpp`를 `SHealthBMITest`에 링크
- `SHEALTH_PROJECT_ROOT`, `SHEALTH_GOLDEN_DIR` compile definition
- `gtest_discover_tests(... WORKING_DIRECTORY ${CMAKE_SOURCE_DIR})`

**4) CI**

- `.github/workflows/ci.yml` — Ubuntu + Windows(MSYS2 MinGW) `ctest`

**실행**: `cd build && ctest --output-on-failure` / `ctest -R Golden`

(도구: Read SHealth.h/cpp, CMakeLists, SHealthBMITest, test_plan; Write GoldenMaster.h, SHealthSnapshot.h, SHealthGoldenTest.cpp, approved.txt×2, ci.yml, scripts, .gitignore; StrReplace CMakeLists, README; Shell cmake build, ctest 26/26)

---

## Turn 2

**User**

보고서 작성 및 저장 Agent 실행해줘.

**Assistant**

`report-github-manager` 4단계 워크플로 실행: Report/Prompting/Prompting_user 012번 산출물 생성 및 Git push.

(도구: next-report-id.ps1 → 012; git status/diff; Write Report/Prompting/Prompting_user; git add/commit/push)
