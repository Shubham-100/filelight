#######################################################################
# Copyright 2008-2014  Martin Sandsmark <martin.sandsmark@kde.org>
# Copyright 2017  Harald Sitter <sitter@kde.org>
#
# This program is free software; you can redistribute it and/or
# modify it under the terms of the GNU General Public License as
# published by the Free Software Foundation; either version 2 of
# the License or (at your option) version 3 or any later version
# accepted by the membership of KDE e.V. (or its successor approved
# by the membership of KDE e.V.), which shall act as a proxy
# defined in Section 14 of version 3 of the license.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <http://www.gnu.org/licenses/>.
#######################################################################

ecm_setup_version(${PROJECT_VERSION}
    VARIABLE_PREFIX FILELIGHT
    VERSION_HEADER version.h)

set(filelight_SRCS
    radialMap/widget.cpp
    radialMap/map.cpp
    radialMap/widgetEvents.cpp
    radialMap/labels.cpp
    scan.cpp
    progressBox.cpp
    Config.cpp
    settingsDialog.cpp
    fileTree.cpp
    localLister.cpp
    remoteLister.cpp
    summaryWidget.cpp
    historyAction.cpp
    mainWindow.cpp
    main.cpp
)
ecm_qt_declare_logging_category(filelight_SRCS HEADER filelight_debug.h IDENTIFIER FILELIGHT_LOG CATEGORY_NAME org.kde.filelight)

set(filelight_ICONS
    ${CMAKE_CURRENT_SOURCE_DIR}/../misc/16-apps-filelight.png
    ${CMAKE_CURRENT_SOURCE_DIR}/../misc/32-apps-filelight.png
    ${CMAKE_CURRENT_SOURCE_DIR}/../misc/48-apps-filelight.png
    ${CMAKE_CURRENT_SOURCE_DIR}/../misc/64-apps-filelight.png
    ${CMAKE_CURRENT_SOURCE_DIR}/../misc/128-apps-filelight.png
)
ecm_add_app_icon(filelight_SRCS ICONS
    ${filelight_ICONS})
ki18n_wrap_ui(filelight_SRCS dialog.ui)
add_executable(filelight ${filelight_SRCS})

target_link_libraries(filelight
    KF5::I18n
    KF5::XmlGui
    KF5::KIOWidgets # Only used for KDirLister, may be able to move away from that.
)
if (WIN32)
    find_package(KDEWin REQUIRED)
    target_link_libraries(filelight kdewin)
endif()

install(TARGETS filelight ${INSTALL_TARGETS_DEFAULT_ARGS})
