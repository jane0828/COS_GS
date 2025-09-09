'use client';
import React, { useEffect, useMemo, useRef, useState } from 'react';

const LS_KEY_IP   = 'gs_cmd_last_ip';
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

export default function GS_CommandSelector({ className = '' }) {
  const [commands, setCommands] = useState<Command[]>([]);
  const [selectedTab, setSelectedTab] = useState<string>('');
  const [selectedCommand, setSelectedCommand] = useState<string | number>('');
  const [selectedMsgID, setSelectedMsgID] = useState<string | number>('');
  const [selectedCC, setSelectedCC] = useState<string | number>('');
  const [parameters, setParameters] = useState<ParamMeta[]>([]);
  const [paramValues, setParamValues] = useState<Record<string, any>>({});

  const DEFAULT_IP = '172.31.21.12';
  const DEFAULT_PORT = '2002';
  const [ip, setIp] = useState<string>(DEFAULT_IP);
  const [port, setPort] = useState<string>(DEFAULT_PORT);

  // --- WebSocket (한 번만 연결)
  const wsRef = useRef<WebSocket | null>(null);
  const wsReady = useRef(false);

  useEffect(() => {
    try {
      const savedIp   = localStorage.getItem(LS_KEY_IP);
      const savedPort = localStorage.getItem(LS_KEY_PORT);
      if (savedIp)   setIp(savedIp);
      if (savedPort) setPort(savedPort);
    } catch {}
  }, []);

  useEffect(() => {
    try {
      if (ip && ip.trim()) localStorage.setItem(LS_KEY_IP, ip);
    } catch {}
  }, [ip]);

  useEffect(() => {
    try {
      if (port && port.trim()) localStorage.setItem(LS_KEY_PORT, port);
    } catch {}
  }, [port]);
  
  useEffect(() => {
    const ws = new WebSocket('ws://localhost:4443');
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
      try { ws.close(); } catch (_) {}
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

  // 현재 탭의 명령들
  const msgidGroups = useMemo(
    () => Array.from(new Set(commands.map((c) => c.Name.split('_')[0]))),
    [commands]
  );
  const filteredCommands = useMemo(
    () => commands.filter((c) => (selectedTab ? c.Name.startsWith(selectedTab) : true)),
    [commands, selectedTab]
  );

  // 탭 바뀌면 그 탭에서 NOOP을 기본 선택
  useEffect(() => {
    const noop = filteredCommands.find((c) => c.Name.includes('NOOP'));
    if (noop) {
      setSelectedCommand(noop.No);
      setSelectedMsgID(noop.msgid);
      setSelectedCC(noop.CC);
    } else if (filteredCommands[0]) {
      const first = filteredCommands[0];
      setSelectedCommand(first.No);
      setSelectedMsgID(first.msgid);
      setSelectedCC(first.CC);
    }
  }, [filteredCommands]);

  // 드롭다운에서 CC 변경
  const handleCommandChange = (e: React.ChangeEvent<HTMLSelectElement>) => {
    const cc = e.target.value;
    const found = filteredCommands.find((c) => String(c.CC) === String(cc));
    if (found) {
      setSelectedCommand(found.No);
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

  // 선택된 명령의 메타데이터 가져오기
  useEffect(() => {
    if (!selectedCommand) return;
    (async () => {
      try {
        const res = await fetch(`/api/cmdmeta?command=${selectedCommand}`, { cache: 'no-store' });
        const metaRaw = await res.json();
        const meta = normalizeParams(metaRaw);
        setParameters(meta);
        setParamValues(
          meta.reduce((acc, p) => {
            acc[p.Parameter] = '';
            return acc;
          }, {} as Record<string, any>)
        );
      } catch (e) {
        console.error('Failed to fetch metadata:', e);
        setParameters([]);
        setParamValues({});
      }
    })();
  }, [selectedCommand]);

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
    return 'u32'; // 안전 기본값
  };

  const buildTypeString = (): string =>
    parameters.map(p => toTypeAbbrev(p.ParameterType)).join('');

  const handleSendCommand = () => {
    const ws = wsRef.current;
    if (!ws || !wsReady.current || ws.readyState !== WebSocket.OPEN) {
      console.error('❌ WebSocket is not connected!');
      return;
    }

    const clientId = 'gs634'; // TODO: 실제 클라이언트 ID로
    const payload = {
      type: 'private',
      to: clientId,
      message: {
        IP: ip,  
        Port: Number(port),   
        msgid: Number(selectedMsgID),
        cc: Number(selectedCC), 
        type: buildTypeString(),      // 축약 타입 문자열 (예: "u8u32")
        parameters: parameters.map((p) => ({
          name: p.Parameter,
          type: p.ParameterType,      // 원문 타입 유지 (예: "uint8", "uint32")
          value: paramValues[p.Parameter], // 입력값 그대로 (문자열일 수 있음)
        })),
      },
    };


    ws.send(JSON.stringify(payload));
    console.log('✅ Sent:', payload);
  };
  const handleFocusSelectAll = (e: React.FocusEvent<HTMLInputElement>) => {
    const el = e.target as HTMLInputElement;
    requestAnimationFrame(() => {
      try { el.select(); } catch {}
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
            onClick={() => setSelectedTab(group)}
            className={`px-4 py-2 rounded-md ${
              selectedTab === group ? 'bg-gray-700 font-bold' : 'bg-gray-600 text-gray-200'
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
            <span className="text-green-400 w-1/2 text-left">{String(selectedCC || '')}</span>
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



      <button
        onClick={handleSendCommand}
        className="mt-4 px-4 py-2 bg-blue-500 hover:bg-blue-600 text-white rounded-md w-full"
      >
        Send Command
      </button>
    </div>
  );
}
