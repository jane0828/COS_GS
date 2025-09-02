// import { query } from "../db";

// export async function GET() {
//   try {
//     // 최신 인덱스 한 개 가져오기
//     const latestBeacon = await query("SELECT * FROM beacon ORDER BY No DESC LIMIT 1");

//     if (latestBeacon.length === 0) {
//       return Response.json({ error: "No data found" }, { status: 404 });
//     }

//     return Response.json(latestBeacon[0]); // 최신 한 개 데이터 반환
//   } catch (error) {
//     console.error("Database error:", error);
//     return Response.json({ error: "Database query failed" }, { status: 500 });
//   }
// }


// /app/api/beacon/route.js
import { query } from "../db.mjs";

export const dynamic = "force-dynamic"; // 캐시 끔
export const revalidate = 0;
export const runtime = "nodejs";

export async function GET() {
  try {
    const sql = `
      SELECT
        a.label  AS application,
        p.label  AS part,
        t.label  AS telemetry,
        COALESCE(tv.value_text, tv.value_num) AS value,
        tv.created_at,
        COALESCE(a.sort_order, 0) AS app_order,
        COALESCE(p.sort_order, 0) AS part_order,
        COALESCE(t.sort_order, 0) AS telm_order
      FROM telemetry_def t
      JOIN parts        p  ON p.part_id = t.part_id
      JOIN applications a  ON a.app_id = p.app_id
      JOIN (
        SELECT telemetry_id, MAX(value_id) AS value_id
        FROM telemetry_value
        GROUP BY telemetry_id
      ) last ON last.telemetry_id = t.telemetry_id
      JOIN telemetry_value tv ON tv.value_id = last.value_id
      ORDER BY app_order, part_order, telm_order, telemetry
    `;

    const rows = await query(sql);
    return Response.json(rows);
  } catch (err) {
    console.error("DB error:", err);
    return Response.json({ error: "Database query failed" }, { status: 500 });
  }
}
