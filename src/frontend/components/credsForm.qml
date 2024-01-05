import QtQml
import QtQuick
import QtQuick.Controls

Rectangle {
    id: credsFormWindow
    visible: true
    width: 320
    height: 320
    x: 0
    y: 0
    color: "#4c637a"

    Rectangle {
        id: titleContainer
        x: 0
        y: parent.y + 15
        anchors.horizontalCenter: parent.horizontalCenter

        Column {
            id: titleRow
            spacing: 2
            anchors.horizontalCenter: parent.horizontalCenter

            Button {
                hoverEnabled: true
                anchors.horizontalCenter: parent.horizontalCenter
                id: cancelNewCredsButton
                text: "Annuler"
                contentItem: Text {
                    text: cancelNewCredsButton.text
                    color: "white"
                    padding: 5
                }
                background: Rectangle {
                    color: cancelNewCredsButton.hovered ? "#d11717" : "#e37b7b"
                    radius: 6
                }
                onClicked: CredsFormWidget.closeForm()
            }

            Text {
                textFormat: Text.RichText
                text: "<h4>Nouveaux identifiants</h4>"
                color: "white"
            }
        }
    }

    Rectangle {
        visible: true
        x: 0
        y: titleContainer.y + 80
        color: "lightgrey"
        radius: 5
        implicitWidth: inputsFormContainer.implicitWidth
        implicitHeight: inputsFormContainer.implicitHeight
        anchors.horizontalCenter: parent.horizontalCenter

        Column {
            id: inputsFormContainer
            spacing: 10
            anchors.horizontalCenter: parent.horizontalCenter

            TextField {
                id: nameInput
                placeholderText: "Nom de l'identifiant"
                width: 200
            }

            TextField {
                id: usernameInput
                placeholderText: "Login"
                width: 200
            }

            TextField {
                id: passwordInput
                placeholderText: "Mot de passe"
                width: 200
            }

            Button {
                id: confirmNewCredsButton
                text: "Confirmer"
                contentItem: Text {
                    text: confirmNewCredsButton.text
                    color: "white"
                    padding: 5
                }
                anchors.horizontalCenter: parent.horizontalCenter
                background: Rectangle {
                    color: "#64c469"
                    radius: 4
                }
                onClicked: {
                    var name = nameInput.text;
                    var username = usernameInput.text;
                    var password = passwordInput.text;
                    CredsFormWidget.saveNewCreds(name, username, password);
                }
            }
        }
    }
}