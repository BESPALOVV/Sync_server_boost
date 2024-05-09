# CMakeList.txt: файл проекта CMake верхнего уровня; проведите глобальную конфигурацию
# и включите сюда подпроекты.
#
cmake_minimum_required (VERSION 3.8)

set(CMAKE_PREFIX_PATH "C:\\Program Files\\vcpkg\\vcpkg\\packages")

project ("server")

# Включите подпроекты.
add_subdirectory ("server")
