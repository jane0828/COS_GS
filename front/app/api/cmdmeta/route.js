import { query } from "../db";

export async function GET(req) {
  const { searchParams } = new URL(req.url);
  const command = searchParams.get("command");

  if (!command) {
    return Response.json({ error: "Command parameter is required" }, { status: 400 });
  }

  try {
    const sql = "SELECT Parameter, ParameterType FROM meta_data WHERE command_tb_no = ?";
    const results = await query(sql, [command]);

    return Response.json(results);
  } catch (error) {
    console.error("Database query error:", error);
    return Response.json({ error: "Internal Server Error" }, { status: 500 });
  }
}
