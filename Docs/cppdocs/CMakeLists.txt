#search for Doxygen package
find_package(Doxygen)
if (DOXYGEN_FOUND)
    message("Found Doxygen")
    set(DOXYGEN_RECURSIVE YES)
    set(DOXYGEN_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})
    set(DOXYGEN_SHOW_NAMESPACES YES)
    set(DOXYGEN_EXTRACT_ALL YES)
    set(HAS_DOT NO)
    set(DOXYGEN_QUIET YES)
    set(DOXYGEN_GENERATE_TREEVIEW NO)
    doxygen_add_docs(CATAPDocs ALL
                     ${PROJECT_SOURCE_DIR})
    set_property(TARGET CATAPDocs PROPERTY FOLDER "Docs")
endif(DOXYGEN_FOUND)

