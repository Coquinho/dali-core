# Compile order
===============
1. microsoft/vcpkg
2. dalihub/windows-dependencies
3. dalihub/dali-core
4. dalihub/dali-adaptor
5. dalihub/dali-toolkit
6. dalihub/dali-demo
7. expertisesolutions/tizenfx-stub
8. dalihub/dali-csharp-binder
9. Samsung/TizenFX

-------------------------------------------------------------------------------

# VCKPKG
========
Run `vcvars64.bat`, then:
```bash
> powershell
> cd $env:USERPROFILE/expertise
> gh repo clone microsoft/vcpkg
> cd vcpkg
> bootstrap-vcpkg.bat
> vcpkg.exe integrate install
> vcpkg.exe install angle[core]:x64-windows bzip2[core]:x64-windows cairo[core]:x64-windows curl[core,ssl,winssl]:x64-windows dirent[core]:x64-windows egl-registry[core]:x64-windows expat[core]:x64-windows fontconfig[core]:x64-windows freetype[core]:x64-windows fribidi[core]:x64-windows getopt-win32[core]:x64-windows gettext[core]:x64-windows giflib[core]:x64-windows glib[core]:x64-windows harfbuzz[core]:x64-windows libexif[core]:x64-windows libffi[core]:x64-windows libiconv[core]:x64-windows libjpeg-turbo[core]:x64-windows libpng[core]:x64-windows libwebp[core]:x64-windows opengl[core]:x64-windows pcre[core]:x64-windows pixman[core]:x64-windows pthreads[core]:x64-windows ragel[core]:x64-windows tool-meson[core]:x64-windows winsock2[core]:x64-windows zlib[core]:x64-windows
```

-------------------------------------------------------------------------------

