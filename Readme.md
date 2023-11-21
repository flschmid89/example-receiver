# Requirements

1. LIBZMQ & CPPZMQ 
   Installation instructions can be found here: https://github.com/zeromq/cppzmq#build-instructions
2. OpenCV
3. CMake
4. (Optional) Ninja


## Build

1. Initialize git submodules by ``` git submodule update --init --recursive``` 
2. Cmake Configure 
    1. Create subfolder ```build```  and change to folder
    2. command ``` cmake  ..``` 
    3. Build ``` cmake --build .``` 
