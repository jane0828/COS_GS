'use client';
import React, { useEffect, useMemo, useRef, useState } from 'react';

const LS_KEY_IP = 'gs_cmd_last_ip';
const LS_KEY_PORT = 'gs_cmd_last_port';

type Command = {
  No: number | string;
  Name: string;
  msgid: number | string;
  CC: number | string;
};

type ParamMeta = {
  Parameter: string;
  ParameterType: string;
};

type HistoryRow = {
  id: number;
  created_at: string;
  command_name: string;
  msgid: number;
  cc: number;
  ip: string | null;
  port: number | null;
  type_string?: string | null;
  parameters?: string | null; // JSON string: [{name,type,value}]
  ws_client_id?: string | null;
  ws_url?: string | null;
  send_result: 'ok' | 'error';
  error_msg?: string | null;
};




export default function GS_CommandSelector({ className = '' }) {
  const [commands, setCommands] = useState<Command[]>([]);
  const [selectedTab, setSelectedTab] = useState<string>('');
  const [selectedCommand, setSelectedCommand] = useState<string | number>('');
  const [selectedCommandName, setSelectedCommandName] = useState<string>('');
  const [selectedMsgID, setSelectedMsgID] = useState<string | number>('');
  const [selectedCC, setSelectedCC] = useState<string | number>('');
  const [parameters, setParameters] = useState<ParamMeta[]>([]);
  const [paramValues, setParamValues] = useState<Record<string, any>>({});
  const [isSending, setIsSending] = useState(false);
  
  const [history, setHistory] = useState<HistoryRow[]>([]);
  const [selectedHistoryId, setSelectedHistoryId] = useState<number | ''>('');
  const pendingParamsRef = useRef<Record<string, any> | null>(null);
  const suppressAutoSelectRef = useRef(false);
  
  const DEFAULT_IP = '172.31.21.12';
  const DEFAULT_PORT = '2002';
  
  const [ip, setIp] = useState<string>(DEFAULT_IP);
  const [port, setPort] = useState<string>(DEFAULT_PORT);
  
  // --- WebSocket (한 번만 연결)
  const wsRef = useRef<WebSocket | null>(null);
  const wsReady = useRef(false);
  
  useEffect(() => {
    try {
      const savedIp = localStorage.getItem(LS_KEY_IP);
      const savedPort = localStorage.getItem(LS_KEY_PORT);
      if (savedIp) setIp(savedIp);
      if (savedPort) setPort(savedPort);
    } catch { }
  }, []);
  
  useEffect(() => {
    try {
      if (ip && ip.trim()) localStorage.setItem(LS_KEY_IP, ip);
    } catch { }
  }, [ip]);
  
  useEffect(() => {
    try {
      if (port && port.trim()) localStorage.setItem(LS_KEY_PORT, port);
    } catch { }
  }, [port]);
  
  useEffect(() => {
    const ws = new WebSocket('ws://165.132.142.126:4443');
    wsRef.current = ws;
    
    ws.onopen = () => {
      wsReady.current = true;
      console.log('✅ WebSocket Connected!');
    };
    ws.onmessage = (e) => console.log('📩 Message:', e.data);
    ws.onerror = (err) => console.error('❌ WebSocket Error:', err);
    ws.onclose = () => {
      wsReady.current = false;
      console.log('❌ WebSocket Disconnected!');
    };
    
    return () => {
      try { ws.close(); } catch (_) { }
      wsRef.current = null;
    };
  }, []);
  
  // 명령 목록 가져오기
  useEffect(() => {
    (async () => {
      try {
        const res = await fetch('/api/command', { cache: 'no-store' });
        const data: Command[] = await res.json();
        setCommands(Array.isArray(data) ? data : []);
        
        const groups = Array.from(new Set(data.map((c) => c.Name.split('_')[0])));
        setSelectedTab(groups[0] ?? '');
      } catch (e) {
        console.error('Failed to fetch commands:', e);
      }
    })();
  }, []);
  
  async function loadHistory(limit = 30) {
    try {
      const res = await fetch(`/api/tc-log?limit=${limit}`, { cache: 'no-store' });
      const data = await res.json();
      if (data?.ok && Array.isArray(data.rows)) setHistory(data.rows);
    } catch (e) {
      console.error('Failed to load history:', e);
    }
  }
  
  useEffect(() => { loadHistory(30); }, []);
  
  // 현재 탭의 명령들
  const msgidGroups = useMemo(
    () => Array.from(new Set(commands.map((c) => c.Name.split('_')[0]))),
    [commands]
  );
  const filteredCommands = useMemo(() => {
    return commands
      .filter((c) => (selectedTab ? c.Name.startsWith(selectedTab) : true))
      .sort((a, b) => Number(a.CC) - Number(b.CC)); // CC 오름차순 정렬
  }, [commands, selectedTab]);

  // // 탭 바뀌면 그 탭에서 NOOP을 기본 선택
  // useEffect(() => {
  //   const noop = filteredCommands.find((c) => c.Name.includes('NOOP'));
  //   const base = noop ?? filteredCommands[0];
  //   if (base) {
  //     setSelectedCommand(base.No);
  //     setSelectedCommandName(base.Name);  // ← 이름 설정
  //     setSelectedMsgID(base.msgid);
  //     setSelectedCC(base.CC);
  //   }
  // }, [filteredCommands]);

  // 드롭다운에서 CC 변경
  const handleCommandChange = (e: React.ChangeEvent<HTMLSelectElement>) => {
    const cc = e.target.value;
    const found = filteredCommands.find((c) => String(c.CC) === String(cc));
    if (found) {
      setSelectedCommand(found.No);
      setSelectedCommandName(found.Name);
      setSelectedMsgID(found.msgid);
      setSelectedCC(found.CC);
    }
  };

  // 응답 정규화: 어떤 형태로 와도 배열로 변환
  function normalizeParams(raw: any): ParamMeta[] {
    if (Array.isArray(raw)) return raw;
    if (raw?.params && Array.isArray(raw.params)) return raw.params;
    if (raw?.rows && Array.isArray(raw.rows)) return raw.rows;
    // {0:{...},1:{...}} 같은 케이스
    if (raw && typeof raw === 'object') {
      const vals = Object.values(raw);
      if (vals.every((v) => v && typeof v === 'object')) {
        return vals as ParamMeta[];
      }
    }
    return [];
  }
  function applyHistoryRow(h: HistoryRow) {
      // 탭/명령 맞추기
    suppressAutoSelectRef.current = true;
    const group = h.command_name.split('_')[0] || '';
    setSelectedTab(group);

    // 가능한 경우 동일 Name로 찾고, 아니면 msgid+cc로
    const byName = commands.find((c) => c.Name === h.command_name);
    const byIdCc = commands.find((c) => Number(c.msgid) === Number(h.msgid) && Number(c.CC) === Number(h.cc));
    const target = byName ?? byIdCc;

    if (target) {
      setSelectedCommand(target.No);
      setSelectedCommandName(target.Name);
      setSelectedMsgID(target.msgid);
      setSelectedCC(target.CC);
    } else {
      setSelectedCommandName(h.command_name);
      setSelectedMsgID(h.msgid);
      setSelectedCC(h.cc);
    }

    // IP/Port
    if (h.ip) setIp(h.ip);
    if (h.port != null) setPort(String(h.port));

    // 파라미터(JSON → Record)
    const record: Record<string, any> = {};
    try {
      const arr = h.parameters ? JSON.parse(h.parameters) : [];
      if (Array.isArray(arr)) {
        for (const it of arr) {
          if (it && typeof it === 'object' && 'name' in it) {
            record[it.name] = it.value ?? '';
          }
        }
      }
    } catch {}

    pendingParamsRef.current = record;
    setSelectedHistoryId(h.id);

    requestAnimationFrame(() => setSelectedHistoryId(''));
  }

  useEffect(() => {
    if (!filteredCommands.length) return;
    // ① 히스토리 적용 중이면 어떤 변경도 하지 말고 바로 종료
    if (suppressAutoSelectRef.current) {
      suppressAutoSelectRef.current = false;
      return;
    }
    // ② 이미 선택된 명령 이름이 있으면(히스토리/사용자 선택) 건드리지 않음
    if (selectedCommandName) return;
    // ③ 아무것도 선택되지 않았을 때만 기본값 지정
    const base = filteredCommands.find(c => c.Name.includes('NOOP')) ?? filteredCommands[0];
    if (base) {
      setSelectedCommand(base.No);
      setSelectedCommandName(base.Name);
      setSelectedMsgID(base.msgid);
      setSelectedCC(base.CC);
    }
  }, [filteredCommands, selectedCommandName]);

  // 선택된 명령의 메타데이터 가져오기
  useEffect(() => {
    if (!selectedCommandName) return;
    let abort = false;
    (async () => {
      try {
        const url = `/api/cmdmeta?name=${encodeURIComponent(selectedCommandName)}`;
        const res = await fetch(url, { cache: 'no-store' });
        const metaRaw = await res.json();
        if (abort) return;

        const meta = normalizeParams(metaRaw);
        setParameters(meta);

        const empty = meta.reduce((acc, p) => { acc[p.Parameter] = ''; return acc; }, {} as Record<string, any>);
        let baseValues = empty;
        
        if (pendingParamsRef.current) {
          const fromHist = pendingParamsRef.current;
          const merged: Record<string, any> = { ...empty };
          for (const k of Object.keys(empty)) {
            if (Object.prototype.hasOwnProperty.call(fromHist, k)) merged[k] = fromHist[k];
          }
          baseValues = merged;
          pendingParamsRef.current = null;
        }
        setParamValues(baseValues); 
      } catch (e) {
        if(abort) return;
        console.error('Failed to fetch metadata:', e);
        setParameters([]);
        setParamValues({});
      }
    })();
    return () => { abort = true; };
  }, [selectedCommandName, selectedMsgID, selectedCC]);

  const toTypeAbbrev = (tRaw: string): string => {
    const t = (tRaw || '').toLowerCase().replace(/[^a-z0-9]/g, '');
    if (t.includes('uint8')) return 'u8';
    if (t.includes('uint16')) return 'u16';
    if (t.includes('uint32')) return 'u32';
    if (t.includes('int8')) return 'i8';
    if (t.includes('int16')) return 'i16';
    if (t.includes('int32')) return 'i32';
    if (t.includes('float') || t === 'f32') return 'f32';
    if (t.includes('double') || t === 'f64') return 'f64';
    // fallback: 유사 키워드
    if (t.includes('u8')) return 'u8';
    if (t.includes('u16')) return 'u16';
    if (t.includes('u32')) return 'u32';
    if (t.includes('str64')) return 'str64';
    return 'u32'; // 안전 기본값
  };
  const isStringType = (tRaw: string): boolean => {
  const t = (tRaw || '').toLowerCase().replace(/\s+/g, '');
  if (t.includes('string') || t === 'str') return true;
  if (t.includes('char*') || /char\[\d*\]/.test(t)) return true;
  if (t.includes('path') || t.includes('filename') || t.includes('file')) return true;
  return false;
  };

  const buildTypeString = (): string =>{
    return parameters.map((p) => {
      if (isStringType(p.ParameterType)) {
        const raw = paramValues[p.Parameter] ?? '';
        const bytes = new TextEncoder().encode(String(raw)).length; // UTF-8 바이트 길이
        return `str${bytes}`;
      }
      return toTypeAbbrev(p.ParameterType);
    }).join('');
  };


  async function logTcCommand(args: {
    command_name: string;
    msgid: number;
    cc: number;
    ip?: string | null;
    port?: number | null;
    type_string?: string | null;
    parameters: Array<{ name: string; type: string; value: any }>;
    ws_client_id?: string | null;
    ws_url?: string | null;
    send_result: 'ok' | 'error';
    error_msg?: string | null;
  }) {
    const res = await fetch('/api/tc-log', {
      method: 'POST',
      headers: { 'Content-Type': 'application/json' },
      body: JSON.stringify(args),
    });
    const data = await res.json();
    if (!res.ok || !data?.ok) {
      throw new Error(data?.error || 'Failed to write log');
    }
    return data;
  }

  const handleSendCommand = async () => {
    if (!wsRef.current || wsRef.current.readyState !== WebSocket.OPEN) {
      console.error('❌ WebSocket is not connected!');
      return;
    }

    const clientId = 'gs634';
    const wsUrl = 'ws://165.132.142.126:4443';

    const msgPayload = {
      type: 'private',
      to: clientId,
      message: {
        IP: ip,
        Port: Number(port),
        msgid: Number(selectedMsgID),
        cc: Number(selectedCC),
        type: buildTypeString() || null,
        parameters: parameters.map((p) => ({
          name: p.Parameter,
          type: p.ParameterType,
          value: paramValues[p.Parameter],
        })),
      },
    };

    setIsSending(true);
    let sendResult: 'ok' | 'error' = 'ok';
    let errMsg: string | null = null;

    try {
      // 1) WS 전송
      wsRef.current.send(JSON.stringify(msgPayload));

      // 2) DB 로그
      await logTcCommand({
        command_name: String(selectedCommandName || ''),
        msgid: Number(selectedMsgID),
        cc: Number(selectedCC),
        ip,
        port: Number(port),
        type_string: buildTypeString() || null,
        parameters: msgPayload.message.parameters,
        ws_client_id: clientId,
        ws_url: wsUrl,
        send_result: sendResult,
        error_msg: errMsg,
      });

      console.log('✅ Log saved');

      await loadHistory(30);
    } catch (err: any) {
      sendResult = 'error';
      errMsg = err?.message || String(err);
      console.error('❌ Log error:', errMsg);

      // 실패도 기록(선택)
      try {
        await logTcCommand({
          command_name: String(selectedCommandName || ''),
          msgid: Number(selectedMsgID),
          cc: Number(selectedCC),
          ip,
          port: Number(port),
          type_string: buildTypeString() || null,
          parameters: msgPayload.message.parameters,
          ws_client_id: clientId,
          ws_url: wsUrl,
          send_result: 'error',
          error_msg: errMsg,
        });
      } catch { }
    } finally {
      setIsSending(false);
    }
  };
  const handleFocusSelectAll = (e: React.FocusEvent<HTMLInputElement>) => {
    const el = e.target as HTMLInputElement;
    requestAnimationFrame(() => {
      try { el.select(); } catch { }
    });
  };
  const restoreIfEmpty = (
    value: string,
    restore: (v: string) => void,
    defVal: string
  ) => {
    if (value.trim() === '') restore(defVal);
  };

  return (
    <div className={`p-4 bg-gray-800 text-white rounded-md shadow-md ${className}`}>
      {/* Tabs */}
      <div className="grid grid-cols-8 gap-2 border-b border-gray-500 mb-4">
        {msgidGroups.map((group) => (
          <button
            key={group}
            onClick={() => {
              // 탭 변경
              setSelectedTab(group);

              // 이 탭의 명령 목록에서 NOOP 우선, 없으면 첫 번째
              const groupCmds = commands
                .filter(c => c.Name.startsWith(group + "_"))
                .sort((a, b) => Number(a.CC) - Number(b.CC));

              const base =
                groupCmds.find(c => /(^|_)NOOP(_|$)/.test(c.Name)) ?? groupCmds[0];

              if (base) {
                setSelectedCommand(base.No);
                setSelectedCommandName(base.Name);
                setSelectedMsgID(base.msgid);
                setSelectedCC(base.CC);
              }

              // 히스토리에서 넘어온 파라미터 주입은 비활성화
              pendingParamsRef.current = null;
            }}
            className={`px-4 py-2 rounded-md ${selectedTab === group ? 'bg-gray-700 font-bold' : 'bg-gray-600 text-gray-200'
              }`}
          >
            {group}
          </button>
        ))}
      </div>

      {/* Command selector */}
      <div className="relative">
        <select
          className="w-full bg-gray-800 text-white p-2 rounded-md"
          value={String(selectedCC)}
          onChange={handleCommandChange}
        >
          {filteredCommands.map((cmd) => {
            // Name이 해당 탭 prefix로 시작할 때만 prefix 제거
            let displayName = cmd.Name;
            if (cmd.Name.startsWith(selectedTab + "_")) {
              displayName = cmd.Name.slice(selectedTab.length + 1);
            }
            return (
              <option key={`${cmd.msgid}-${cmd.CC}`} value={String(cmd.CC)}>
                {displayName.replace(/_/g, ' ')}
              </option>
            );
          })}
        </select>
      </div>

      <div className="mt-4 grid grid-cols-[1fr_3fr] gap-4 items-start">
        {/* Left: msgID / CC */}
        <div className="flex flex-col justify-center">
          <div className="mb-4 flex items-center">
            <span className="font-bold text-gray-300 w-1/2 text-center">msgID</span>
            <span className="text-green-400 w-1/2 text-left">{String(selectedMsgID || '')}</span>
          </div>
          <div className="flex items-center">
            <span className="font-bold text-gray-300 w-1/2 text-center">CC</span>
            <span className="text-green-400 w-1/2 text-left">{String(selectedCC ?? '')}</span>
          </div>
        </div>

        {/* Right: Parameters */}
        <div>
          {parameters.length === 0 ? (
            <p className="text-gray-400">No parameters</p>
          ) : (
            parameters.map((param) => (
              <div key={param.Parameter} className="mb-2">
                <div className="flex justify-between">
                  <label className="block mb-1">{param.Parameter}</label>
                  <span className="text-gray-400">{param.ParameterType}</span>
                </div>
                <input
                  className="p-2 rounded bg-gray-700 text-white w-full"
                  type={param.ParameterType.startsWith('uint') ? 'number' : 'text'}
                  value={paramValues[param.Parameter] ?? ''}
                  onChange={(e) =>
                    setParamValues((prev) => ({ ...prev, [param.Parameter]: e.target.value }))
                  }
                />
              </div>
            ))
          )}
        </div>
      </div>

      <div className="mt-4 grid grid-cols-2 gap-3">
        <div>
          <label className="block text-sm text-gray-300 mb-1">IP</label>
          <input
            className="p-2 rounded bg-gray-700 text-gray-200 placeholder-gray-400 w-full caret-white text-left"
            placeholder={DEFAULT_IP}
            value={ip}
            onChange={(e) => setIp(e.target.value.replace(/[^\d.]/g, ''))} // 숫자/점만
            onFocus={handleFocusSelectAll}
            onMouseUp={(e) => e.preventDefault()} // 일부 브라우저에서 선택 해제 방지
            onBlur={() => restoreIfEmpty(ip, setIp, DEFAULT_IP)}           // 빈칸이면 기본값 복구
            type="text"
            inputMode="numeric"
          />
        </div>
        <div>
          <label className="block text-sm text-gray-300 mb-1">Port</label>
          <input
            className="p-2 rounded bg-gray-700 text-gray-200 placeholder-gray-400 w-full caret-white text-left"
            placeholder={DEFAULT_PORT}
            value={port}
            onChange={(e) => setPort(e.target.value.replace(/[^\d]/g, ''))} // 숫자만
            onFocus={handleFocusSelectAll}
            onMouseUp={(e) => e.preventDefault()}
            onBlur={() => restoreIfEmpty(port, setPort, DEFAULT_PORT)}       // 빈칸이면 기본값 복구
            type="text"
            inputMode="numeric"
            pattern="[0-9]*"
          />
        </div>
      </div>

      <div className="mt-4 grid grid-cols-1 md:grid-cols-[1fr_auto_auto] gap-2 items-center">
        <select
          className="bg-gray-800 text-gray-200 p-2 rounded-md w-full"
          value={selectedHistoryId}
          onChange={(e) => {
            const id = e.target.value ? Number(e.target.value) : '';
            setSelectedHistoryId(id as any);
            if (id !== '') {
              const row = history.find(r => r.id === id);
              if (row) applyHistoryRow(row);
            }
          }}
        >
          <option value="">History: 최근 전송 선택…</option>
          {history.map((h) => (
            <option key={h.id} value={h.id}>
              {`${new Date(h.created_at).toLocaleString()} — ${h.command_name} (msg:${h.msgid} / cc:${h.cc}) @ ${h.ip ?? '-'}:${h.port ?? '-'}`}
            </option>
          ))}
        </select>

        <button
          onClick={() => loadHistory(30)}
          className="px-3 py-2 bg-gray-700 hover:bg-gray-600 rounded-md"
          title="DB 히스토리 새로고침"
        >
          Reload
        </button>

        <button
          onClick={handleSendCommand}
          className="px-4 py-2 bg-blue-500 hover:bg-blue-600 text-white rounded-md w-full md:w-auto"
          disabled={isSending}
        >
          {isSending ? 'Sending…' : 'Send Command'}
        </button>
      </div>

    </div>
  );
}
