import mysql from "mysql2/promise";

const pool = mysql.createPool({
  host: "localhost",
  user: "miman",
  password: "qwe123",
  database: "HYVRID",
  waitForConnections: true,
  connectionLimit: 10, // 최대 연결 수 제한
  queueLimit: 0,
});

export async function query(sql, values = []) {
  const connection = await pool.getConnection();
  try {
    const [results] = await connection.execute(sql, values);
    return results;
  } finally {
    connection.release(); // 사용 후 연결 반환
  }
}
