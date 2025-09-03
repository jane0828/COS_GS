'use client';
import { useState } from "react";
import { orbitron } from "@/app/ui/fonts";

function GS_SSHPanel({ className = "" }) {
  const [command, setCommand] = useState("");
  const [history, setHistory] = useState<string[]>([]);

  const handleSendCommand = () => {
    if (!command.trim()) return; // 빈 명령어 방지

    // SSH 실행 로직 (추후 실제 SSH 실행 함수와 연결 가능)
    const response = `> ${command}\nExecuting...`; // 예제 응답

    setHistory((prev) => [...prev, response]); // 히스토리 추가
    setCommand(""); // 입력 필드 초기화
  };

  return (
    <div className={`${className} p-4 bg-gray-900 text-blue-300 border border-blue-500 rounded-lg`}>
      {/* SSH 입력 & 실행 */}
      <div className="grid grid-cols-[1fr_auto] gap-4 items-center">
        <input
          type="text"
          value={command}
          onChange={(e) => setCommand(e.target.value)}
          placeholder="Enter SSH Command"
          className="px-2 py-2 bg-gray-700 border border-gray-500 rounded-lg text-center font-orbitron flex-1"
        />
        <button
          onClick={handleSendCommand}
          className="w-24 px-2 py-2 bg-pink-500 hover:bg-pink-600 rounded-lg font-bold text-white"
        >
          Send
        </button>
      </div>

      {/* SSH 명령어 히스토리 */}
      <div className="mt-4 p-2 bg-gray-800 border border-gray-600 rounded-lg h-40 overflow-y-auto font-orbitron">
        {history.length === 0 ? (
          <span className="text-gray-500">No commands executed.</span>
        ) : (
          history.map((entry, index) => (
            <div key={index} className="text-green-400 font-mono whitespace-pre-line">
              {entry}
            </div>
          ))
        )}
      </div>
    </div>
  );
}



export default function Page() {
  return (
    <div>
      <GS_SSHPanel        className = "absolute top-[1rem] left-[1rem] md:left-20 p-6 bg-opacity-80 bg-gray-900 border border-blue-500 shadow-[0_0_10px_rgba(0,255,255,0.6)] rounded-lg backdrop-blur-md w-[48rem] h-[17rem]"/>
    </div>
  );
}