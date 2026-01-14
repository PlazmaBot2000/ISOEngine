t=$(date +"%Y%m%d_%H%M")

SCRIPT_ROOT=$(dirname "$(readlink -f "$0")")
dest="${SCRIPT_ROOT}/Builds/build_$t"

mkdir -p "$dest/Assets"

if [ -d "${SCRIPT_ROOT}/Assets" ]; then
    cp -r "${SCRIPT_ROOT}/Assets/." "$dest/Assets/"
fi

g++ *.cpp Engine/*.cpp -o "$dest/main" $(pkg-config --cflags --libs sdl2 SDL2_image)

if [ $? -eq 0 ]; then
    echo "Build finished: $dest"
else
    echo "Build failed!"
    exit 1
fi
