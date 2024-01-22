const UserRepository        = require("../repository/UserRepository");
const sha256                = require('js-sha256');
const jwt                   = require("jsonwebtoken");
const secret                = require("../../config.json").secretJwt;
const baseUrl               = require("../../config.json").baseUrl;
const { exec }              = require('node:child_process')

class UserService {
    userRepository;

    constructor() {
        this.userRepository = new UserRepository();
    }
  
    async authLoginService(email, password, masterPwd) {
        try {
            let salt = await this.userRepository.getSaltByEmail(email);
            if (salt != undefined) {
                salt = salt.salt;
                let authToken = false;
                password = salt + password + salt;
                masterPwd = salt + masterPwd + salt;
                const hash = sha256(password);
                const hashMaster = sha256(masterPwd);
                const checkLogin = await this.userRepository.checkLogin(email, hash, hashMaster);
                if (checkLogin) authToken = this.generateKey(checkLogin.id, email);
                return (authToken) ? authToken : false;
            } else {
                return false;
            }
        } catch (error) {
            console.log(error);
            return false;
        }
    }

    async myUserService(userId) {
        try {
            let userData = false;
            if (userId != 0) userData = await this.userRepository.getUserById(userId);
            return userData;
        } catch (error) {
            console.log(error);
            return false;
        }
    }

    async getUserService(userId) {
        try {
            let userData = false;
            userData = await this.userRepository.getUserById(userId);
            if (userData) return userData;
            return {error: form.userNotFound};
        } catch (error) {
            console.log(error);
            return false;
        }
    }

    async generateKey(userId, email) {
        let token = jwt.sign({email: email, userId: userId}, secret, { expiresIn: "72h" });
        let check = await this.userRepository.setNewKey(userId, token);
        if (check.affectedRows == 0) return false;
        return token;
    }

    async getCredential(userId, website) {
        try {
            let resData = await this.userRepository.getCredentialByUserId(userId, website);
            if (resData.length > 0) {
                resData = resData[0];
                console.log("res:", resData);
                return {id: resData.id, name: resData.name, login: resData.login, password: resData.password.toString()};
            } else {
                return {error: "not found"};
            }
        } catch (error) {
            console.log(error);
            return {error: "not found"};
        }
    }

    async sendVerifCode(email) {
        try {
            let checkUserEmail = await this.userRepository.getUserByEmail(email);
            if (checkUserEmail && checkUserEmail.email) {
                let emailCode = Math.floor(Math.random() * (9864, 1234) + 1234);
                let resDb = await this.userRepository.insertEmailCode(checkUserEmail.email, emailCode, checkUserEmail.id);
                if (resDb.affectedRows > 0) {
                    this.sendVerifCodeMail(email, emailCode);
                    return {emailCode: emailCode};
                } else {
                    return {error: "error_email_code"};
                }
            } else {
                return {error: "user_exist"};
            }
        } catch (error) {
            return {error: "not found"};
        }
    }

    sendVerifCodeMail(email, code) {
        exec(`php /home/debian/southpassApi/sendmail.php ${email} ${code}`, (error, output) => {
            if (error) {
                console.log(error);
                return;
            }
            console.log("Output: \n", output);
        });
    }
}

module.exports = UserService;