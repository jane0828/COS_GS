// /app/api/ingest/route.ts
import { NextRequest } from "next/server";
// db.mjs가 /app/api/db.mjs 에 있으니 한 단계 위로 올라갑니다.
import { query } from "../db.mjs";

/** MySQL DATETIME 형식으로 변환 */
function toMysqlDatetime(d: Date) {
  const pad = (n: number) => String(n).padStart(2, "0");
  const Y = d.getUTCFullYear();
  const M = pad(d.getUTCMonth() + 1);
  const D = pad(d.getUTCDate());
  const h = pad(d.getUTCHours());
  const m = pad(d.getUTCMinutes());
  const s = pad(d.getUTCSeconds());
  return `${Y}-${M}-${D} ${h}:${m}:${s}`;
}

async function getAppId(appLabel: string) {
  // code 또는 label 로 조회
  const rows: any[] = await query(
    "SELECT app_id FROM applications WHERE code=? OR label=? LIMIT 1",
    [appLabel, appLabel]
  );
  if (rows.length) return rows[0].app_id;

  const res: any = await query(
    "INSERT INTO applications (code,label,sort_order) VALUES (?,?,0)",
    [appLabel, appLabel]
  );
  // mysql2는 insertId를 반환합니다.
  return res.insertId;
}

async function getPartId(appId: number, partLabel: string) {
  const rows: any[] = await query(
    "SELECT part_id FROM parts WHERE app_id=? AND (code=? OR label=?) LIMIT 1",
    [appId, partLabel, partLabel]
  );
  if (rows.length) return rows[0].part_id;

  const res: any = await query(
    "INSERT INTO parts (app_id, code, label, sort_order) VALUES (?,?,?,0)",
    [appId, partLabel, partLabel]
  );
  return res.insertId;
}

async function getTelemetryId(partId: number, name: string, dtype: string) {
  const rows: any[] = await query(
    "SELECT telemetry_id FROM telemetry_def WHERE part_id=? AND name=? LIMIT 1",
    [partId, name]
  );
  if (rows.length) return rows[0].telemetry_id;

  const res: any = await query(
    "INSERT INTO telemetry_def (part_id, name, label, dtype, is_active, sort_order) VALUES (?,?,?,?,TRUE,0)",
    [partId, name, name, dtype]
  );
  return res.insertId;
}

export async function POST(req: NextRequest) {
  try {
    const body = await req.json();
    // body.timestamp 가 있으면 그걸 쓰고, 없으면 지금 시간
    const ts = body?.timestamp
      ? toMysqlDatetime(new Date(body.timestamp))
      : toMysqlDatetime(new Date());

    const items: Array<{
      application: string;
      part: string;
      values: Record<string, any>;
    }> = body?.items ?? [];

    if (!Array.isArray(items) || items.length === 0) {
      return Response.json({ error: "items 배열이 비어있습니다." }, { status: 400 });
    }

    let defsCreated = 0;
    let valuesInserted = 0;

    for (const item of items) {
      const appLabel = String(item.application).trim();
      const partLabel = String(item.part).trim();
      const values = item.values || {};

      // 1) 앱/파트 ID 확보 (없으면 생성)
      const appId = await getAppId(appLabel);
      const partId = await getPartId(appId, partLabel);

      // 2) values를 (key, value)로 순회
      for (const [name, rawVal] of Object.entries(values)) {
        // dtype 추정: 숫자면 int/float, 아니면 text
        let dtype = "text";
        let value_num: number | null = null;
        let value_text: string | null = null;

        if (typeof rawVal === "number") {
          dtype = Number.isInteger(rawVal) ? "int" : "float";
          value_num = rawVal;
        } else if (rawVal === null || rawVal === undefined) {
          dtype = "text";
          value_text = null;
        } else {
          dtype = "text";
          value_text = String(rawVal);
        }

        // 3) telemetry_def 확보 (없으면 생성)
        const before = await query(
          "SELECT telemetry_id FROM telemetry_def WHERE part_id=? AND name=? LIMIT 1",
          [partId, name]
        );
        let telemetryId: number;
        if ((before as any[]).length) {
          telemetryId = (before as any[])[0].telemetry_id;
        } else {
          const res: any = await query(
            "INSERT INTO telemetry_def (part_id, name, label, dtype, is_active, sort_order) VALUES (?,?,?,?,TRUE,0)",
            [partId, name, name, dtype]
          );
          telemetryId = res.insertId;
          defsCreated += 1;
        }

        // 4) 값 적재 (created_at = ts)
        await query(
          "INSERT INTO telemetry_value (telemetry_id, value_text, value_num, created_at) VALUES (?,?,?,?)",
          [telemetryId, value_text, value_num, ts]
        );
        valuesInserted += 1;
      }
    }

    return Response.json({
      ok: true,
      message: "ingest complete",
      defsCreated,
      valuesInserted,
      created_at: ts,
    });
  } catch (err: any) {
    console.error("INGEST ERROR:", err);
    return Response.json({ error: err?.message ?? "ingest failed" }, { status: 500 });

  }
}