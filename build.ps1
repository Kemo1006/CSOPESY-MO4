# Build + run: .\build.ps1   (add -NoRun to only build)
param([switch]$NoRun)

# Prefer MSYS2 UCRT64 g++ (C++17 support + glob expansion) over older compilers.
if (Test-Path "C:\msys64\ucrt64\bin") {
    $env:PATH = "C:\msys64\ucrt64\bin;" + $env:PATH
}

$root = $PSScriptRoot
New-Item -ItemType Directory -Force "$root\build" | Out-Null
if (-not (Test-Path "$root\build\glfw3.dll")) { Copy-Item "$root\lib\lib-vc2022\glfw3.dll" "$root\build\" }
g++ -std=c++17 -g -I"$root\src" -I"$root\lib\imgui" -I"$root\lib\include" `
    "$root\src\*.cpp" "$root\lib\imgui\*.cpp" -o "$root\build\main.exe" `
    -L"$root\lib\lib-vc2022" -l:glfw3.dll -lopengl32 -lgdi32 -limm32
if ($LASTEXITCODE -ne 0) { Write-Host "BUILD FAILED (is main.exe still open? close it first)"; exit 1 }
if (-not $NoRun) { Push-Location "$root\build"; & .\main.exe; Pop-Location }
