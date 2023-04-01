# BasaltTestProject
This command line utility allows us to get information about 2 Alt Linux branches, compare them and write information into json file. At first execute in your terminal 2 commands: sudo apt-get install nlohmann-json-devel and sudo apt-get install libssl-devel. Then you should execute the following git commands: 
1) git clone https://github.com/kotelifelif/BasaltTestProject.git 
2) git submodule init
3) git submodule update
After that you should install library using this commands:
1) mkdir build
2) cd build
3) cmake -DCMAKE_INSTALL_LIBDIR=bin ..
4) cmake --build .
5) cmake --install .
To use this utility input BasaltTestProject {first_branch_name} {second_branch_name} {json_file_name}
