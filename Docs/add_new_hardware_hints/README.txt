Some hints and tips for adding a new hardware type
(not complete)

create the necessary  .cpp and .h files for the hardware type

Place them "in the correct folders" 
These should be obvious from the class heirarchy 

Also create and add in the test files,

Examples for the camera are given here


Update all the necessary cmakelists.txt files with the new hardware filenames 

Create folder in master lattice, with name of harwdare
privde template file "ABC.yaml" (must be .yaml)
YOU MUST HAVE: 
    hardware_type : 
    machine_area : 
    name : 

cmake, configure, generate till no errors! 

reload visual studio project 

update GlobaltypeEnums.h with new TYPE name
(maybe have to append _TYPE to name if it conflicts)

update ConfigReader::allowedHardwareTypes to inlcude new harwdare 

update GlobalConstants::stringToTypeMap to inlcud the new hardware TYPE 
may also need versions in different upper/lower case














