const jwt       = require("jsonwebtoken");
const config    = require("../../config.json");
const secret    = config.secretJwt;
const servKey   = config.servKey;

const serverAuthKeyMiddleware = (req, res, next) => {
    const authToken = req.headers.authorization;
    if (authToken) {
        try {
            req.specialApiKey = authToken;
            if (authToken !== servKey) return res.status(403).json({error: "Vous n'êtes pas autorisé à appeler l'API."});
            next();
        } catch (error) {
            res.status(500).json({error: "bad_token"});
        }
    } else {
        return res.status(403).json({error: "Unauthorized"});
    }
};

module.exports = serverAuthKeyMiddleware;