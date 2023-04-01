# BasaltTestProject
This command line utility allows us to get information about 2 Alt Linux branches, compare them and write information into json file. At first execute in your terminal 2 commands: sudo apt-get install nlohmann-json-devel and sudo apt-get install libssl-devel. Then you should execute the following commands: 
1) git clone https://github.com/kotelifelif/BasaltTestProject.git 
2) cd BasaltTestProject
3) git submodule init
4) git submodule update
5) cd BasaltTestProject
6) mkdir build
7) cd build
8) cmake -DCMAKE_INSTALL_LIBDIR=bin ..
9) cmake --build .
10) cmake --install .
To use this utility input BasaltTestProject {first_branch_name} {second_branch_name} {json_file_name}
