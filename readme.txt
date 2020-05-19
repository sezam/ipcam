#используется vcpkg
#при сборке vcpkg возможно сообщение
#/include/vcpkg/packagespec.h:22:9: note: vcpkg::PackageSpec::PackageSpec() noexcept is implicitly deleted because its exception-specification does not match the implicit exception-specification
#PackageSpec() noexcept = default;
#в файле /include/vcpkg/packagespec.h удалить noexept у классов Package и PackageSpec

vcpkg install boost
vcpkg install boost-interprocess
vcpkg install opencv4

#сборка под win7
cd build
cmake .. -DCMAKE_SYSTEM_VERSION=8.1 -DCMAKE_TOOLCHAIN_FILE=D:\lib\vcpkg\scripts\buildsystems\vcpkg.cmake
cmake --build . --config Release -j 4

#сборка под linux
scl enable devtoolset-8 bash
cd build
cmake .. -DCMAKE_TOOLCHAIN_FILE=/opt/vcpkg/scripts/buildsystems/vcpkg.cmake
cmake --build . --config Release -j 4

#запуск под linux
#установка ffmpeg в centos7
yum install epel-release -y
yum update -y

#There are no official FFmpeg rpm packages for CentOS for now. Instead, you can use a 3rd-party YUM repo, Nux Dextop, to finish the job.
sudo rpm --import http://li.nux.ro/download/nux/RPM-GPG-KEY-nux.ro
sudo rpm -Uvh http://li.nux.ro/download/nux/dextop/el7/x86_64/nux-dextop-release-0-5.el7.nux.noarch.rpm
sudo yum install ffmpeg ffmpeg-devel -y

#centos8
sudo yum install https://dl.fedoraproject.org/pub/epel/epel-release-latest-8.noarch.rpm
sudo yum install https://download1.rpmfusion.org/free/el/rpmfusion-free-release-8.noarch.rpm https://download1.rpmfusion.org/nonfree/el/rpmfusion-nonfree-release-8.noarch.rpm
sudo yum install http://rpmfind.net/linux/epel/7/x86_64/Packages/s/SDL2-2.0.10-1.el7.x86_64.rpm
sudo yum install ffmpeg ffmpeg-devel

#test ffmpeg installation
ffmpeg -version

#test
echo GetStatus | nc localhost 9091
echo GetPhoto | nc localhost 9091