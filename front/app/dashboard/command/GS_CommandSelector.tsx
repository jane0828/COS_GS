'use client';
import React, { useEffect, useMemo, useRef, useState } from 'react';

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

  // --- WebSocket (한 번만 연결)
  const wsRef = useRef<WebSocket | null>(null);
  const wsReady = useRef(false);

  useEffect(() => {
    const ws = new WebSocket('ws://192.168.215.6:4443');
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
        msgid: Number(selectedMsgID),
        cc: Number(selectedCC),
        parameters: parameters.map((p) => ({
          name: p.Parameter,
          type: p.ParameterType,
          value: paramValues[p.Parameter],
        })),
      },
    };

    ws.send(JSON.stringify(payload));
    console.log('✅ Sent:', payload);
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

      <button
        onClick={handleSendCommand}
        className="mt-4 px-4 py-2 bg-blue-500 hover:bg-blue-600 text-white rounded-md w-full"
      >
        Send Command
      </button>
    </div>
  );
}
