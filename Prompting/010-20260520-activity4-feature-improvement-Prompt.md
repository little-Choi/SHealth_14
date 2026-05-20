# 대화 프롬프트 기록 — 010-20260520-activity4-feature-improvement

- **보고서 번호**: 010
- **저장일**: 2026-05-20

---

## Turn 1

**User**

@README.md @src/test/cpp/SHealthBMITest.cpp @src/main/cpp/SHealth.cpp 

[P] 기능 구현에 능한 C++ QA 엔지니어입니다.
[T] README.md의 기능 개선 항목들을 구현해줘
    1) 해당 기능에 대한 TEST도 같이 작성
	2) 경계 값을 포함한 TEST 필요 시 추가 작성
	3) Given-When-Then 구조 사용
[F] 완성된 기능 코드 및 테스트 코드. 체크리스트 체크완료. cmake --build build && ctest가 Green이 되게 작성

**Assistant**

README Activity 4(기능 개선) 5항을 구현했습니다.

**구현 요약**

1. **SRP 파이프라인**: `loadFromCsv` → `imputeMissingWeights` → `imputeMissingHeights` → `computeAllBmis` → `aggregateRatiosByAgeBand` → `aggregateGlobalRatios`
2. **연령대 BMI 분포**: 기존 `getBmiRatio(ageClass, type)` + `GetBmiRatio_SpecificAgeBandDistribution` TC
3. **Height==0 보정**: `imputeMissingHeights()` (연령대 평균, 체중 보정과 대칭)
4. **정상 BMI 목록**: `getNormalBmiUserIds()` — CSV `id` 로드, `18.5 < BMI < 23`
5. **전체 비율**: `getGlobalBmiRatio(type)`

**신규 API**

```cpp
double getGlobalBmiRatio(int type) const;
std::vector<int> getNormalBmiUserIds() const;
```

**테스트**: Given-When-Then TC 11건 추가. `ImputeMissingHeight_DoesNotCrossAgeBands`는 키 160cm 시 BMI 과체중으로 실패 → 170cm로 수정 후 Green.

**검증**: `cmake --build build` 성공, `ctest` **24/24 Passed**. README Activity 4 체크리스트 `[x]` 반영.

---

## Turn 2

**User**

보고서 작성 및 저장 Agent 실행해줘.

**Assistant**

report-github-manager 워크플로 실행 — 보고서 010·프롬프트 저장 및 GitHub push.
