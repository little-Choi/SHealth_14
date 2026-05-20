# 작업 보고서 — Golden Master 회귀 테스트 설계·구현

- **보고서 번호**: 012
- **작성일**: 2026-05-20
- **브랜치**: feature
- **커밋(작성 시점)**: babc789 (보고서 커밋 전 기준)

## 1. 작업 개요

TexttestFixture 스타일 **출력 기반 Golden Master(Approval) 회귀 테스트**를 설계·구현했습니다. `shealth.dat` 기준 6×4 연령대 BMI 비율 및 전체(global) 비율을 고정 텍스트 스냅샷(`*.approved.txt`)과 비교하며, 리팩토링 시 의도치 않은 출력 변경을 `ctest`로 감지합니다. Google Test 비교 유틸, CMake/ctest 통합, CI 워크플로, 기대값 갱신 스크립트를 추가했고 **26/26 Passed**를 확인했습니다 (기존 단위 24 + Golden 2).

## 2. 변경·산출물 목록

| 구분 | 경로/항목 | 설명 |
|------|-----------|------|
| 테스트 | `src/test/cpp/SHealthGoldenTest.cpp` | T-027 `GivenSixAgeBands_WhenSnapshot_ThenMatchGoldenFile` 등 2건 |
| 테스트 | `src/test/cpp/golden/GoldenMaster.h` | 파일 읽기/쓰기, CRLF 정규화, 줄 diff, `SHEALTH_UPDATE_GOLDEN` |
| 테스트 | `src/test/cpp/golden/SHealthSnapshot.h` | CLI·전체 스냅샷 텍스트 포맷 (`printf %f` 동일) |
| Golden | `src/test/golden/shealth_full_snapshot.approved.txt` | 6연령대 + global + records 메타 |
| Golden | `src/test/golden/shealth_cli_age_bands.approved.txt` | `SHealthBMI` CLI 6줄 출력 |
| 빌드 | `CMakeLists.txt` | Golden 소스·include·`SHEALTH_*_DIR` 정의, `gtest_discover_tests` |
| CI | `.github/workflows/ci.yml` | Ubuntu + Windows(MSYS2 MinGW) `ctest` |
| 스크립트 | `scripts/update_golden.ps1`, `update_golden.sh` | approved 파일 일괄 재생성 |
| 문서 | `README.md` | Golden Master 실행·갱신·디렉터리 구조 |
| 기타 | `.gitignore` | `build/` 등 제외 |
| 보고서 | `Report/012-20260520-golden-master-regression-test.md` | 본 문서 |
| 프롬프트 | `Prompting/012-20260520-golden-master-regression-test-Prompt.md` | 세션 대화 |
| User | `Prompting_user/012-20260520-golden-master-regression-test-User.md` | User 메시지만 |

### Golden Master 전략 요약

| 항목 | 정책 |
|------|------|
| 기대 출력 | `src/test/golden/*.approved.txt` — Git 커밋 |
| 실패 시 diff | `build/test_output/golden/*.actual.txt` — 로컬만 |
| 갱신 | `SHEALTH_UPDATE_GOLDEN=1` + `SHealthGoldenFixture.*` 필터 |
| ctest cwd | `CMAKE_SOURCE_DIR` (프로젝트 루트, `shealth.dat` 접근) |

## 3. 주요 결정·이슈

### 결정 사항

- **프로덕션 코드 무변경**: 스냅샷 포맷터는 테스트 전용 헤더에만 두어 `SHealthBMI` 동작과 분리.
- **이중 Golden**: (1) 전체 스냅샷 T-027, (2) CLI 6줄 parity — 회귀 범위 분리.
- **정확 일치**: 줄 단위 exact match (TexttestFixture 관례); 부동소수는 `%f` 6자리로 고정.
- **CI 2 OS**: Ubuntu(g++) + Windows(MSYS2 MinGW) — 로컬 MinGW 환경과 정합.

### 이슈·해결

| 이슈 | 해결 |
|------|------|
| `getBmiRatio`가 non-const | 스냅샷 함수 인자에서 `const SHealth&` 제거 |
| 빌드 디렉터리에서 exe 직접 실행 시 `shealth.dat` 미발견 | `ctest`의 `WORKING_DIRECTORY` 사용 문서화 |
| global 비율 golden 초기값 오류 | actual 아티팩트로 수정 후 approved 갱신 |

### 미해결·후속

| 항목 | 설명 |
|------|------|
| 소형 fixture Golden | `test_*.csv` 시나리오별 approved 추가 (선택) |
| `getBmiRatio` const 정리 | Epic B와 함께 API 시그니처 개선 |

## 4. 테스트·검증

| 명령 | 결과 |
|------|------|
| `cmake --build build --target SHealthBMITest` | 성공 |
| `ctest --output-on-failure` | **26/26 Passed** |
| `ctest -R Golden --output-on-failure` | **2/2 Passed** |
| `next-report-id.ps1` | `012` |

## 5. 다음 단계

1. **Activity 5 회고**: Golden Master 도입이 리팩토링 안전망에 준 효과 정리.
2. **Epic A/C 리팩토링 시**: `ctest` Green(26건) 유지, 변경 시 `update_golden` 절차 준수.
3. **커버리지 게이트**: CI에 lcov 90% 측정 단계 추가 (선택).
