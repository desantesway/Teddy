pushd %~dp0\..\
if not exist VS-build\ mkdir VS-build\
pushd .\VS-build\
call cmake ..\
popd
popd
PAUSE