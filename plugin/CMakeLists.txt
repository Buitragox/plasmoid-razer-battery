# ecm_add_library(razerbatteryplugin STATIC)

# ecm_add_qml_module(razerbatteryplugin URI
#     com.github.buitragox.private.razerbattery
# )

add_library(razerbatteryplugin SHARED
    razerbattery.cpp
    razerplugin.cpp
)
# target_sources(razerbatteryplugin PRIVATE razerbattery.cpp razerplugin.cpp)

target_link_libraries(razerbatteryplugin
    Qt6::Core
    Qt6::Qml
    Qt6::DBus
)

install(TARGETS razerbatteryplugin
    DESTINATION ${KDE_INSTALL_QMLDIR}/com/github/buitragox/private/razerbattery
)

install(FILES qmldir plugins.qmltypes
    DESTINATION ${KDE_INSTALL_QMLDIR}/com/github/buitragox/private/razerbattery
)

# ecm_finalize_qml_module(razerbatteryplugin DESTINATION ${KDE_INSTALL_QMLDIR})
