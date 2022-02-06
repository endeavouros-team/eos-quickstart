import QtQuick 2.15
import QtQuick.Controls 2.15

ApplicationWindow {
    width: 600
    height: 800
    visible: true
    title: qsTr("EndeavourOS QuickStart Installer")

    SystemPalette { id: systemPalette; colorGroup: SystemPalette.Active }

    ListView {
        id: lvPackages

        property var expandMap: ({})

        model: packageModel
        delegate: Item {
            property bool expanded: lvPackages.isSectionExpanded( model.category )
            width: parent.width - sbListview.width
            implicitHeight: expanded ? check.implicitHeight : 0

            Row {

                height: parent.height
                width: parent.width
                clip: true

                CheckDelegate {
                    id: check
                    width: parent.width
                    text: "     " + model.name + " - " + model.description + (model.isInstalled ? " (" + qsTr("Installed") + ")" : "")
                    checkable: model.isInstalled ? false : true
                    checked: model.isChecked

                    font.bold: model.isInstalled ? true : false

                    onCheckStateChanged: model.isChecked = checked
                }
            }
            Behavior on height {
                NumberAnimation { duration: 200 }
            }
        }

        width: Window.width
        height: Window.height - installButton.height - 10
        focus: true
        clip: true

        section.property: "category"
        section.criteria: ViewSection.FullString

        section.delegate: Rectangle {
            width: parent.width - sbListview.width
            implicitHeight: text.implicitHeight + 2
            color: systemPalette.window
            Text {
                id: text
                anchors.left: parent.left
                font.bold: true
                text: section
                padding: 5
                color: systemPalette.windowText
            }

            MouseArea {
                anchors.fill: parent
                onClicked: lvPackages.toggleSection( section );

            }

        }

        function isSectionExpanded( section ) {
            return (section in expandMap);
        }

        function hideSection( section ) {
            delete expandMap[section]
            expandMapChanged();
        }

        function showSection( section ) {
            expandMap[section] = true
            expandMapChanged();
        }

        function toggleSection( section ) {
            if ( isSectionExpanded( section ) ) {
                hideSection( section )
            } else {
                showSection( section )
            }
        }

        // We need to make sure our selections don't become unchecked as we scroll
        cacheBuffer: 50000

        ScrollBar.vertical: ScrollBar {
            id: sbListview
            active: true
        }
    }

    Button {
        id: installButton
        height: 40
        anchors {
            bottom: parent.bottom
            bottomMargin: 5
            horizontalCenter: parent.horizontalCenter;
        }

        onClicked: packageManager.installPackages()

        text: qsTr("Install Now")
        palette {
            button: systemPalette.button
            buttonText: systemPalette.buttonText
        }
    }

}
