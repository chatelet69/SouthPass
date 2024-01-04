import QtQml
import QtQuick
import QtQuick.Controls

Item {
    id: credsFormWindow
    visible: true
    width: 300
    height: 300
    x: 0
    y: 0

    Rectangle {
        height: 20
//        x: 20
        y: 0
        color: "white"
        border.color: "black"
        border.width: 3

        Row {
            spacing: 10

            Label {
                //anchors.centerIn: parent
                text: "Nouveaux identifiants"
            }

            Button {
                id: cancelNewCredsButton
                text: "Annuler"
                onClicked: CredsFormWidget.closeForm()
                background: Rectangle {
                    color: "#c95151"
                    border.width: 2
                    border.color: "#ffffff"
                    radius: 6
                }
            }
        }
    }

    Rectangle {
        width: 200
        x: 0
        y: 30
        border.color: "red"
        border.width: 4

        Column {
            spacing: 10

            TextField {
                id: nameInput
                placeholderText: "Nom de l'identifiant"
                width: 160
            }

            TextField {
                id: usernameInput
                placeholderText: "Login"
                width: 160
            }

            TextField {
                id: passwordInput
                placeholderText: "Mot de passe"
                width: 160
            }

            Button {
                text: "Confirmer"
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