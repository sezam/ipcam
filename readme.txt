используется vcpkg
при сборке vcpkg возможно сообщение
/include/vcpkg/packagespec.h:22:9: note: vcpkg::PackageSpec::PackageSpec() noexcept is implicitly deleted because its exception-specification does not match the implicit exception-specification
PackageSpec() noexcept = default;
в файле /include/vcpkg/packagespec.h удалить noexept у классов Package и PackageSpec

vcpkg install boost
vcpkg install boost-interprocess
vcpkg install opencv4

сборка под win7
cd build
cmake .. -DCMAKE_SYSTEM_VERSION=8.1 -DCMAKE_TOOLCHAIN_FILE=D:\lib\vcpkg\scripts\buildsystems\vcpkg.cmake
cmake --build . --config Release -j 4

сборка под linux
scl enable devtoolset-8 bash
cd build
cmake .. -DCMAKE_TOOLCHAIN_FILE=/opt/vcpkg/scripts/buildsystems/vcpkg.cmake
cmake --build . --config Release -j 4

