# TTest ver 1.0

Small, simple, easy to use and modify testing framework.
Not designed to be any sort of GTest or doctest/catch2 competitor,
just enougth to test home/personal projects.
 
Attempt to provide somewhat of a testing framework, without mental load of big code base,
but keeping most usefull features.
 
Currenly have some minor faults in terms of memory efficiency and 
dependant on Crt for memory buffer writes, which may or not, be solved in further(?) versions.
Some places may be improved with more of C++(i.e. necessity to pass format strings in checks),
although requires more time to look into.

Maybe needs TEST_CASE type, that await warning, or abort result,
however in current state looks more like a sugar.

#### More info in ttest.h file
