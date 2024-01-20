const jwt       = require("jsonwebtoken");
const config    = require("../../config.json");
const secret    = config.secretJwt;

const authMiddleware = (req, res, next) => {
    console.log(req.headers);
    const authToken = req.headers.authorization;
    if (authToken) {
        try {
            const infos = jwt.verify(authToken, secret);
            req.user = infos;
            next();
        } catch (error) {
            res.status(500).json({error: "expired_token"});
        }
    } else {
        return res.status(403).json({error: "Unauthorized"});
    }
};

module.exports = authMiddleware;