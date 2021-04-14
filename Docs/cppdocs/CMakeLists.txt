#search for Doxygen package
find_package(Doxygen)
if (DOXYGEN_FOUND)
    message("Found Doxygen")
    set(DOXYGEN_RECURSIVE YES)
    set(DOXYGEN_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})
    set(DOXYGEN_SHOW_NAMESPACES YES)
    set(DOXYGEN_GENERATE_TREEVIEW YES)
    doxygen_add_docs(CATAPDocs
                     ${PROJECT_SOURCE_DIR} ALL)
    set_property(TARGET CATAPDocs PROPERTY FOLDER "Docs")
endif(DOXYGEN_FOUND)

