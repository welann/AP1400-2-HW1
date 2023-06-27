add_rules("mode.debug", "mode.release")

add_includedirs("include/")

set_languages("c++14")

add_requires("gtest")

target("hw1")
    set_kind("binary")
    add_files("src/*.cpp")
    add_packages("gtest")
    set_rundir("$(projectdir)")