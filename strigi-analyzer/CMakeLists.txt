project( strigi-analyzer )

set( STRIGI_MIN_VERSION 0.7.1 )
macro_optional_find_package( Strigi )
macro_log_feature(STRIGI_FOUND "Strigi" "A fast and small desktop searching program" "http://strigi.sourceforge.net" FALSE "${STRIGI_MIN_VERSION}" "")

if( STRIGI_FOUND )
    find_package( Json-c )
    macro_log_feature(JSON-C_FOUND "json-c" "A JSON implementation in C" "http://oss.metaparadigm.com/json-c" TRUE "0.9" "")

    include_directories( ${STRIGI_INCLUDE_DIR} ${JSON-C_INCLUDE_DIR})
    add_library( ea_knipptasch MODULE knipptaschanalyzer.cpp )
    target_link_libraries( ea_knipptasch ${STRIGI_STREAMANALYZER_LIBRARY} ${STRIGI_STREAMS_LIBRARY} ${JSON-C_LIBRARIES})
    set_target_properties( ea_knipptasch PROPERTIES PREFIX strigi )

    install( TARGETS ea_knipptasch LIBRARY DESTINATION ${LIB_INSTALL_DIR}/strigi )
endif( STRIGI_FOUND )
