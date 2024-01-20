const express           = require('express');
const config            = require("./config.json");
const app               = express();
const fs                = require("fs");
const path              = require('path');
const port              = config.apiPort;
const routesPath        = path.join(__dirname, './src/routes');
const bodyParser        = require('body-parser');
const cors              = require("cors");

app.use(bodyParser.json());
app.use(bodyParser.urlencoded({ extended: true }));
app.use(bodyParser.raw());

app.use(cors());

// Parcourt le répertoire routes afin de charger chaque fichier de route
fs.readdirSync(routesPath).forEach(file => {
    if (file.endsWith('.js')) {
        const route = require(path.join(routesPath, file));
        app.use(route);
    }
});

app.listen(port, () => { console.log("Serveur démarré sur le port :", port); });

// Pour toutes les requêtes sur des routes non définies
app.use((req, res, next) => {
    res.status(404).json({error: "ressource not found", cause: "bad method or inexistant route"});
});