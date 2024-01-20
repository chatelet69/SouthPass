const authorizationMiddleware = (req, res, next) => {
    const reqUser = req.user;
    if (!reqUser || reqUser.userId == 0 || reqUser.userId == undefined) return res.status(403).json({error: "Vous n'êtes pas autorisé à appeler l'API."});
    next();
};

module.exports = authorizationMiddleware;