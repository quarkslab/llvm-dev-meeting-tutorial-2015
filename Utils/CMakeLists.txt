# Altough this lib somehow depends on LLVM*, there's no need to add the
# dependencies here, they will be resolved at load time
# This could be made a shared lib
add_library(Utils STATIC Utils.cpp)
