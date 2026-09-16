function(gemsight_setup_translations target)
    find_package(Qt6 REQUIRED COMPONENTS LinguistTools)
    set(_gemsight_ts_files
        "${CMAKE_CURRENT_SOURCE_DIR}/translations/gemsight_en.ts"
        "${CMAKE_CURRENT_SOURCE_DIR}/translations/gemsight_ru.ts"
    )
    qt_add_translations(${target}
        TS_FILES ${_gemsight_ts_files}
        RESOURCE_PREFIX /i18n
        LUPDATE_OPTIONS -no-obsolete
    )
endfunction()
