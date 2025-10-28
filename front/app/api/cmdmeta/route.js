// import { query } from "../db";

// export async function GET(req) {
//   const { searchParams } = new URL(req.url);
//   const command = searchParams.get("command");

//   if (!command) {
//     return Response.json({ error: "Command parameter is required" }, { status: 400 });
//   }

//   try {
//     const sql = "SELECT Parameter, ParameterType FROM meta_data WHERE command_tb_no = ?";
//     const results = await query(sql, [command]);

//     return Response.json(results);
//   } catch (error) {
//     console.error("Database query error:", error);
//     return Response.json({ error: "Internal Server Error" }, { status: 500 });
//   }
// }

// /api/cmdmeta/route.ts
import { query } from "../db";

export async function GET(req) {
  const { searchParams } = new URL(req.url);
  const byName = searchParams.get("name") || searchParams.get("cmd_name");
  const byId   = searchParams.get("command"); // 하위호환

  if (!byName && !byId) {
    return Response.json({ error: "name (or command) is required" }, { status: 400 });
  }

  try {
    let sql;
    let params= [];

    if (byName) {
      sql = `
        SELECT Parameter, ParameterType
        FROM meta_data
        WHERE cmd_name = ?
        ORDER BY No ASC
      `;
      params = [byName];
    } else {
      // 번호만 들어온 경우: 조인으로 이름 찾기(기존 지원)
      sql = `
        SELECT m.Parameter, m.ParameterType
        FROM meta_data m
        JOIN command c ON c.No = m.command_tb_no
        WHERE c.No = ?
        ORDER BY m.No ASC
      `;
      params = [byId];
    }

    const rows = await query(sql, params);
    return Response.json(rows ?? []);
  } catch (e) {
    console.error("Database query error:", e);
    return Response.json({ error: "Internal Server Error" }, { status: 500 });
  }
}
