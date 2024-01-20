const DatabaseConnection = require("./Database");

class UserRepository {
    db;

    constructor() {
        this.db = new DatabaseConnection();
    }
  
    getUserById(userId) {
        const sqlQuery = "SELECT id, email, pwfAccount, pwdMaster,salt,jwt FROM users WHERE id = ?";
        return new Promise((resolve, reject) => {
            this.db.query(sqlQuery, [userId], (error, result) => {
                if (error) throw error;
                resolve(result[0]);
            });
        });
    }

    getSaltByEmail(email) {
        const sqlQuery = "SELECT salt FROM users WHERE email = ?";
        return new Promise((resolve, reject) => {
            this.db.query(sqlQuery, [email], (error, result) => {
                if (error) throw error;
                resolve(result[0]);
            });
        });
    }

    checkLogin(email, password, masterPassword) {
        const sqlQuery = "SELECT id, email, email FROM users WHERE email = ? AND pwdAccount = ? AND pwdMaster = ?";
        return new Promise ((resolve, reject) => {
            this.db.query(sqlQuery, [email, password, masterPassword], (error, result) => {
                if (error) throw (error);
                resolve(result[0]);
            });
        });
    }

    setNewKey(userId, jwt) {
        const sqlQuery = "UPDATE users set jwt = ? WHERE id = ?";
        return new Promise ((resolve, reject) => {
            this.db.query(sqlQuery, [jwt, userId], (error, result) => {
                if (error) throw (error);
                resolve(result);
            });
        });
    }

    async getCredentialByUserId(userId, name) {
        let sqlQuery = "SELECT psw.id AS id, psw.name AS name, psw.loginName AS login, (AES_DECRYPT(psw.password, UNHEX(u.pwdMaster))) AS password" +
         " FROM pswd_stock psw INNER JOIN users u ON u.id = psw.userId WHERE psw.userId = ? AND (name LIKE ? OR ? LIKE (CONCAT('%', CONCAT(name, '%'))))";
        return new Promise((resolve, reject) => {
            this.db.query(sqlQuery, [userId, `%${name}%`, name], (error, result) => {
                if (error) throw (error);
                resolve(result);
            });
        });
    }
}

module.exports = UserRepository;