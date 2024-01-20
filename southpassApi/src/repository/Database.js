const mysql     = require('mysql2');
const config    = require("../../config.json");

class Database {
    connectionPool;

    constructor() {
        this.connectionPool = mysql.createPool({
            host: config.host,
            user: config.user,
            password: config.password,
            database: config.database,
            waitForConnections: true,
            connectionLimit: 10,
            queueLimit: 0,
            port: 3306
        });
        return this.connectionPool;
    }

    close() {
        this.connectionPool.end;
    }
}

module.exports = Database;