# Build and Test Script for C++ Raytracer
# PowerShell script to build and run tests

Write-Host "=== C++ Raytracer Build and Test ===" -ForegroundColor Cyan

# Step 1: Configure CMake
Write-Host "`nConfiguring CMake..." -ForegroundColor Yellow
cmake -B build -S .

if ($LASTEXITCODE -ne 0) {
    Write-Host "CMake configuration failed!" -ForegroundColor Red
    exit 1
}

# Step 2: Build the project
Write-Host "`nBuilding project..." -ForegroundColor Yellow
cmake --build build --config Release

if ($LASTEXITCODE -ne 0) {
    Write-Host "Build failed!" -ForegroundColor Red
    exit 1
}

# Step 3: Run tests
Write-Host "`nRunning tests..." -ForegroundColor Yellow
Set-Location build
ctest -C Release --output-on-failure --verbose

if ($LASTEXITCODE -ne 0) {
    Write-Host "`nSome tests failed!" -ForegroundColor Red
    Set-Location ..
    exit 1
}

Set-Location ..
Write-Host "`nAll tests passed successfully!" -ForegroundColor Green
