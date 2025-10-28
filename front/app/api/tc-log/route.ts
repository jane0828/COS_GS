import { NextRequest, NextResponse } from 'next/server';
import { query } from '../db.mjs'; // ← 너의 db 헬퍼 경로로 맞춰줘
export const runtime = 'nodejs';

type Param = { name: string; type: string; value: any };
type LogBody = {
  command_name: string;
  msgid: number;
  cc: number;
  ip?: string | null;
  port?: number | null;
  type_string?: string | null;
  parameters: Param[];
  ws_client_id?: string | null;
  ws_url?: string | null;
  send_result: 'ok' | 'error';
  error_msg?: string | null;
};

export async function POST(req: NextRequest) {
  try {
    const body = (await req.json()) as LogBody;

    // 최소 검증
    if (
      !body?.command_name ||
      typeof body.msgid !== 'number' ||
      typeof body.cc !== 'number' ||
      !Array.isArray(body.parameters) ||
      (body.send_result !== 'ok' && body.send_result !== 'error')
    ) {
      return NextResponse.json({ ok: false, error: 'Bad request' }, { status: 400 });
    }

    const result: any = await query(
      `INSERT INTO tc_command_log
       (command_name, msgid, cc, ip, port, type_string, parameters, ws_client_id, ws_url, send_result, error_msg)
       VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?)`,
      [
        body.command_name,
        body.msgid,
        body.cc,
        body.ip ?? null,
        body.port ?? null,
        body.type_string ?? null,
        JSON.stringify(body.parameters),
        body.ws_client_id ?? null,
        body.ws_url ?? null,
        body.send_result,
        body.error_msg ?? null,
      ]
    );

    return NextResponse.json({ ok: true, id: result.insertId });
  } catch (err: any) {
    console.error('[tc-log] insert error:', err);
    return NextResponse.json({ ok: false, error: err?.message ?? 'Server error' }, { status: 500 });
  }
}

export async function GET(req: NextRequest) {
  try {
    const { searchParams } = new URL(req.url);
    const limit = Math.min(Math.max(Number(searchParams.get('limit') ?? '30'), 1), 200);
    const offset = Math.max(Number(searchParams.get('offset') ?? '0'), 0);
    const command = searchParams.get('command') || ''; // 선택: 특정 명령 필터
    const result = await query(
      `
      SELECT
        id,
        created_at,
        command_name,
        msgid,
        cc,
        ip,
        port,
        type_string,
        parameters,        -- JSON string: [{name,type,value}, ...]
        ws_client_id,
        ws_url,
        send_result,
        error_msg
      FROM tc_command_log
      ${command ? 'WHERE command_name = ?' : ''}
      ORDER BY id DESC
      LIMIT ? OFFSET ?
      `,
      command ? [command, limit, offset] : [limit, offset]
    );

    // query()가 rows 배열을 반환한다고 가정
    const rows = Array.isArray(result) ? result : (result?.rows ?? []);
    return NextResponse.json({ ok: true, rows });
  } catch (e: any) {
    console.error('[tc-log] GET error:', e);
    return NextResponse.json({ ok: false, error: e?.message ?? 'Server error' }, { status: 500 });
  }
}