# dali-env
==========
This consider that every `dali`/`Tizen` will be cloned at
`$env:USERPROFILE/expertise/dalihub/`, and `dali-env` in
`$env:USERPROFILE/expertise/dalihub/dali-env.
`dali-env` need some ambiente variables, some may be aquired by
`windows-dependencies/{prebuile.bat,setenv.bat}`, but not all, I recommend
runing the `prebuild.bat` and doing all the rest by yourself.
```bash
> $env:DALI_ENV_FOLDER="$env:USERPROFILE/expertise/dalihub/dali-env"
> mkdir $env:DALI_ENV_FOLDER
> cd $env:DALI_ENV_FOLDER
> ../dalihub/windows-dependencies/prebuild.bat
> ../dalihub/windows-dependencies/setenv.bat
```

All ambient variable seted here are needed for all `dali` and `Tizen`
aplications, so either set then for each section with `cmd` `set`/`powershell`
`$env`/`Git bash` `export`, or use `cmd` `setx`/Windows interface to change the
variables for your user acount.
At the end, those are the ones needed:

## VCPKG related
================
Need to set, the folder where to find `vcpkg`, add to `$env:PATH` the `dll` of
the instaled packages, and set where to find the `font.conf`:
```bash
> $env:VCPKG_FOLDER="$env:USERPROFILE/expertise/"
> $env:PATH="$env:PATH;$env:VCPKG_FOLDER\vcpkg\installed\x64-windows\bin"
> $env:PATH="$env:PATH;$env:VCPKG_FOLDER\vcpkg\installed\x64-windows\bin\debug"
> $env:FONTCONFIG_PATH="$env:VCPKG_FOLDE\vcpkg\installed\x64-windows\tools\fontconfig\"
```

## Dali related
===============
Need to set, the addres of `dali-env` folder, add to `$env:PATH` the `dll` of
the `dali` libs, as well as where are to find dali aplications and window
related sizes.
```bash
> $env:DALI_ENV_FOLDER="$env:USERPROFILE/expertise/dalihub/dali-env"
> $env:PATH="$env:PATH;$env:DALI_ENV\bin\"
> $env:PATH="$env:PATH;$env:DALI_ENV\bin\debug"
> $env:DALI_APPLICATION_PACKAGE=$env:DALI_ENV\opt\share\com.samsung.dali-demo\res\%
> $env:DALI_WINDOW_WIDTH="480"
> $env:DALI_WINDOW_HEIGHT="800"
```
Becaues on windows we don't install at the default place (`/usr`), there is a
need to set some paths to `dali-toolkit`:
```bash
> $env:DALI_IMAGE_DIR="$env:DALI_ENV_FOLDER/share/dali/toolkit/images/"
> $env:DALI_SOUND_DIR="$env:DALI_ENV_FOLDER/share/dali/toolkit/sounds/"
> $env:DALI_STYLE_DIR="$env:DALI_ENV_FOLDER/share/dali/toolkit/styles/"
> $env:DALI_STYLE_IMAGE_DIR="$env:DALI_ENV_FOLDER/share/dali/toolkit/styles/images/"
> $env:DALI_DATA_READ_ONLY_DIR="$env:DALI_ENV_FOLDER/share/dali/"
```

-------------------------------------------------------------------------------

Instalation should happen at the same terminal dali-env was set.
## dalihub/windows-dependencies
```bash
> cd $env:USERPROFILE/expertise/dalihub/windows-dependencies/build
> cmake -g Ninja . -DCMAKE_TOOLCHAIN_FILE=$env:VCPKG_FOLDER/vcpkg/scripts/buildsystems/vcpkg.cmake -DENABLE_PKG_CONFIGURE=OFF -DENABLE_LINK_TEST=OFF -DCMAKE_INSTALL_PREFIX=$env:DALI_ENV_FOLDER -DINSTALL_CMAKE_MODULES=ON -DENABLE_DEBUG=ON
> cmake --build . --target install
```
Here using `dali_1.9.30`, so, `git checkout dali_1.9.30` on every dali libs.
## dalihub/dali-core
```bash
> cd $env:USERPROFILE/expertise/dalihub/dali-core/build/tizen
> cmake -g Ninja . -DCMAKE_TOOLCHAIN_FILE="$env:VCPKG_FOLDER/vcpkg/scripts/buildsystems/vcpkg.cmake" -DENABLE_PKG_CONFIGURE=OFF -DENABLE_LINK_TEST=OFF -DCMAKE_INSTALL_PREFIX="$env:DALI_ENV_FOLDER" -DINSTALL_CMAKE_MODULES=ON -DENABLE_DEBUG=ON
> cmake --build . --target install
```

## dalihub/dali-adaptor
```bash
> cd $env:USERPROFILE/expertise/dalihub/dali-core/build/tizen
> cmake -g Ninja . -DCMAKE_TOOLCHAIN_FILE="$env:VCPKG_FOLDER/vcpkg/scripts/buildsystems/vcpkg.cmake" -DENABLE_PKG_CONFIGURE=OFF -DENABLE_LINK_TEST=OFF -DCMAKE_INSTALL_PREFIX="$env:DALI_ENV_FOLDER" -DINSTALL_CMAKE_MODULES=ON -DPROFILE_LCASE=windows -DENABLE_DEBUG=ON
> cmake --build . --target install
```

## dalihub/dali-toolkit
```bash
> cd $env:USERPROFILE/expertise/dalihub/dali-toolkit/build/tizen
> cmake -g Ninja . -DCMAKE_TOOLCHAIN_FILE="$env:VCPKG_FOLDER/vcpkg/scripts/buildsystems/vcpkg.cmake" -DENABLE_PKG_CONFIGURE=OFF -DENABLE_LINK_TEST=OFF -DCMAKE_INSTALL_PREFIX="$env:DALI_ENV_FOLDER" -DINSTALL_CMAKE_MODULES=ON -DUSE_DEFAULT_RESOURCE_DIR=OFF -DENABLE_DEBUG=ON
> cmake --build . --target install
```

## dalihub/dali-demo
```bash
> cd $env:USERPROFILE/expertise/dalihub/dali-toolkit/build/tizen
> cmake -g Ninja . -DCMAKE_TOOLCHAIN_FILE="$env:VCPKG_FOLDER/vcpkg/scripts/buildsystems/vcpkg.cmake" -DENABLE_PKG_CONFIGURE=OFF -DINTERNATIONALIZATION=OFF -DCMAKE_INSTALL_PREFIX="$env:DALI_ENV_FOLDER" -DENABLE_DEBUG=ON
> cmake --build . --target install
```

##  expertisesolutions/tizenfx-stub
```bash
> cd $env:USERPROFILE/expertise/dalihub/tizenfx-stub
> git checkout walac/windows
> mkdir build
> cmake -DCMAKE_INSTALL_PREFIX="$env:DALI_ENV_FOLDER" -DCMAKE_TOOLCHAIN_FILE="$env:VCPKG_FOLDER/vcpkg/scripts/buildsystems/vcpkg.cmake" ..
> cmake --build . --target install
```

## dalihub/dali-csharp-binder
```bash
> cd $env:USERPROFILE/expertise/dalihub/dali-csharp-binder
> cmake -g Ninja . -DCMAKE_TOOLCHAIN_FILE="$env:VCPKG_FOLDER/vcpkg/scripts/buildsystems/vcpkg.cmake" -DCMAKE_INSTALL_PREFIX="$env:DALI_ENV_FOLDER"
> cmake --build . --target install
```

## Samsung/TizenFX
Pick `expertisesolutions/TizenFX/pull/1`.
Open `Git bash` shell (with all the `dali-env` set):
```
$ cd $USERPROFILE/expertise/dalihub/TizenFX
$ ./build.sh full -p:DefineConstants=NOTIZEN
```
