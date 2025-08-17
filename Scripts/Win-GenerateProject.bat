pushd %~dp0\..\
git submodule update --init --recursive
if not exist VS-build\ mkdir VS-build\
pushd .\VS-build\
call cmake ..\
popd
popd
PAUSE