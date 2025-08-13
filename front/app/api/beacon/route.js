import { query } from "../db";

export async function GET() {
  try {
    // 최신 인덱스 한 개 가져오기
    const latestBeacon = await query("SELECT * FROM beacon ORDER BY No DESC LIMIT 1");

    if (latestBeacon.length === 0) {
      return Response.json({ error: "No data found" }, { status: 404 });
    }

    return Response.json(latestBeacon[0]); // 최신 한 개 데이터 반환
  } catch (error) {
    console.error("Database error:", error);
    return Response.json({ error: "Database query failed" }, { status: 500 });
  }
}
