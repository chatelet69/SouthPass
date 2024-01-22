const express                   = require('express');
const router                    = express.Router();
const UserController            = require("../controller/UserController");
const authMiddleware            = require("../middlewares/authMiddleware");
const authorizationMiddleware   = require("../middlewares/authorizationMiddleware");
const serverAuthKeyMiddleware   = require("../middlewares/serverMiddleware");
const userController            = new UserController();

// Get method

router.get("/me", [authMiddleware, authorizationMiddleware], (req, res) => { userController.myUser(req, res); });

router.get("/getCredential", [authMiddleware, authorizationMiddleware], userController.getCredential);

router.get("/sendVerifCode", [serverAuthKeyMiddleware], userController.sendVerifCode);

// Post method

router.post("/login", userController.authLogin);

module.exports = router;