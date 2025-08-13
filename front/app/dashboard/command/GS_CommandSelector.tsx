'use client';
import React, { useState, useEffect } from "react";
import axios from "axios";

export default function GS_CommandSelector({ className = "" }) {
  const [commands, setCommands] = useState([]);
  const [selectedTab, setSelectedTab] = useState("");
  const [selectedCommand, setSelectedCommand] = useState("");
  const [selectedMsgID, setSelectedMsgID] = useState("");
  const [selectedCC, setSelectedCC] = useState("");
  const [parameters, setParameters] = useState([]);
  const [paramValues, setParamValues] = useState({});

  useEffect(() => {
    async function fetchCommands() {
      try {
        const response = await fetch("/api/command");
        const data = await response.json();
        setCommands(data);

        const msgidGroups = [...new Set(data.map((cmd) => cmd.Name.split("_")[0]))];
        setSelectedTab(msgidGroups[0] || "");
      } catch (error) {
        console.error("Failed to fetch commands:", error);
      }
    }
    fetchCommands();
  }, []);

  useEffect(() => {
    if (selectedCommand) {
      async function fetchMetadata() {
        try {
          const response = await fetch(`/api/cmdmeta?command=${selectedCommand}`);
          const metadata = await response.json();
          setParameters(metadata);
          setParamValues(metadata.reduce((acc, param) => {
            acc[param.Parameter] = "";
            return acc;
          }, {}));
        } catch (error) {
          console.error("Failed to fetch metadata:", error);
        }
      }
      fetchMetadata();
    }
  }, [selectedCommand]);

  const msgidGroups = [...new Set(commands.map((cmd) => cmd.Name.split("_")[0]))];
  const filteredCommands = commands.filter((cmd) => cmd.Name.startsWith(selectedTab));

  useEffect(() => {
    const noopCommand = filteredCommands.find((cmd) => cmd.Name.includes("NOOP"));
    if (noopCommand) {
      setSelectedCommand(noopCommand.No);
      setSelectedMsgID(noopCommand.msgid);
      setSelectedCC(noopCommand.CC);
    }
  }, [selectedTab]);

  useEffect(() => {
    if (selectedMsgID && selectedCC) {
      const matchedCommand = commands.find((cmd) => cmd.msgid === selectedMsgID && cmd.CC === selectedCC);
      if (matchedCommand) {
        setSelectedCommand(matchedCommand.No);
      }
    }
  }, [selectedMsgID, selectedCC, commands]); // selectedMsgID, selectedCC, commands 변경 시 실행
  

  const handleCommandChange = (e) => {
    const selectedCC = e.target.value;
    const selectedCmd = filteredCommands.find((cmd) => String(cmd.CC) === selectedCC);
  
    if (selectedCmd) {
      setSelectedMsgID(selectedCmd.msgid); // msgID는 유지
      setSelectedCC(selectedCmd.CC);
    }
  
    // 현재 선택된 탭(selectedTab)에 해당하는 NOOP 명령 찾기
    const noopCommand = filteredCommands.find(
      (cmd) => cmd.Name.includes("NOOP") && cmd.Name.startsWith(selectedTab)
    );
  
    if (noopCommand) {
      setSelectedCommand(noopCommand.No); // 선택한 탭 내에서 NOOP 초기화
    }
  };

  const socket = new WebSocket("ws://localhost:4443"); // WebSocket 서버 주소

  const handleSendCommand = () => {
      if (!socket || socket.readyState !== WebSocket.OPEN) {
          console.error("❌ WebSocket is not connected!");
          return;
      }
  
      const clientId = "gs634"; // 실제 클라이언트 ID로 변경 필요
  
      console.log("📤 Sending Command...");
  
      // JSON 데이터 생성
      const jsonData = {
          type: "private",
          to: clientId,
          message: {
              msgid: parseInt(selectedMsgID, 10),
              cc: parseInt(selectedCC, 10),
              parameters: parameters.map(param => ({
                  name: param.Parameter,
                  type: param.ParameterType,
                  value: paramValues[param.Parameter]
              }))
          },
      };
  
      // WebSocket으로 JSON 데이터 전송
      socket.send(JSON.stringify(jsonData));
  
      console.log("✅ Sent:", jsonData);
  };
  
  // WebSocket 이벤트 리스너 추가 (연결, 메시지 수신 등)
  socket.onopen = () => {
      console.log("✅ WebSocket Connected!");
  };
  
  socket.onmessage = (event) => {
      console.log("📩 Message received:", event.data);
  };
  
  socket.onerror = (error) => {
      console.error("❌ WebSocket Error:", error);
  };
  
  socket.onclose = () => {
      console.log("❌ WebSocket Disconnected!");
  };
  

  return (
    <div className={`p-4 bg-gray-800 text-white rounded-md shadow-md ${className}`}>
      {/* Tabs (2줄 레이아웃) */}
      <div className="grid grid-cols-8 sm:grid-cols-8 md:grid-cols-8 gap-2 border-b border-gray-500 mb-4">
        {msgidGroups.map((msgid) => (
          <button
            key={msgid}
            onClick={() => setSelectedTab(msgid)}
            className={`px-4 py-2 rounded-md ${
              selectedTab === msgid ? "bg-gray-700 text-white font-bold" : "bg-gray-500 text-gray-200"
            }`}
          >
            {msgid}
          </button>
        ))}
      </div>

      {/* Command Selector */}
      <div className="relative">
        <select
          className="w-full bg-gray-800 text-white p-2 rounded-md"
          value={selectedCC}
          onChange={handleCommandChange}
        >
          {filteredCommands.map((cmd) => (
            <option key={cmd.CC} value={cmd.CC}>
              {cmd.Name.replace(`${selectedTab}_`, "").replace(/_/g, " ")}
            </option>
          ))}
        </select>
      </div>

      <div className="mt-4 grid grid-cols-[1fr_3fr] gap-4 items-start">
        {/* 왼쪽: msgID & CC 정보 */}
        <div className="flex flex-col justify-center h-full">
          <div className="mb-4 flex items-center">
            <span className="font-bold text-gray-300 text-center w-1/2">msgID</span>
            <span className="text-green-400 text-left w-1/2">{selectedMsgID}</span>
          </div>
          <div className="flex items-center">
            <span className="font-bold text-gray-300 text-center w-1/2">CC</span>
            <span className="text-green-400 text-left w-1/2">{selectedCC}</span>
          </div>
        </div>



        {/* 오른쪽: Parameter 입력 필드 */}
        <div>
          {parameters.map((param) => (
            <div key={param.Parameter} className="mb-2">
              <div className="flex justify-between">
                <label className="block mb-1">{param.Parameter}</label>
                <span className="text-gray-400">{param.ParameterType}</span>
              </div>
              <input
                className="p-2 rounded bg-gray-700 text-white w-full"
                type={param.ParameterType === "uint8" ? "number" : "text"}
                value={paramValues[param.Parameter]}
                onChange={(e) => setParamValues({ ...paramValues, [param.Parameter]: e.target.value })}
              />
            </div>
          ))}
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