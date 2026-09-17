cmake_minimum_required(VERSION 3.20)
project(gemsight LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(GEMSIGHT_VERSION "0.1.0-dev" CACHE STRING "Application version")

find_package(Qt6 6.11 REQUIRED COMPONENTS
    Core
    Gui
    Network
    Qml
    Quick
    HttpServer
    Sql
    Concurrent
    LinguistTools
)

qt_standard_project_setup(REQUIRES 6.11)

include(FetchContent)
FetchContent_Declare(
    qml_material
    GIT_REPOSITORY https://github.com/hypengw/QmlMaterial.git
    GIT_TAG 7bb1dcd008a941909df958e92be5212764a857f6
    GIT_SHALLOW FALSE
    EXCLUDE_FROM_ALL
)
FetchContent_MakeAvailable(qml_material)

FetchContent_GetProperties(qml_material)
if(qml_material_POPULATED)
    execute_process(
        COMMAND bash "${CMAKE_CURRENT_SOURCE_DIR}/scripts/setup-material-fonts.sh"
                "${qml_material_SOURCE_DIR}"
    )
endif()

set(CMAKE_AUTOMOC ON)

set(GEMSIGHT_SOURCES
    src/app/main.cpp
    src/core/facade/core_controller.cpp
    src/core/settings/settings_store.cpp
    src/core/i18n/translation_service.cpp
    src/core/gsi/gsi_server.cpp
    src/core/session/match_session_controller.cpp
    src/core/draft/game_mode_policy.cpp
    src/core/draft/intel_gate.cpp
    src/core/draft/draft_controller.cpp
    src/models/enemy_team_model.cpp
    src/models/ally_team_model.cpp
)

qt_add_executable(gemsight_app ${GEMSIGHT_SOURCES})

file(GLOB_RECURSE GEMSIGHT_QML_FILES CONFIGURE_DEPENDS
     RELATIVE "${CMAKE_CURRENT_SOURCE_DIR}"
     "${CMAKE_CURRENT_SOURCE_DIR}/qml/*.qml")

set_source_files_properties(
    qml/theme/Appearance.qml
    qml/i18n/Messages.qml
    PROPERTIES QT_QML_SINGLETON_TYPE TRUE)

qt_add_qml_module(gemsight_app
    URI gemsight
    QML_FILES ${GEMSIGHT_QML_FILES}
    DEPENDENCIES
        Qcm.Material
        QtQuick.Layouts
)

include(${CMAKE_CURRENT_SOURCE_DIR}/cmake/GemSightTranslations.cmake)
gemsight_setup_translations(gemsight_app)

add_dependencies(gemsight_app qml_materialplugin)

target_compile_definitions(gemsight_app PRIVATE
    "QT_QML_MATERIAL_IMPORT_PATH=\"qml_modules\""
    "GEMSIGHT_VERSION=\"${GEMSIGHT_VERSION}\""
)

target_include_directories(gemsight_app PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/src
)

target_link_libraries(gemsight_app
    PRIVATE
        Qt6::Core
        Qt6::Gui
        Qt6::Network
        Qt6::Concurrent
        Qt6::Qml
        Qt6::Quick
        Qt6::HttpServer
        Qt6::Sql
        qml_material::qml_material
        qml_materialplugin
)

qt_import_qml_plugins(gemsight_app)

install(TARGETS gemsight_app RUNTIME DESTINATION bin)
