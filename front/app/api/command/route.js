import { query } from "../db";

export async function GET() {
  try {
    const results = await query("SELECT * FROM command");
    return Response.json(results);
  } catch (error) {
    console.error("Database query error:", error);
    return Response.json({ error: "Internal Server Error" }, { status: 500 });
  }
}
