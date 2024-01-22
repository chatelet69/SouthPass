const UserService   = require("../services/UserService");
const baseUrl       = require("../../config.json").baseUrl;
const userService   = new UserService();

class UserController {
    async authLogin(req, res) {
        try {
            const body = req.body;
            console.log("body : ", body);
            const resLogin = await userService.authLoginService(body.email, body.password, body.masterPassword);
            if (resLogin) res.status(200).json({message: "success", jwt: resLogin});
            else res.status(403).json({error: "Impossible de se connecter"});
        } catch (error) {
            console.log(error);
            res.status(500).json({error: "Une erreur est survenue durant la connexion"});
        }
    }

    async myUser(req, res) {
        try {
            const userId = req.user.userId;
            const userData = await userService.myUserService(userId);
            res.status(200).json({message: "logged", user: userData});
        } catch (error) {
            console.log(error);
            res.status(500).json({error: "Une erreur est survenue durant la récupération de l'utilisateur"});
        }
    }

    async getUserById(req, res) {
        try {
            const userId = req.params.id;
            const userData = await userService.getUserService(userId);
            if (!userData.error) res.status(200).json(userData);    // Si l'utilisateur existe
            else res.status(404).json({error: userData.error});
        } catch (error) {
            console.log(error);
            res.status(500).json({error: "Une erreur est survenue durant la récupération de l'utilisateur"});
        }
    }

    async getCredential(req, res) {
        try {
            const data = {
                userId: req.user.userId,                
                website: req.query.website
            }
            let result = await userService.getCredential(data.userId, data.website);
            if (result.login) res.status(200).json(result);
            else res.status(500).json({error: result.error});
        } catch (error) {
            console.log(error);
            res.status(500).json({error: "error during getting credential"});
        }
    }

    async sendVerifCode(req, res) {
        try {
            const email = req.query.email;
            let result = await userService.sendVerifCode(email);
            if (result.emailCode) res.status(200).json(result);
            else res.status(500).json({error: result.error});
        } catch (error) {
            res.status(500).json({error: "error during getting credential"});
        }
    }
}

module.exports = UserController;