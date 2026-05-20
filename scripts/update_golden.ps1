# Regenerate Golden Master approved files from current SHealth behavior.
# Usage (from repo root):
#   .\scripts\update_golden.ps1
#   .\scripts\update_golden.ps1 -Filter "SHealthGoldenFixture.GivenSixAgeBands*"

param(
    [string]$Filter = "SHealthGoldenFixture.*"
)

$ErrorActionPreference = "Stop"
$Root = Split-Path -Parent $PSScriptRoot
$Build = Join-Path $Root "build"

if (-not (Test-Path $Build)) {
    New-Item -ItemType Directory -Path $Build | Out-Null
    Push-Location $Build
    cmake ..
    Pop-Location
}

Push-Location $Build
cmake --build . --target SHealthBMITest
if ($LASTEXITCODE -ne 0) { exit $LASTEXITCODE }

$env:SHEALTH_UPDATE_GOLDEN = "1"
& ".\SHealthBMITest.exe" --gtest_filter=$Filter
$code = $LASTEXITCODE
Remove-Item Env:SHEALTH_UPDATE_GOLDEN -ErrorAction SilentlyContinue
Pop-Location

if ($code -ne 0) {
    Write-Error "Golden update run failed (exit $code)."
    exit $code
}

Write-Host "Approved files updated under src/test/golden/"
Write-Host "Review diff, then run: cd build; ctest --output-on-failure"
