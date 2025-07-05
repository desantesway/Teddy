call cd ..\
call git submodule foreach git fetch
call git submodule update
call git fetch --recurse-submodules
call git push --recurse-submodules=on-demand
call git submodule update --init --recursive
call git submodule update --recursive --remote

PAUSE