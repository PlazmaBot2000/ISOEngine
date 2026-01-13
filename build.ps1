$t = Get-Date -Format "yyyyMMdd_HHmm"
$dest = "${PSScriptRoot}/Builds/build_$t"

New-Item -ItemType Directory -Force -Path $dest
New-Item -ItemType Directory -Force -Path "$dest/Assets"

if (Test-Path "${PSScriptRoot}/Assets") {
    Copy-Item -Path "${PSScriptRoot}/Assets/*" -Destination "$dest/Assets" -Recurse -Force
}

g++ *.cpp Engine/*.cpp -o "$dest/main.exe" `
    -I"C:/msys64/ucrt64/include/SDL2" `
    -L"C:/msys64/ucrt64/lib" `
    -lmingw32 -lSDL2main -lSDL2 -lSDL2_image

Write-Host "Build finished: $dest"
